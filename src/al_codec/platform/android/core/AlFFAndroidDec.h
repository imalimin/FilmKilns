/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef HWVC_ANDROID_ALFFANDROIDDEC_H
#define HWVC_ANDROID_ALFFANDROIDDEC_H

#define AL_MEDIA_CODEC_DEC_NAME "al_h264_mediacodec"

#if defined(__AL_ENABLE_MEDIA_CODEC__)

class AlFFAndroidDec {
public:
    static void reg();
};

#define al_media_codec_dec_register() AlFFAndroidDec::reg()
#else
#define al_media_codec_dec_register()
#endif

#endif //HWVC_ANDROID_ALFFANDROIDDEC_H
