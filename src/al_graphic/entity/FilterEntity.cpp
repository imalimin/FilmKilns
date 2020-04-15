/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "FilterEntity.h"
#include "AlXMLDocument.h"
#include "AlObjectGuard.h"
#include "Base64.h"
#include "Logcat.h"

#define TAG "FilterEntity"
#define ATTR_KEY  "key"
#define VERSION   "version"
#define NAME      "name"
#define VERTEX    "vertex"
#define FRAGMENT  "fragment"
#define PARAM     "param"
#define SAMPLER   "sampler"

FilterEntity *FilterEntity::fromFile(std::string file) {
    AlXMLDocument *doc = AlXMLDocument::fromFile(file);
    AlObjectGuard guard((Object **) &doc);
    if (nullptr == doc) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return nullptr;
    }
    AlElement *root = doc->root();
    if (!root->nameIs("hwvc_filter")) {
        Logcat::e(TAG, "%s(%d) failed", __FUNCTION__, __LINE__);
        return nullptr;
    }

    FilterEntity *entity = new FilterEntity();
    size_t size = root->size();
    for (int i = 0; i < size; ++i) {
        AlElement *child = root->childAt(i);
        if (child->nameIs(VERSION)) {
            entity->version = child->attrInt("");
        } else if (child->nameIs(NAME)) {
            entity->name = child->text();
        } else if (child->nameIs(VERTEX)) {
            entity->vertex = child->text();
        } else if (child->nameIs(FRAGMENT)) {
            entity->fragment = child->text();
        } else if (child->nameIs(PARAM)) {
            string key = child->attr(ATTR_KEY);
            string value = child->text();
            entity->params[key] = static_cast<float>(atof(value.c_str()));
        } else if (child->nameIs(SAMPLER)) {
            string key = child->attr(ATTR_KEY);
            string value = child->text();
            entity->samplers[key] = base64_decode(value);
        }
    }
    return entity;
}

FilterEntity::FilterEntity() {

}

FilterEntity::~FilterEntity() {

}
