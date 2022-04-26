/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*
* AUTHOR: ${author}
* CREATE TIME: 2022-4-26 13:18:24
*/

#include "FkCanvasMatProgram.h"

FkCanvasMatProgram::FkCanvasMatProgram(const FkProgramDescription &desc) : FkGraphicMatProgram(desc) {
    FK_MARK_SUPER
}

FkCanvasMatProgram::~FkCanvasMatProgram() {

}

std::string FkCanvasMatProgram::getFragment() {
    std::string shader(R"(
        precision mediump float;
        varying mediump vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main(){
            vec4 back = vec4(1.0, 1.0, 1.0, 1.0);
            float ix = vTextureCoord.x / 0.1;
            float iy = vTextureCoord.y / 0.1;
            if (iy / 2.0 - floor(iy / 2.0) >= 0.0 && iy / 2.0 - floor(iy / 2.0) < 1.0) {
                    back = vec4(0.796, 0.796, 0.796, 1.0);
            } else {
            }
            vec4 front = texture2D(uTexture, vTextureCoord);
            vec4 color = mix(front, back, 1.0 - front.a);
            gl_FragColor = color;
        })");
    return shader;
}