/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/RGBA2NV12Filter.h"
#include "../include/NormalDrawer.h"

static const string VERTEX = SHADER(
        attribute vec4 aPosition;
        attribute vec2 aTextureCoord;
        varying vec2 vTextureCoord;
        void main(void) {
            gl_Position = aPosition;
            vTextureCoord = aTextureCoord;
        }
);

static const string FRAGMENT = SHADER(
        precision mediump float;
        varying vec2 vTextureCoord;
        uniform sampler2D uTexture;
        uniform int width;

        float y(vec3 c) {
            return c.r * 0.257 + c.g * 0.504 + c.b * 0.098 + 0.0625;
        }

        float u(vec3 c) {
            return -0.148 * c.r - 0.291 * c.g + 0.439 * c.b + 0.5000;
        }

        float v(vec3 c) {
            return 0.439 * c.r - 0.368 * c.g - 0.071 * c.b + 0.5000;
        }

        float divide(float num1, float num2) {
            if (0.0 == num2) {
                return 0.0;
            }
            return num1 / num2;
        }

        void main(void) {
            vec2 offset = vec2(1.0 / float(width), 0.0);
            if (vTextureCoord.y < 0.666667) {// Y
                vec2 pos = vec2(vTextureCoord.x, divide(vTextureCoord.y, 0.666667));
                vec3 color0 = texture2D(uTexture, pos).rgb;
                vec3 color1 = texture2D(uTexture, pos + offset).rgb;
                vec3 color2 = texture2D(uTexture, pos + offset * 2.0).rgb;
                vec3 color3 = texture2D(uTexture, pos + offset * 3.0).rgb;

                gl_FragColor = vec4(y(color0), y(color1), y(color2), y(color3));
            } else {// UV
                vec2 pos = vec2(vTextureCoord.x, divide((vTextureCoord.y - 0.666667), 0.333333));
                vec3 color0 = texture2D(uTexture, pos).rgb;
                vec3 color1 = texture2D(uTexture, pos + offset).rgb;
                vec3 color2 = texture2D(uTexture, pos + offset * 2.0).rgb;
                vec3 color3 = texture2D(uTexture, pos + offset * 3.0).rgb;

                gl_FragColor = vec4(u(color0), v(color0), u(color2), v(color2));
            }
        }
);

RGBA2NV12Filter::RGBA2NV12Filter() : Filter() {
    name = __func__;
}

RGBA2NV12Filter::~RGBA2NV12Filter() {

}

bool RGBA2NV12Filter::init(int w, int h) {
    if (!Filter::init(w / 4, h * 3 / 2))
        return false;
    drawer = new NormalDrawer(VERTEX, FRAGMENT);
    this->widthLocation = drawer->getUniformLocation("width");
    return true;
}

void RGBA2NV12Filter::bindResources() {
    Filter::bindResources();
    glUniform1i(this->widthLocation, getFrameBuffer()->width() * 4);
}

void RGBA2NV12Filter::draw(GLuint texture) {
    glDisable(GL_BLEND);
    glViewport(0, 0, getFrameBuffer()->width(), getFrameBuffer()->height());
    Filter::draw(texture);
}
