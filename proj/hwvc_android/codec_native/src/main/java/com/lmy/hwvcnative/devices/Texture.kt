/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.devices

/**
 * Created by lmyooyo@gmail.com on 2018/3/27.
 */
interface Texture {
    fun init()
    fun draw(transformMatrix: FloatArray?)
}