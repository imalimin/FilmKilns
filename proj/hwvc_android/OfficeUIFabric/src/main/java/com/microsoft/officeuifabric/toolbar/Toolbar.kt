/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.toolbar

import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.drawable.BitmapDrawable
import android.support.v7.widget.Toolbar
import android.util.AttributeSet
import android.view.ViewGroup
import com.microsoft.officeuifabric.R
import com.microsoft.officeuifabric.persona.AvatarSize
import com.microsoft.officeuifabric.persona.AvatarView
import com.microsoft.officeuifabric.persona.IAvatar
import com.microsoft.officeuifabric.persona.setAvatar
import com.microsoft.officeuifabric.theming.UIFabricContextThemeWrapper

/**
 * [Toolbar] appears at the top of the activity and can display the [avatarView], title,
 * and action items.
 */
class Toolbar : Toolbar {
    companion object {
        private val AVATAR_SIZE = AvatarSize.MEDIUM
    }

    /**
     * [avatarView] is used as the navigation icon and appears at the start of the [Toolbar].
     */
    var avatar: IAvatar? = null
        set(value) {
            if (field == value)
                return
            field = value
            updateStylesAndIcon()
        }

    @JvmOverloads
    constructor(context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0) : super(UIFabricContextThemeWrapper(context), attrs, R.attr.toolbarStyle) {
        // minHeight can't be set in theme or it will also set title height. Having minHeight helps center option menu icons.
        minimumHeight = context.resources.getDimension(R.dimen.uifabric_toolbar_min_height).toInt()
        updateStylesAndIcon()
    }

    private fun updateStylesAndIcon() {
        if (avatar == null) {
            setPaddingRelative(context.resources.getDimension(R.dimen.uifabric_toolbar_padding_start).toInt(), 0, 0, 0)
            titleMarginStart = context.resources.getDimension(R.dimen.uifabric_toolbar_title_margin_start).toInt()
            navigationIcon = null
            return
        }

        setPaddingRelative(context.resources.getDimension(R.dimen.uifabric_toolbar_padding_start_with_avatar).toInt(), 0, 0, 0)
        titleMarginStart = context.resources.getDimension(R.dimen.uifabric_toolbar_title_margin_start_with_avatar).toInt()
        navigationIcon = getAvatarBitmapDrawable()
    }

    /**
     * Draw the [avatarView]'s initials or bitmap onto a canvas that's used to create a [BitmapDrawable].
     */
    private fun getAvatarBitmapDrawable(): BitmapDrawable? {
        val avatar = avatar ?: return null
        val avatarView = AvatarView(context)
        avatarView.setAvatar(avatar)
        avatarView.avatarSize = AVATAR_SIZE

        val avatarDisplayValue = avatarView.avatarSize.getDisplayValue(context)
        val avatarBitmap = Bitmap.createBitmap(avatarDisplayValue, avatarDisplayValue, Bitmap.Config.ARGB_8888)
        val canvas = Canvas(avatarBitmap)

        avatarView.layoutParams = ViewGroup.LayoutParams(avatarDisplayValue, avatarDisplayValue)
        avatarView.layout(0, 0, avatarDisplayValue, avatarDisplayValue)
        avatarView.draw(canvas)

        return BitmapDrawable(resources, avatarBitmap)
    }
}