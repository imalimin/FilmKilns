/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.util

import android.content.Context
import android.support.annotation.DrawableRes
import android.support.v4.content.ContextCompat
import android.view.View
import android.view.ViewGroup
import android.view.inputmethod.InputMethodManager
import android.widget.ImageView

/**
 * Adds a given [view] to a [ViewGroup]. Especially useful when you need a custom view in a control.
 * Use [updateLayout] to update any part of the view's layout before adding it to the [ViewGroup].
 */
fun ViewGroup.setContentAndUpdateVisibility(view: View?, updateLayout: (() -> Unit)? = null) {
    // We need to remove the view each time so that RecyclerViews can properly recycle the view.
    removeAllViews()

    if (view == null) {
        visibility = View.GONE
        return
    }

    // Make sure the custom view isn't already in a ViewGroup.
    // With RecyclerView reusing ViewHolders, it could have a different parent than the current container.
    (view.parent as? ViewGroup)?.removeView(view)

    updateLayout?.invoke()
    addView(view)
    visibility = View.VISIBLE
}

/**
 * Returns an ImageView containing a Drawable.
 * @param imageId a Drawable resource id.
 */
fun Context.createImageView(@DrawableRes imageId: Int): ImageView {
    val drawable = ContextCompat.getDrawable(this, imageId)
    val imageView = ImageView(this)
    imageView.setImageDrawable(drawable)
    return imageView
}

/**
 * Sets a view's visibility based on a boolean [isVisible].
 */
var View.isVisible: Boolean
    get() = visibility == View.VISIBLE
    set(value) {
        visibility = if (value) View.VISIBLE else View.GONE
    }

/**
 * Shows the soft keyboard.
 */
fun View.showKeyboard() {
    val imm = context.getSystemService(Context.INPUT_METHOD_SERVICE) as InputMethodManager
    imm.toggleSoftInput(InputMethodManager.SHOW_IMPLICIT, InputMethodManager.HIDE_IMPLICIT_ONLY)
}