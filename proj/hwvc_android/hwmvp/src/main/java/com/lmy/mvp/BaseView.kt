package com.lmy.mvp


interface BaseView<P> {
    val isActive: Boolean
    var presenter: P
    fun showError(error: Int, msg: String)
}