package com.lmy.samplenative.adapter

/**
 * Created by aliminabc@gmail.com on 2018/4/19.
 */
abstract class RecyclerViewPoolAdapter<E> : RecyclerAdapter<E,
        RecyclerAdapter.BaseViewHolder<E>>() {
    private var itemType = 0

    override fun getItemViewType(position: Int): Int {
        if (0 == itemType)
            itemType = this::class.java.name.hashCode()
//        Log.i(this::class.java.name, "type = $itemType")
        return itemType
    }
}