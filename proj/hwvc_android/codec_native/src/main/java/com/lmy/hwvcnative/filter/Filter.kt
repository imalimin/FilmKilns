/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative.filter

import com.lmy.hwvcnative.CPPObject

abstract class Filter : CPPObject() {
    abstract fun setParams(params: IntArray)
    abstract fun setParam(index: Int, value: Int)

    companion object {
        const val FILTER_BASE = 100
        const val FILTER_NONE = 2333
    }
}