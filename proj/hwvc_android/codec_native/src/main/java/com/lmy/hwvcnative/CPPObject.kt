/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative

abstract class CPPObject {
    internal var handler: Long = 0

    fun isNativeNull(): Boolean = NULL == handler

    open fun onNativeMessage(what: Int, arg1: Int) {

    }

    //    internal abstract fun create(): Long
    companion object {
        const val NULL = 0L
    }
}