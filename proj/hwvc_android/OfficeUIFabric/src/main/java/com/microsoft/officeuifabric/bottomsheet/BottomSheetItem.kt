/*
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */

package com.microsoft.officeuifabric.bottomsheet

import android.os.Parcel
import android.os.Parcelable
import android.support.annotation.DrawableRes

class BottomSheetItem : Parcelable {
    interface OnClickListener {
        fun onBottomSheetItemClick(item: BottomSheetItem)
    }

    val id: Int
    @DrawableRes
    val imageId: Int
    val title: String
    val subtitle: String
    val useDivider: Boolean

    @JvmOverloads
    constructor(
        id: Int = NO_ID,
        @DrawableRes imageId: Int = NO_ID,
        title: String,
        subtitle: String = "",
        useDivider: Boolean = false
    ) {
        this.id = id
        this.imageId = imageId
        this.title = title
        this.subtitle = subtitle
        this.useDivider = useDivider
    }

    private constructor(parcel: Parcel) : this(
        id = parcel.readInt(),
        imageId = parcel.readInt(),
        title = parcel.readString() ?: "",
        subtitle = parcel.readString() ?: "",
        useDivider = parcel.readInt() == 1
    )

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeInt(id)
        parcel.writeInt(imageId)
        parcel.writeString(title)
        parcel.writeString(subtitle)
        parcel.writeInt(if (useDivider) 1 else 0)
    }

    override fun describeContents(): Int = 0

    companion object {
        const val NO_ID = -1

        @JvmField
        val CREATOR = object : Parcelable.Creator<BottomSheetItem> {
            override fun createFromParcel(source: Parcel): BottomSheetItem = BottomSheetItem(source)
            override fun newArray(size: Int): Array<BottomSheetItem?> = arrayOfNulls(size)
        }
    }
}