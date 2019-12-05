package com.lmy.hwvcnative.entity

class AlRational(var num: Int = 0, ///< Numerator
                 var den: Int = 1 ///< Denominator
) {
    companion object {
        fun zero(): AlRational = AlRational(0, 1)
    }
}