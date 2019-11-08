/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

/**
 * Created by aliminabc@gmail.com on 2018/3/27.
 */
interface Texture {
    fun init()
    fun draw(transformMatrix: FloatArray?)
}