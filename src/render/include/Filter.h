//
// Created by limin on 2018/12/16.
//

#ifndef HARDWAREVIDEOCODEC_FILTER_H
#define HARDWAREVIDEOCODEC_FILTER_H

#include <string>
#include "Object.h"
#include "HwAbsFrameBuffer.h"
#include "BaseDrawer.h"

using namespace std;

const int FILTER_BASE = 100;
const int FILTER_NONE = 2333;

class Filter : public Object {
public:
    string name;

    Filter();

#ifdef ANDROID

    /**
     * Enable direct texture for android, if device supports.
     * Default disable.
     */
    Filter(bool requestHwMode);

#endif

    virtual ~Filter();

    virtual bool init(int w, int h);

    virtual void draw(GLuint texture);

    virtual void bindResources();

    HwAbsFrameBuffer *getFrameBuffer();

    virtual void setParams(int *params);

    virtual void setParam(int key, int value);

protected:
    BaseDrawer *drawer = nullptr;
private:
    HwAbsFrameBuffer *fbo = nullptr;
    bool initialized = false;

#ifdef ANDROID
private:
    bool requestHwMode = false;
#endif
};


#endif //HARDWAREVIDEOCODEC_FILTER_H
