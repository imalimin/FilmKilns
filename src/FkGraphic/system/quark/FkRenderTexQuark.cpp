/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-2-25 13:17:35
*/

#include "FkRenderTexQuark.h"
#include "FkNewTexProto.h"
#include "FkRenderProto.h"
#include "FkGLDefinition.h"
#include "FkSizeCompo.h"
#include "FkColorCompo.h"
#include "FkNewBmpTexProto.h"
#include "FkBufCompo.h"
#include "FkRmMaterialProto.h"
#include "FkMemUtils.h"
#include "FkBitmap.h"
#include "FkString.h"
#include "FkRenderContext.h"

FK_IMPL_CLASS_TYPE(FkRenderTexQuark, FkQuark)

FkRenderTexQuark::FkRenderTexQuark() : FkQuark() {

}

FkRenderTexQuark::~FkRenderTexQuark() {
}

void FkRenderTexQuark::describeProtocols(std::shared_ptr<FkPortDesc> desc) {
    FK_PORT_DESC_QUICK_ADD(desc, FkNewTexProto, FkRenderTexQuark::_onAllocTex);
    FK_PORT_DESC_QUICK_ADD(desc, FkNewBmpTexProto, FkRenderTexQuark::_onAllocTexWithBmp);
    FK_PORT_DESC_QUICK_ADD(desc, FkRenderProto, FkRenderTexQuark::_onRender);
    FK_PORT_DESC_QUICK_ADD(desc, FkRmMaterialProto, FkRenderTexQuark::_onRemoveTex);
}

FkResult FkRenderTexQuark::onCreate() {
    return FkQuark::onCreate();
}

FkResult FkRenderTexQuark::onDestroy() {
    return FkQuark::onDestroy();
}

FkResult FkRenderTexQuark::onStart() {
    auto ret = FkQuark::onStart();
    if (FK_OK != ret) {
        return ret;
    }
    allocator = std::make_shared<FkGraphicAllocator>(256 * 1024 * 1024);
    return ret;
}

FkResult FkRenderTexQuark::onStop() {
    sMap.clear();
    allocator->release();
    allocator = nullptr;
    return FkQuark::onStop();
}

FkResult FkRenderTexQuark::_onAllocTex(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkNewTexProto, p);
    if (allocator == nullptr) {
        FkLogE(FK_DEF_TAG, "Texture allocator is null.");
        return FK_FAIL;
    }
    auto materialId = proto->texEntity->getMaterial()->id();
    auto texArray = _findTex(materialId);
    if (texArray != nullptr && !texArray->empty() && proto->texEntity->size() != texArray->size) {
        sMap.erase(proto->texEntity->getMaterial()->id());
        texArray = nullptr;
        FkLogI(FK_DEF_TAG, "Texture size has change. Request new one.");
    }
    bool isMakeNewTex = (texArray == nullptr || texArray->empty());
    if (isMakeNewTex) {
        texArray = _allocTex(proto->texEntity);
        sMap.insert(std::make_pair(proto->texEntity->getMaterial()->id(), texArray));
    }
    auto deviceImage = proto->texEntity->getMaterial()->getDeviceImage();
    if (deviceImage != nullptr && deviceImage->type() == FkDeviceImage::kType::Android) {
        // Do somethings
        deviceImage->onCreate(&(texArray->textures[0]->tex), -1, -1);
    } else {
        auto bufCompo = FK_FIND_COMPO(proto->texEntity, FkBufCompo);
        if (bufCompo) {
            _updateTexWithBuf(texArray, proto->texEntity, bufCompo->buf);
        } else if (isMakeNewTex || proto->texEntity->size() != texArray->size) {
            _updateTexWithBuf(texArray, proto->texEntity, nullptr);
        }
        auto ret = _drawColor(texArray, proto->texEntity);
        if (FK_OK != ret) {
            FkLogD(FK_DEF_TAG, "Skip draw color. No color component.");
        }
    }
    return FK_OK;
}

FkResult FkRenderTexQuark::_onAllocTexWithBmp(std::shared_ptr<FkProtocol> &p) {
    return _onAllocTex(p);
}

FkResult FkRenderTexQuark::_onRender(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRenderProto, p);
    auto material = proto->materials->getMaterial();
    auto texArray = _findTex(material->id());
    if (texArray == nullptr || texArray->empty()) {
        return FK_SOURCE_NOT_FOUND;
    }
    proto->materials->addComponent(std::make_shared<FkTexArrayCompo>(*texArray));
    auto sizeCompo = FK_FIND_COMPO(proto->materials, FkSizeCompo);
    if (sizeCompo == nullptr) {
        sizeCompo = std::make_shared<FkSizeCompo>(texArray->size);
        proto->materials->addComponent(sizeCompo);
    }
    sizeCompo->size = texArray->size;
    if (!proto->device->getMaterial()->isInvalid()) {
        texArray = _findTex(proto->device->getMaterial()->id());
        proto->device->addComponent(std::make_shared<FkTexArrayCompo>(*texArray));
        sizeCompo = FK_FIND_COMPO(proto->device, FkSizeCompo);
        if (sizeCompo == nullptr) {
            sizeCompo = std::make_shared<FkSizeCompo>(texArray->size);
            proto->device->addComponent(sizeCompo);
        }
        sizeCompo->size = texArray->size;
    }
    return FK_OK;
}

std::shared_ptr<FkTexArrayCompo> FkRenderTexQuark::_findTex(FkID id) {
    auto itr = sMap.find(id);
    if (itr != sMap.end()) {
        return itr->second;
    }
    return {};
}

std::shared_ptr<FkTexArrayCompo> FkRenderTexQuark::_allocTex(std::shared_ptr<FkTexEntity> &texEntity) {
    int32_t blockSize = _calcBestBlockSize(texEntity->size());
    FkAssert(blockSize > 0, nullptr);
    FkTexDescription desc(GL_TEXTURE_2D);
    desc.fmt = texEntity->format();
    auto blocks = _calcTexBlock(texEntity->size(), blockSize);
    auto deviceImage = texEntity->getMaterial()->getDeviceImage();
    if (deviceImage && deviceImage->type() == FkDeviceImage::kType::Android) {
        desc.target = GL_TEXTURE_EXTERNAL_OES;
        blocks.x = 1;
        blocks.y = 1;
        FkLogI(FK_DEF_TAG, "Use GL_TEXTURE_EXTERNAL_OES (%dx%d) for material %d", texEntity->size().getWidth(), texEntity->size().getHeight(), texEntity->getMaterial()->id());
    }
    auto blockSizeX = blocks.x == 1 ? texEntity->size().getWidth() : blockSize;
    auto blockSizeY = blocks.y == 1 ? texEntity->size().getHeight() : blockSize;
    FkLogI(FK_DEF_TAG, "blockSize: %d, %dx%d", blockSize, blockSizeX, blockSizeY);
    auto texArray = std::make_shared<FkTexArrayCompo>(texEntity->size(),
                                                      blocks.x, blocks.y,
                                                      blockSizeX, blockSizeY);
    for (int y = 0; y < blocks.y; ++y) {
        for (int x = 0; x < blocks.x; ++x) {
            desc.size = {blockSizeX, blockSizeY};
            if (blocks.x == x + 1 && blocks.y == y + 1) {
                desc.size = {texEntity->size().getWidth() - blockSizeX * x,
                             texEntity->size().getHeight() - blockSizeY * y};
            } else if (blocks.x == x + 1) {
                desc.size = {texEntity->size().getWidth() - blockSizeX * x, blockSizeY};
            } else if (blocks.y == y + 1) {
                desc.size = {blockSizeX, texEntity->size().getHeight() - blockSizeY * y};
            }
            auto tex = allocator->alloc(desc);
            if (nullptr == tex) {
                FkLogE(FK_DEF_TAG, "Texture allocator return null.");
                return {};
            }
            texArray->textures.emplace_back(tex);
        }
    }
    FkLogI(FK_DEF_TAG, "Tex allocator usage: %d + %d / %d",
           allocator->usingSize(), allocator->cacheSize(), allocator->capacity());
    return texArray;
}

FkResult FkRenderTexQuark::_updateTexWithBuf(std::shared_ptr<FkTexArrayCompo> &texArray,
                                             std::shared_ptr<FkTexEntity> &texEntity,
                                             std::shared_ptr<FkBuffer> buf) {
    auto startTime = FkTimeUtils::getCurrentTimeUS();
    FkIntVec2 pos(0, 0);
    std::shared_ptr<FkBuffer> dstBuf = nullptr;
    for (int y = 0; y < texArray->blocks.y; ++y) {
        pos.x = 0;
        for (int x = 0; x < texArray->blocks.x; ++x) {
            auto index = y * texArray->blocks.x + x;
            auto tex = (*texArray)[index];
            auto size = tex->desc.size.getWidth() * tex->desc.size.getHeight() * 4;
            if (buf) {
                if (dstBuf == nullptr || dstBuf->capacity() != size) {
                    dstBuf = FkBuffer::alloc(size);
                }
                auto ret = _copySubImage(buf, texArray->size.getWidth(), dstBuf, tex->desc.size,
                                         pos);
                FkAssert(ret == FK_OK, ret);
            }
            tex->update(tex->desc.fmt, tex->desc.size.getWidth(), tex->desc.size.getHeight(),
                        dstBuf ? dstBuf->data() : nullptr);
#if 0
            glViewport(0, 0, tex->desc.size.getWidth(), tex->desc.size.getHeight());
            auto fboCompo = FK_FIND_COMPO(texEntity, FkFboCompo);
            if (fboCompo && buf) {
                fboCompo->fbo->bind();
                fboCompo->fbo->attach(tex);
                auto buf1 = FkBuffer::alloc(tex->desc.size.getWidth() * tex->desc.size.getHeight() * 4);
                glReadPixels(0, 0,
                             tex->desc.size.getWidth(), tex->desc.size.getHeight(),
                             GL_RGBA, GL_UNSIGNED_BYTE, buf1->data());
                FkString path("/storage/emulated/0/Android/data/com.alimin.pic/cache/image_sub_");
                path.append(index);
                path.append(".jpg");
                auto ret = FkBitmap::write(path.c_str(), FkImage::Format::kJPEG, buf1, tex->desc.size, 80);
                FkLogI("aliminabc", "%d, %d", index, ret);
                fboCompo->fbo->detach(tex->desc.target);
                fboCompo->fbo->unbind();
            }
#endif
            if (texArray->blocks.x == x + 1) {
                pos.y += tex->desc.size.getHeight();
            }
            pos.x += tex->desc.size.getWidth();
        }
    }
    FkLogD(FK_DEF_TAG, "_updateTexWithBuf cost %" PRId64, FkTimeUtils::getCurrentTimeUS() - startTime);
    return FK_OK;
}

FkResult FkRenderTexQuark::_drawColor(std::shared_ptr<FkTexArrayCompo> &texArray,
                                      std::shared_ptr<FkTexEntity> &texEntity) {
    for (auto &tex: texArray->textures) {
        auto ret = _drawColor(tex, texEntity);
        FkAssert(FK_OK == FK_OK, ret);
    }
    return FK_OK;
}

FkResult FkRenderTexQuark::_drawColor(std::shared_ptr<FkGraphicTexture> &tex,
                                      std::shared_ptr<FkTexEntity> &texEntity) {
    auto colorCompo = FK_FIND_COMPO(texEntity, FkColorCompo);
    auto fboCompo = texEntity->fbo();
    if (colorCompo && fboCompo) {
        auto size = tex->desc.size;
        fboCompo->fbo->bind();
        fboCompo->fbo->attach(tex);
        glViewport(0, 0, size.getWidth(), size.getHeight());
        glClearColor(colorCompo->color.fRed(), colorCompo->color.fGreen(),
                     colorCompo->color.fBlue(), colorCompo->color.fAlpha());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        fboCompo->fbo->detach(tex->desc.target);
        fboCompo->fbo->unbind();
        return FK_OK;
    }
    return FK_FAIL;
}

FkResult FkRenderTexQuark::_onRemoveTex(std::shared_ptr<FkProtocol> &p) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(proto, FkRmMaterialProto, p);
    if (proto->material == nullptr) {
        return FK_NPE;
    }
    auto itr = sMap.find(proto->material->id());
    if (itr != sMap.end()) {
        sMap.erase(itr);
        auto deviceImage = proto->material->getDeviceImage();
        if (deviceImage) {
            deviceImage->onDestroy();
        }
    }
    return FK_OK;
}

FkIntVec2 FkRenderTexQuark::_calcTexBlock(FkSize size, int32_t blockSize) {
    return {(int32_t) std::ceil(size.getWidth() / (float) blockSize),
            (int32_t) std::ceil(size.getHeight() / (float) blockSize)};
}

FkResult FkRenderTexQuark::_copySubImage(std::shared_ptr<FkBuffer> &src, int32_t width,
                                         std::shared_ptr<FkBuffer> &dst, FkSize size,
                                         FkIntVec2 pos) {
    return FkMemUtils::copySubImage(src, width, dst, size, pos);
}

int32_t FkRenderTexQuark::_calcBestBlockSize(FkSize size) {
    FK_CAST_NULLABLE_PTR_RETURN_INT(context, FkRenderContext, getContext());
    std::vector<int> supportBlockArray = {4096, 1024, 512};
    supportBlockArray.emplace_back(context->getMaxTextureSize());
    std::sort(supportBlockArray.begin(), supportBlockArray.end(),
              [](int a, int b) { return a > b; });
    int32_t blockSize = supportBlockArray[0];
    for (auto it: supportBlockArray) {
        auto x = (int32_t) std::ceil(size.getWidth() * 1.0f / it);
        auto y = (int32_t) std::ceil(size.getHeight() * 1.0f / it);
        if (x * y <= context->getMaxCountOfFragmentTexture()) {
            blockSize = it;
        }
    }
    return blockSize;
}
