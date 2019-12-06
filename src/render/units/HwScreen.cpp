//
// Created by mingyi.li on 2018/12/25.
//

#include "HwScreen.h"
#include "NormalDrawer.h"
#include "HwProgram.h"
#include "Size.h"
#include "Logcat.h"

#define TAG "HwScreen"

HwScreen::HwScreen(string alias) : Unit(alias) {
    registerEvent(EVENT_SCREEN_DRAW, reinterpret_cast<EventFunc>(&HwScreen::eventDraw));
    registerEvent(EVENT_SCREEN_UPDATE_WINDOW,
                  reinterpret_cast<EventFunc>(&HwScreen::eventUpdateWindow));
}

HwScreen::~HwScreen() {
}

bool HwScreen::onDestroy(Message *msg) {
    Logcat::i(TAG, "Screen::onDestroy");
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

bool HwScreen::onCreate(Message *msg) {
    Logcat::i(TAG, "Screen::onCreate");
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
    NativeWindow *nw = static_cast<NativeWindow *>(msg->tyrUnBox());
    if (egl && egl->updateWindow(nw->win)) {
        this->width = nw->win->getWidth();
        this->height = nw->win->getHeight();
        Logcat::i("HwScreen", "%s(%d)", __FUNCTION__, __LINE__);
    }
    return true;
}

bool HwScreen::eventDraw(Message *msg) {
    Size *size = static_cast<Size *>(msg->tyrUnBox());
    GLuint tex = static_cast<GLuint>(msg->arg1);
    egl->makeCurrent();
    if (egl->isAttachWindow()) {
        setScaleType(size->width, size->height);
        draw(tex);
    }
    delete size;
    return true;
}

void HwScreen::initWindow(NativeWindow *nw) {
    if (!egl) {
        if (nw) {
            if (nw->hasContext()) {
                egl = Egl::create(nw->context, nw->win, true);
                Logcat::i(TAG, "Screen::init EGL with context %d x %d", egl->width(),
                          egl->height());
            } else {
                egl = Egl::create(nullptr, nw->win, true);
                nw->context = egl->getContext();
                Logcat::i(TAG, "Screen::init EGL %d x %d", egl->width(), egl->height());
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
    Logcat::i(TAG, "Screen::eventDraw %d, %dx%d", texture, egl->width(), egl->height());
    glViewport(0, 0, egl->width(), egl->height());
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
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