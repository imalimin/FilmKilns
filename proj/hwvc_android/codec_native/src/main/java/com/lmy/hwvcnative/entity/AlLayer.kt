package com.lmy.hwvcnative.entity

class AlLayer(var id: Int,
              var width: Int,
              var height: Int) {
    companion object {
        fun none(): AlLayer = AlLayer(-1, 0, 0)
    }
}