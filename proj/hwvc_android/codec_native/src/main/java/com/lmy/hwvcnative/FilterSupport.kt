/*
 * Copyright (c) 2018-present, aliminabc@gmail.com.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */
package com.lmy.hwvcnative

import com.lmy.hwvcnative.filter.Filter

interface FilterSupport {
    fun setFilter(filter: Filter)
    fun getFilter(): Filter?
    fun invalidate()
}