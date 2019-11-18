//
// Created by mingyi.li on 2018/12/25.
//

#include "../include/HwScreen.h"
#include "../include/NormalDrawer.h"
#include "Size.h"
#include "Logcat.h"

HwScreen::HwScreen(string alias) : Unit(alias) {
    registerEvent(EVENT_COMMON_PREPARE, reinterpret_cast<EventFunc>(&HwScreen::eventPrepare));
    registerEvent(EVENT_SCREEN_DRAW, reinterpret_cast<EventFunc>(&HwScreen::eventDraw));
    registerEvent(EVENT_SCREEN_UPDATE_WINDOW,
                  reinterpret_cast<EventFunc>(&HwScreen::eventUpdateWindow));
}

HwScreen::~HwScreen() {
}

bool HwScreen::eventRelease(Message *msg) {
    Logcat::i("HWVC", "Screen::eventRelease");
    if (egl) {
        egl->makeCurrent();
    }
    if (drawer) {
        delete drawer;
        drawer = nullptr;
    }
    if (egl) {
        delete egl;
        egl = nullptr;
    }
    return true;
}

bool HwScreen::eventPrepare(Message *msg) {
    Logcat::i("HWVC", "Screen::eventPrepare");
    if (msg->obj) {
        NativeWindow *nw = static_cast<NativeWindow *>(msg->tyrUnBox());
        this->width = nw->win->getWidth();
        this->height = nw->win->getHeight();
        initWindow(nw);
    } else {
        initWindow(nullptr);
    }
    return true;
}

bool HwScreen::eventUpdateWindow(Message *msg) {
    Logcat::i("HWVC", "Screen::eventUpdateWindow");
    NativeWindow *nw = static_cast<NativeWindow *>(msg->tyrUnBox());
    if (egl) {
        this->width = nw->win->getWidth();
        this->height = nw->win->getHeight();
        egl->updateWindow(nw->win);
    }
    return true;
}

bool HwScreen::eventDraw(Message *msg) {
    Size *size = static_cast<Size *>(msg->tyrUnBox());
    GLuint tex = static_cast<GLuint>(msg->arg1);
    egl->makeCurrent();
    setScaleType(size->width, size->height);
    draw(tex);
    delete size;
    return true;
}

void HwScreen::initWindow(NativeWindow *nw) {
    if (!egl) {
        if (nw) {
            if (nw->hasContext()) {
                egl = Egl::create(nw->context, nw->win, true);
                Logcat::i("HWVC", "Screen::init EGL with context %d x %d", egl->width(),
                          egl->height());
            } else {
                egl = Egl::create(nullptr, nw->win, true);
                nw->context = egl->getContext();
                Logcat::i("HWVC", "Screen::init EGL %d x %d", egl->width(), egl->height());
            }
        } else {
            egl = Egl::create(nullptr, nullptr, true);
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
    if (egl->isAttachWindow()) {
        Logcat::i("HWVC", "Screen::eventDraw %d, %dx%d", texture, egl->width(), egl->height());
        glViewport(0, 0, egl->width(), egl->height());
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        drawer->draw(texture);
    }
    egl->swapBuffers();
}

void HwScreen::setScaleType(int dw, int dh) {
    int viewWidth = egl->width();
    int viewHeight = egl->height();
    float viewScale = viewWidth / (float) viewHeight;
    float picScale = dw / (float) dh;

    int destViewWidth = viewWidth;
    int destViewHeight = viewHeight;
    if (viewScale > picScale) {
        destViewWidth = (int) (viewHeight * picScale);
    } else {
        destViewHeight = (int) (viewWidth / picScale);
    }
    float left = -destViewWidth / (float) viewWidth;
    float right = -left;
    float bottom = -destViewHeight / (float) viewHeight;
    float top = -bottom;

    float *texCoordinate = new float[8]{
            0.0f, 0.0f,//LEFT,BOTTOM
            1.0f, 0.0f,//RIGHT,BOTTOM
            0.0f, 1.0f,//LEFT,TOP
            1.0f, 1.0f//RIGHT,TOP
    };
    float *position = new float[8]{
            left, bottom, //LEFT,BOTTOM
            right, bottom, //RIGHT,BOTTOM
            left, top, //LEFT,TOP
            right, top,//RIGHT,TOP
    };

    drawer->updateLocation(texCoordinate, position);
    delete[]texCoordinate;
    delete[]position;
}