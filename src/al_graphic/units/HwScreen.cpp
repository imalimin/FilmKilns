//
// Created by mingyi.li on 2018/12/25.
//

#include "HwScreen.h"
#include "NormalDrawer.h"
#include "HwProgram.h"
#include "Size.h"
#include "ObjectBox.h"
#include "Logcat.h"

#define TAG "HwScreen"

HwScreen::HwScreen(string alias) : Unit(alias) {
    registerEvent(EVENT_SCREEN_DRAW, reinterpret_cast<EventFunc>(&HwScreen::eventDraw));
    registerEvent(EVENT_SCREEN_DRAW_TEX, reinterpret_cast<EventFunc>(&HwScreen::onDrawTex));
    registerEvent(EVENT_SCREEN_UPDATE_WINDOW,
                  reinterpret_cast<EventFunc>(&HwScreen::eventUpdateWindow));
}

HwScreen::~HwScreen() {
}

bool HwScreen::onDestroy(AlMessage *msg) {
    Logcat::i(TAG, "Screen::onDestroy");
    if (egl) {
        egl->makeCurrent();
    }
    delete drawer;
    drawer = nullptr;
    delete egl;
    egl = nullptr;
    return true;
}

bool HwScreen::onCreate(AlMessage *msg) {
    Logcat::i(TAG, "Screen::onCreate");
    return true;
}

bool HwScreen::eventUpdateWindow(AlMessage *msg) {
    NativeWindow *nw = msg->getObj<NativeWindow *>();
    if (nullptr == egl) {
        initWindow(nw);
    } else {
        if (egl->updateWindow(nw->win)) {
            Logcat::i(TAG, "%s(%d): updateWindow failed", __FUNCTION__, __LINE__);
        }
    }
    this->width = nw->win->getWidth();
    this->height = nw->win->getHeight();
    auto *m = AlMessage::obtain(EVENT_SCREEN_UPDATE_NOTIFY);
    m->arg1 = this->width;
    m->arg2 = this->height;
    postEvent(m);
    return true;
}

bool HwScreen::eventDraw(AlMessage *msg) {
    if (nullptr == egl) {
        return true;
    }
    Size *size = msg->getObj<ObjectBox *>()->unWrap<Size *>();
    GLuint tex = static_cast<GLuint>(msg->arg1);
    egl->makeCurrent();
    if (egl->isAttachWindow()) {
        setScaleType(size->width, size->height);
        draw(tex);
    }
    delete size;
    return true;
}

bool HwScreen::onDrawTex(AlMessage *msg) {
    if (nullptr == egl) {
        return true;
    }
    HwAbsTexture *tex = static_cast<HwAbsTexture *>(msg->obj);
    egl->makeCurrent();
    if (egl->isAttachWindow()) {
        setScaleType(tex->getWidth(), tex->getHeight());
        draw(tex->texId());
    }
    return true;
}

void HwScreen::initWindow(NativeWindow *nw) {
    if (!egl) {
        if (nw) {
            EGLContext c = AlEgl::currentContext();
            egl = AlEgl::window(nw->win, c);
            Logcat::i(TAG, "Screen::init EGL with context %p, size %d x %d",
                      c, egl->width(), egl->height());
        } else {
            Logcat::e(TAG, "Screen::init EGL ERROR");
        }
        egl->makeCurrent();
        drawer = new NormalDrawer();
        drawer->setRotation(ROTATION_VERTICAL);
    } else {

    }
}

void HwScreen::draw(GLuint texture) {
//    string glslVersion = (const char *) glGetString(GL_SHADING_LANGUAGE_VERSION);
//    LOGE("version: %s", glslVersion.c_str());
//    Logcat::i(TAG, "Screen::eventDraw %d, %dx%d", texture, egl->width(), egl->height());
    glViewport(0, 0, egl->width(), egl->height());
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawer->draw(texture);
    egl->swapBuffers();
}

void HwScreen::setScaleType(int dw, int dh) {
    float *texCoordinate = new float[8]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    float *vertex = new float[8];
    HwProgram::calculateFitWinVertex(vertex, Size(dw, dh), Size(egl->width(), egl->height()));
    drawer->updateLocation(texCoordinate, vertex);
    delete[]texCoordinate;
    delete[]vertex;
}