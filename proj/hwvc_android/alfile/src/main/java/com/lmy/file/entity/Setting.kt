package com.lmy.file.entity

data class Setting(var sort: Sort = Sort.ASC,
                   var showHide: Boolean = false,
                   var group: Boolean = true) {
    fun copyTo(setting: Setting) {
        setting.sort = sort
        setting.showHide = showHide
        setting.group = group
    }

    enum class Sort {
        ASC, DESC
    }
}