/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "AlFFAndroidDec.h"
#include "AlAndroidCodecDecCompat.h"
#include "HwVideoFrame.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/fifo.h"
#include "libavutil/opt.h"
#include "libavutil/attributes.h"
#include "libavutil/imgutils.h"
#include "libavcodec/avcodec.h"
#include "libavcodec/internal.h"
}

#define TAG "AlFFAndroidDec"

#if defined(__AL_ENABLE_MEDIA_CODEC__)

typedef struct _AlMediaCodecDecContext {
    const AVClass *av_class;
    AlAndroidCodecDecCompat *ctx;
    AVBSFContext *bsf;
    AVPacket pkt;
    AVFifoBuffer *fifo;
    AVFrame *frame;
} _AlMediaCodecDecContext;

static av_cold int al_media_codec_dec_close(AVCodecContext *ctx) {
    AlLogI(TAG, "");
    _AlMediaCodecDecContext *s = (_AlMediaCodecDecContext *) ctx->priv_data;
    if (s && s->ctx) {
        delete s->ctx;
        s->ctx = nullptr;
    }
    if (s->frame) {
        av_frame_free(&s->frame);
        s->frame = nullptr;
    }
    av_fifo_free(s->fifo);
    av_bsf_free(&s->bsf);
    av_packet_unref(&s->pkt);
    return 0;
}

static av_cold int al_media_codec_dec_init(AVCodecContext *ctx) {
    AlLogI(TAG, "");
    int ret = 0, sizeOfSPS = 0, sizeOfPPS = 0;
    uint8_t sps[1024] = {0};
    uint8_t pps[1024] = {0};
    _AlMediaCodecDecContext *s = (_AlMediaCodecDecContext *) ctx->priv_data;
    const AVBitStreamFilter *bsf = av_bsf_get_by_name("h264_mp4toannexb");
    if (nullptr == bsf) {
        al_media_codec_dec_close(ctx);
        AlLogE(TAG, "failed.");
        return AVERROR_BSF_NOT_FOUND;
    }
    ret = av_bsf_alloc(bsf, &s->bsf);
    if (0 != ret) {
        al_media_codec_dec_close(ctx);
        return ret;
    }
    ret = avcodec_parameters_from_context(s->bsf->par_in, ctx);
    if (ret < 0) {
        al_media_codec_dec_close(ctx);
        AlLogE(TAG, "failed.");
        return ret;
    }
    ret = av_bsf_init(s->bsf);
    if (ret < 0) {
        al_media_codec_dec_close(ctx);
        AlLogE(TAG, "failed.");
        return ret;
    }
    av_init_packet(&s->pkt);
    if (ctx->extradata) {
        av_free(ctx->extradata);
        ctx->extradata = (uint8_t *) av_malloc((size_t) s->bsf->par_out->extradata_size);
        ctx->extradata_size = s->bsf->par_out->extradata_size;

        memcpy(ctx->extradata, s->bsf->par_out->extradata, ctx->extradata_size);
    }

    memcpy(sps, ctx->extradata, ctx->extradata_size);
    sizeOfSPS = ctx->extradata_size;
    sizeOfPPS = 0;
    auto spsBuf = std::shared_ptr<AlBuffer>(AlBuffer::wrap(sps, sizeOfSPS));
    auto ppsBuf = std::shared_ptr<AlBuffer>(AlBuffer::wrap(pps, sizeOfPPS));

    AlBundle format;
    format.put(AlCodec::KEY_WIDTH, ctx->width);
    format.put(AlCodec::KEY_HEIGHT, ctx->height);
    format.put(AlCodec::KEY_DURATION, INT64_C(1000));
    format.put(AlCodec::KEY_CSD_0, (int64_t) spsBuf.get());
    format.put(AlCodec::KEY_CSD_1, (int64_t) ppsBuf.get());

    s->ctx = new AlAndroidCodecDecCompat(AlCodec::kID::H264);
    if (Hw::OK != s->ctx->configure(format)) {
        al_media_codec_dec_close(ctx);
        AlLogE(TAG, "failed.");
        return AVERROR_UNKNOWN;
    }

    s->fifo = av_fifo_alloc(sizeof(AVPacket));
    if (nullptr == s->fifo) {
        al_media_codec_dec_close(ctx);
        AlLogE(TAG, "failed.");
        return AVERROR_UNKNOWN;
    }
    AVPixelFormat pixFmt = AV_PIX_FMT_YUV420P;
    s->frame = av_frame_alloc();
    s->frame->width = ctx->width;
    s->frame->height = ctx->height;
    s->frame->format = pixFmt;
    auto *buf = (uint8_t *) av_malloc(
            avpicture_get_size(pixFmt, s->frame->width,
                               s->frame->height));
    ret = av_image_fill_arrays(s->frame->data, s->frame->linesize, buf, pixFmt,
                               s->frame->width, s->frame->height, 1);
    return ret;
}

static int al_media_codec_decprocess(AVCodecContext *ctx, AVFrame *frame,
                                     int *got, AVPacket *pkt) {
    int ret = -1;
    uint8_t *buf = nullptr;
    int size = 0;
    _AlMediaCodecDecContext *s = (_AlMediaCodecDecContext *) ctx->priv_data;

    HwAbsMediaFrame *dst = nullptr;
    HwPacket *src = HwPacket::wrap(pkt->data, pkt->size, pkt->pts, pkt->dts, pkt->flags);
    HwResult nRet = s->ctx->process(&dst, &src);
    if (Hw::OK != nRet) {
        *got = 0;
        return pkt->size;
    }
    *got = 1;
    HwVideoFrame *dstFrame = dynamic_cast<HwVideoFrame *>(dst);
    frame->width = dstFrame->getWidth();
    frame->height = dstFrame->getHeight();
    frame->format = HwAbsMediaFrame::convertVideoFrameFormat(dstFrame->getFormat());
    frame->pts = dstFrame->getPts();
    frame->pkt_dts = AV_NOPTS_VALUE;
    frame->data[0] = dstFrame->data();
    frame->linesize[0] = dstFrame->getWidth();

//    frame->buf[0] = av_buffer_create(NULL,
//                                     0,
//                                     mediacodec_buffer_release,
//                                     pTextureFrameST,
//                                     AV_BUFFER_FLAG_READONLY);
    return pkt->size;
}

static int al_media_codec_dec_receive_frame(AVCodecContext *avctx, AVFrame *frame) {
//    int ret = -1;
//    AVFrame *frame = static_cast<AVFrame *>(data);
//    _AlMediaCodecDecContext *s = (_AlMediaCodecDecContext *) ctx->priv_data;
//    if (pkt->size) {
//        AVPacket tmp = {0};
//
//        if (sizeof(AVPacket) > av_fifo_space(s->fifo)) {
//            ret = av_fifo_realloc2(s->fifo, sizeof(AVPacket) + av_fifo_size(s->fifo));
//            if (ret < 0) {
//                AlLogE(TAG, "failed.");
//                return ret;
//            }
//        }
//        ret = av_packet_ref(&tmp, pkt);
//        if (ret < 0) {
//            AlLogE(TAG, "failed.");
//            return ret;
//        }
//        av_fifo_generic_write(s->fifo, &tmp, sizeof(AVPacket), nullptr);
//    }
//    ret = 0;
//    while (!ret) {
//        if (s->pkt.size <= 0) {
//            AVPacket tmp = {0};
//            av_packet_unref(&s->pkt);
//
//            if (sizeof(AVPacket) > av_fifo_size(s->fifo)) {
//                if (pkt->size) {
//                    return pkt->size;
//                } else {
//                    return al_media_codec_decprocess(ctx, frame, &ret, pkt);
//                }
//            }
//
//            av_fifo_generic_read(s->fifo, &tmp, sizeof(AVPacket), nullptr);
//
//            if (!s->bsf) {
//                av_packet_move_ref(&s->pkt, &tmp);
//            } else {
//                if ((ret = av_bsf_send_packet(s->bsf, &tmp)) < 0)
//                    return ret;
//                ret = av_bsf_receive_packet(s->bsf, &s->pkt);
//                if (ret == AVERROR(EAGAIN)) {
//                    break;
//                }
//            }
//            if (ret < 0) {
//                return ret;
//            }
//        }
//
//        ret = al_media_codec_decprocess(ctx, frame, &ret, &s->pkt);
//        if (ret < 0)
//            return ret;
//
//        s->pkt.size -= ret;
//        s->pkt.data += ret;
//        break;
//    }
//
//    if (ret && !frame->buf[0]) {
//        AlLogE(TAG, "failed.");
//    }
//
//    return pkt->size;
    return 0;
}

static void al_media_codec_dec_flush(AVCodecContext *ctx) {
    AlLogI(TAG, "");
    _AlMediaCodecDecContext *s = (_AlMediaCodecDecContext *) ctx->priv_data;
    s->ctx->flush();
    while (av_fifo_size(s->fifo)) {
        AVPacket pkt;
        av_fifo_generic_read(s->fifo, &pkt, sizeof(pkt), NULL);
        av_packet_unref(&pkt);
    }
    av_fifo_reset(s->fifo);
}

const AVOption options[] = {
        {NULL},
};

const AVClass al_media_codec_dec_class = {
        .class_name = AL_MEDIA_CODEC_DEC_NAME,
        .item_name  = av_default_item_name,
        .option     = options,
        .version    = LIBAVUTIL_VERSION_INT,
};

AVCodec al_media_codec_dec = {
        .name           = AL_MEDIA_CODEC_DEC_NAME,
        .long_name      = NULL, /// "H.264 MediaCodec Decoder"
        .type           = AVMEDIA_TYPE_VIDEO,
        .id             = AV_CODEC_ID_H264,
        .priv_data_size = sizeof(_AlMediaCodecDecContext),
        .priv_class     = &al_media_codec_dec_class,
        .init           = al_media_codec_dec_init,
        .receive_frame  = al_media_codec_dec_receive_frame,
        .flush          = al_media_codec_dec_flush,
        .close          = al_media_codec_dec_close,
        .capabilities   = AV_CODEC_CAP_DELAY,
        .caps_internal  = FF_CODEC_CAP_INIT_THREADSAFE,
};

void AlFFAndroidDec::reg() {
    AlLogI(TAG, "");
    avcodec_register(&al_media_codec_dec);
}

#endif ///__AL_ENABLE_MEDIA_CODEC__
