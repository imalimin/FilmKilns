package com.alimin.fk.entity

class FkRational(var num: Int = 0, ///< Numerator
                 var den: Int = 1 ///< Denominator
) {
    companion object {
        fun zero(): FkRational = FkRational(0, 1)
    }
}