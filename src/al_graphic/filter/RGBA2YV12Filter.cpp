/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "../include/RGBA2YV12Filter.h"
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
        uniform int height;

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
            vec2 offsetY = vec2(0.0, 1.0 / float(height));
            if (vTextureCoord.y < 0.666667) {// Y
                vec2 pos = vec2(vTextureCoord.x, divide(vTextureCoord.y, 0.666667));
                vec3 color0 = texture2D(uTexture, pos).rgb;
                vec3 color1 = texture2D(uTexture, pos + offset).rgb;
                vec3 color2 = texture2D(uTexture, pos + offset * 2.0).rgb;
                vec3 color3 = texture2D(uTexture, pos + offset * 3.0).rgb;

                gl_FragColor = vec4(y(color0), y(color1), y(color2), y(color3));
            } else if (vTextureCoord.y >= 0.666667 && vTextureCoord.y < 0.888883) {//U
                vec2 pos = vec2(vTextureCoord.x * 2.0,
                                divide((vTextureCoord.y - 0.666667), 0.166667));
                if (pos.x >= 1.0) {
                    pos = pos - vec2(1.0, 0.0) + offsetY * 2.0;
                }
                vec3 color0 = texture2D(uTexture, pos).rgb;
                vec3 color1 = texture2D(uTexture, pos + offset * 2.0).rgb;
                vec3 color2 = texture2D(uTexture, pos + offset * 4.0).rgb;
                vec3 color3 = texture2D(uTexture, pos + offset * 6.0).rgb;

                gl_FragColor = vec4(u(color0), u(color1), u(color2), u(color3));
            } else {//V
                vec2 pos = vec2(vTextureCoord.x * 2.0,
                                divide((vTextureCoord.y - 0.888883), 0.166667));
                if (pos.x >= 1.0) {
                    pos = pos - vec2(1.0, 0.0) + offsetY * 2.0;
                }
                vec3 color0 = texture2D(uTexture, pos).rgb;
                vec3 color1 = texture2D(uTexture, pos + offset * 2.0).rgb;
                vec3 color2 = texture2D(uTexture, pos + offset * 4.0).rgb;
                vec3 color3 = texture2D(uTexture, pos + offset * 6.0).rgb;

                gl_FragColor = vec4(v(color0), v(color1), v(color2), v(color3));
            }
        }
);

RGBA2YV12Filter::RGBA2YV12Filter() : Filter() {
    name = __func__;
}

RGBA2YV12Filter::~RGBA2YV12Filter() {

}

bool RGBA2YV12Filter::init(int w, int h) {
    if (!Filter::init(w / 4, h * 3 / 2))
        return false;
    drawer = new NormalDrawer(VERTEX, FRAGMENT);
    this->widthLocation = drawer->getUniformLocation("width");
    this->heightLocation = drawer->getUniformLocation("height");
    return true;
}

void RGBA2YV12Filter::bindResources() {
    Filter::bindResources();
    glUniform1i(this->widthLocation, getFrameBuffer()->width() * 4);
    glUniform1i(this->heightLocation, getFrameBuffer()->height() * 2 / 3);
}

void RGBA2YV12Filter::draw(GLuint texture) {
    glDisable(GL_BLEND);
    glViewport(0, 0, getFrameBuffer()->width(), getFrameBuffer()->height());
    Filter::draw(texture);
}