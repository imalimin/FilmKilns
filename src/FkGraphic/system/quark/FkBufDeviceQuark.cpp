/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: Alimin
* CREATE TIME: 2022-2-26 12:42:33
*/

#include "FkBufDeviceQuark.h"
#include "FkRenderProto.h"
#include "FkTexEntity.h"
#include "FkGLDefinition.h"

FK_IMPL_CLASS_TYPE(FkBufDeviceQuark, FkQuark)

FkBufDeviceQuark::FkBufDeviceQuark() : FkQuark() {

}

FkBufDeviceQuark::~FkBufDeviceQuark() {
}

void FkBufDeviceQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkBufDeviceQuark::_onRender);
}

FkResult FkBufDeviceQuark::onCreate() {
    auto ret = FkQuark::onCreate();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkBufDeviceQuark::onDestroy() {
    auto ret = FkQuark::onDestroy();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkBufDeviceQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    return ret;
}

FkResult FkBufDeviceQuark::onStop() {
    return FkQuark::onStop();
}

FkResult FkBufDeviceQuark::_onRender(std::shared_ptr<FkProtocol> p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    if (!FK_INSTANCE_OF(proto->device, FkBufDeviceEntity)) {
        return FK_SKIP;
    }
    FK_CAST_NULLABLE_PTR_RETURN_INT(device, FkBufDeviceEntity, proto->device);
    FK_CAST_NULLABLE_PTR_RETURN_INT(material, FkTexEntity, proto->materials);
    auto fboCompo = material->fbo();
    if (fboCompo == nullptr) {
        return FK_NPE;
    }
    auto texArray = material->texArray();
    if (texArray->empty()) {
        FkLogW(FK_DEF_TAG, "Not support for multi-texture frame buffer object.");
        return FK_SKIP;
    }
    glFinish();
    fboCompo->fbo->bind();
    auto texSize = material->size();
    auto dstSize = device->getSize();
    if (dstSize.isZero()) {
        dstSize = texSize;
    }
    auto pos = device->getPosition();

    auto bytesOfSlice = texArray->blockSize.getWidth() * texArray->blockSize.getHeight() * 4;
    auto tmpBuf = FkBuffer::alloc(bytesOfSlice);
    FkIntVec2 tmpPos(0, 0);
    for (int y = 0; y < texArray->blocks.y; ++y) {
        tmpPos.x = 0;
        for (int x = 0; x < texArray->blocks.x; ++x) {
            int index = y * texArray->blocks.x + x;
            auto tex = (*texArray)[index];
            FkIntVec2 readPos(std::max(pos.x - tmpPos.x, 0), std::max(pos.y - tmpPos.y, 0));
            FkSize readSize(std::min(dstSize.getWidth() + pos.x - tmpPos.x - readPos.x, tex->desc.size.getWidth() - readPos.x),
                            std::min(dstSize.getHeight()+ pos.y - tmpPos.y - readPos.y, tex->desc.size.getHeight() - readPos.y));
            if (readSize.getWidth() > 0 && readSize.getHeight() > 0) {
                memset(tmpBuf->data(), 0, bytesOfSlice);
                fboCompo->fbo->attach(tex);
                glReadPixels(readPos.x, readPos.y,
                             readSize.getWidth(), readSize.getHeight(),
                             GL_RGBA, GL_UNSIGNED_BYTE, tmpBuf->data());
                fboCompo->fbo->detach(tex->desc.target);
                auto dst = device->buffer();
                _copySubBuffer(tmpBuf, readSize, dst, dstSize, readPos + tmpPos - pos);
            }
            if (texArray->blocks.x == x + 1) {
                tmpPos.y += tex->desc.size.getHeight();
            }
            tmpPos.x += tex->desc.size.getWidth();
        }
    }
    fboCompo->fbo->unbind();
    return FK_OK;
}

bool FkBufDeviceQuark::_copySubBuffer(std::shared_ptr<FkBuffer> &src, FkSize srcSize,
                                      std::shared_ptr<FkBuffer> &dst, FkSize dstSize, FkIntVec2 pos) {
    for (int i = 0; i < srcSize.getHeight(); ++i) {
        auto dstOffset = ((pos.y + i) * dstSize.getWidth() + pos.x) * 4;
        auto srcOffset = i * srcSize.getWidth() * 4;
        memcpy(dst->data() + dstOffset,
               src->data() + srcOffset,
               srcSize.getWidth() * 4);
    }
    return true;
}