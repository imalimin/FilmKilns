/*
* Copyright (c) 2018-present, lmyooyo@gmail.com.
*
* This source code is licensed under the GPL license found in the
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
        float offset = 0.0011574074;
        void main(void) {
            if(vTextureCoord.y < 0.666667) {// Y
                vec2 pos = vec2(vTextureCoord.x, divide(1.0, (vTextureCoord.y)));
                vec4 color0 = vec4(texture2D(uTexture, pos).rgb, 1.0);
                vec4 color1 = vec4(texture2D(uTexture, vec2(pos.x + offset, pos.y)).rgb, 1.0);
                vec4 color2 = vec4(texture2D(uTexture, vec2(pos.x + offset * 2.0, pos.y)).rgb, 1.0);
                vec4 color3 = vec4(texture2D(uTexture, vec2(pos.x + offset * 3.0, pos.y)).rgb, 1.0);

                gl_FragColor = vec4(y(color0), y(color1), y(color2), y(color3));
            } else if (vTextureCoord.y >= 0.666667 && vTextureCoord.y < 0.833333){//U
                vec2 pos = vec2(vTextureCoord.x, divide(1.0, (vTextureCoord.y - 0.666667)));
                vec4 color0 = vec4(texture2D(uTexture, pos).rgb, 1.0);
                vec4 color1 = vec4(texture2D(uTexture, vec2(pos.x + offset, pos.y)).rgb, 1.0);
                vec4 color2 = vec4(texture2D(uTexture, vec2(pos.x + offset * 2.0, pos.y)).rgb, 1.0);
                vec4 color3 = vec4(texture2D(uTexture, vec2(pos.x + offset * 3.0, pos.y)).rgb, 1.0);

                gl_FragColor = vec4(u(color0), u(color1), u(color2), u(color3));
            } else {//V
                vec2 pos = vec2(vTextureCoord.x, divide(1.0, (vTextureCoord.y - 0.833333)));
                vec4 color0 = vec4(texture2D(uTexture, pos).rgb, 1.0);
                vec4 color1 = vec4(texture2D(uTexture, vec2(pos.x + offset, pos.y)).rgb, 1.0);
                vec4 color2 = vec4(texture2D(uTexture, vec2(pos.x + offset * 2.0, pos.y)).rgb, 1.0);
                vec4 color3 = vec4(texture2D(uTexture, vec2(pos.x + offset * 3.0, pos.y)).rgb, 1.0);

                gl_FragColor = vec4(v(color0), v(color1), v(color2), v(color3));
            }
        }

        float y(vec4 c) {
            return c.r * 0.257 + c.g * 0.504 + c.b * 0.098 + 0.0625;
        }

        float u(vec4 c) {
            return -0.148 * c.r - 0.291 * c.g + 0.439 * c.b + 0.5000;
        }

        float v(vec4 c) {
            return 0.439 * c.r - 0.368 * c.g - 0.071 * c.b + 0.5000;
        }

        float divide(float num1, float num2) {
            if (0.0 == num2) {
                return 0.0;
            }
            return num1 / num2;
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
    return true;
}