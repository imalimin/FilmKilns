package com.alimin.pic.widget

import android.content.ComponentName
import android.content.Context
import android.content.Intent
import android.content.res.Resources
import android.graphics.Bitmap
import android.graphics.drawable.Drawable
import android.util.AttributeSet
import android.view.*
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import androidx.annotation.Dimension
import androidx.annotation.DrawableRes
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.alimin.pic.R
import com.microsoft.fluentui.contextualcommandbar.CommandItem
import com.microsoft.fluentui.contextualcommandbar.CommandItemGroup
import com.microsoft.fluentui.contextualcommandbar.DefaultCommandItem
import com.microsoft.fluentui.theming.FluentUIContextThemeWrapper
import com.microsoft.fluentui.util.isVisible

open class FkContextualCommandBar @JvmOverloads constructor(
    appContext: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : FrameLayout(FluentUIContextThemeWrapper(appContext,R.style.Theme_FluentUI_ContextualCommandBar), attrs, defStyleAttr) {

    interface OnItemLongClickListener {
        fun onItemLongClick(item: CommandItem, view: View)
    }

    private var dismissButtonContainer: ViewGroup? = null
    private var commandItemAdapter: CommandItemAdapter
    private var commandItemRecyclerView: RecyclerView

    var dismissCommandItem: DismissCommandItem? = null
        set(value) {
            field = value

            updateDismissButton()
        }

    var orientation: Int = LinearLayout.HORIZONTAL
        set(value) {
            field = value
            commandItemAdapter.orientation = value
            commandItemRecyclerView.apply {
                layoutManager = LinearLayoutManager(
                    context,
                    value,
                    false
                )
            }
        }

    init {
        var groupSpace = resources.getDimensionPixelSize(R.dimen.fluentui_contextual_command_bar_default_group_space)
        var itemSpace = resources.getDimensionPixelSize(R.dimen.fluentui_contextual_command_bar_default_item_space)
        attrs?.let {
            val styledAttributes = appContext.theme.obtainStyledAttributes(
                it,
                R.styleable.ContextualCommandBar,
                0,
                0
            )

            try {
                groupSpace = styledAttributes.getDimensionPixelSize(
                    R.styleable.ContextualCommandBar_fluentui_groupSpace,
                    resources.getDimensionPixelSize(R.dimen.fluentui_contextual_command_bar_default_group_space)
                )
                itemSpace = styledAttributes.getDimensionPixelSize(
                    R.styleable.ContextualCommandBar_fluentui_itemSpace,
                    resources.getDimensionPixelSize(R.dimen.fluentui_contextual_command_bar_default_item_space)
                )
            } finally {
                styledAttributes.recycle()
            }
        }

        commandItemAdapter = CommandItemAdapter(
            CommandItemAdapter.CommandListOptions(groupSpace, itemSpace, orientation)
        )
        commandItemRecyclerView = RecyclerView(
            context
        ).apply {
            layoutManager = LinearLayoutManager(
                context,
                LinearLayoutManager.HORIZONTAL,
                false
            )
            adapter = commandItemAdapter
            overScrollMode = View.OVER_SCROLL_NEVER
        }
        addView(commandItemRecyclerView)
    }

    fun setItemOnClickListener(listener: CommandItem.OnItemClickListener) {
        commandItemAdapter.itemClickListener = listener
    }

    fun setOnItemLongClickListener(listener: OnItemLongClickListener) {
        commandItemAdapter.itemLongClickListener = listener
    }

    fun setItemGroups(itemGroups: ArrayList<CommandItemGroup>) {
        commandItemAdapter.commandItemGroups = itemGroups
        commandItemAdapter.notifyDataSetChanged()
    }

    fun addItemGroup(itemGroup: CommandItemGroup) {
        commandItemAdapter.addItemGroup(itemGroup)
        commandItemAdapter.notifyDataSetChanged()
    }

    fun removeItemGroup(filter: (it: CommandItemGroup) -> Boolean) {
        val items = arrayListOf<CommandItemGroup>()
        commandItemAdapter.commandItemGroups.forEach {
            if (filter(it)) {
                items.add(it)
            }
        }
        commandItemAdapter.commandItemGroups.removeAll(items)
        commandItemAdapter.notifyDataSetChanged()
    }

    fun setItemSelected(filter: (it: DefaultCommandItem) -> Boolean) {
        commandItemAdapter.commandItemGroups.forEach { group ->
            group.items.forEach {
                if (it is DefaultCommandItem) {
                    val select = filter(it)
                    it.setSelected(select)
                }
            }
        }
        commandItemAdapter.notifyDataSetChanged()
    }

    /**
     * Set the space between each [CommandItemGroup]
     */
    fun setCommandGroupSpace(@Dimension(unit = Dimension.PX) space: Int) {
        commandItemAdapter.setGroupSpace(space)
        commandItemAdapter.notifyDataSetChanged()
    }

    /**
     * Set the space between each [CommandItem]
     */
    fun setCommandItemSpace(@Dimension(unit = Dimension.PX) space: Int) {
        commandItemAdapter.setItemSpace(space)
        commandItemAdapter.notifyDataSetChanged()
    }

    /**
     * Set dismiss button position, see [DismissItemPosition]
     */
    fun setDismissButtonPosition(position: DismissItemPosition) {
        dismissCommandItem?.position = position
        updateDismissButton()
    }

    /**
     * Notify any registered observers that the data set has changed, including
     * Command item list and dismiss button
     */
    fun notifyDataSetChanged() {
        commandItemAdapter.notifyDataSetChanged()
        updateDismissButton()
    }

    private fun updateDismissButton() {
        dismissCommandItem ?: return
        val icon = dismissCommandItem!!.getIcon()
        if (icon == 0) {
            return
        }
        val dismissItemVisible = dismissCommandItem!!.visible
        val dismissItemGravity = dismissCommandItem!!.position

        if (dismissButtonContainer == null) {
            dismissButtonContainer = LayoutInflater.from(context)
                .inflate(R.layout.view_dismiss_command_item, null) as ViewGroup
            addView(dismissButtonContainer)
        }
        val dismissButton: ImageView = dismissButtonContainer!!.findViewById(R.id.dismiss_command_item_button)
        val dismissButtonDivider: View = dismissButtonContainer!!.findViewById(R.id.dismiss_command_item_divider)
        (dismissButtonContainer!!.layoutParams as LayoutParams).apply {
            height = ViewGroup.LayoutParams.MATCH_PARENT
            width = ViewGroup.LayoutParams.WRAP_CONTENT

            gravity = when (dismissCommandItem!!.position) {
                DismissItemPosition.START -> Gravity.START
                DismissItemPosition.END -> Gravity.END
            }
        }

        // Set the position of DismissItemPosition
        if (dismissItemGravity == DismissItemPosition.START) {
            dismissButtonDivider.setBackgroundResource(
                R.drawable.contextual_command_bar_dismiss_button_divider_start_background
            )
            dismissButtonContainer!!.removeAllViews()
            dismissButtonContainer!!.addView(dismissButton)
            dismissButtonContainer!!.addView(dismissButtonDivider)
        } else if (dismissItemGravity == DismissItemPosition.END) {
            dismissButtonDivider.setBackgroundResource(
                R.drawable.contextual_command_bar_dismiss_button_divider_end_background
            )
            dismissButtonContainer!!.removeAllViews()
            dismissButtonContainer!!.addView(dismissButtonDivider)
            dismissButtonContainer!!.addView(dismissButton)
        }

        dismissButton.setImageResource(icon)
        dismissButton.contentDescription = dismissCommandItem!!.getContentDescription()
        dismissButtonContainer!!.isVisible = dismissItemVisible
        dismissButtonContainer!!.setOnClickListener {
            dismissCommandItem!!.dismissListener?.invoke()
        }

        // Adjust RecyclerView's position to adapt dismiss button
        val dismissButtonPlaceholder = if (dismissItemVisible) {
            resources.getDimensionPixelSize(
                R.dimen.fluentui_contextual_command_bar_dismiss_button_width
            ) + resources.getDimensionPixelSize(
                R.dimen.fluentui_contextual_command_bar_dismiss_gap_width
            )
        } else 0
        commandItemRecyclerView.setPaddingRelative(
            if (dismissItemGravity == DismissItemPosition.START) dismissButtonPlaceholder else 0,
            0,
            if (dismissItemGravity == DismissItemPosition.END) dismissButtonPlaceholder else 0,
            0)
        commandItemRecyclerView.clipToPadding = false

        // Bring this dismiss button on the front of command list
        bringChildToFront(dismissButtonContainer)
    }

    class DismissCommandItem(
        @DrawableRes private var icon: Int = 0,
        private var contentDescription: String? = null,
        var visible: Boolean = true,
        var position: DismissItemPosition = DismissItemPosition.END,
        var dismissListener: (() -> Unit)? = null
    ) : CommandItem {

        override fun getIcon(): Int {
            return icon
        }

        override fun getContentDescription(): String? {
            return contentDescription
        }
    }

    enum class DismissItemPosition {
        START, END
    }

    fun setMenu(menuRes: Int) {
        val menu = FkCommandMenu(context.resources)
        val menuInflater = MenuInflater(context)
        menuInflater.inflate(menuRes, menu)
        setItemGroups(menu.asCommandItemGroups())
    }
}

internal class CommandItemAdapter(
    private var options: CommandListOptions
) : RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    private var flattenCommandItems = arrayListOf<CommandItem>()
    var commandItemGroups = arrayListOf<CommandItemGroup>()
        set(value) {
            field = value

            flatItemGroup()
        }

    var itemClickListener: CommandItem.OnItemClickListener? = null
    var itemLongClickListener: FkContextualCommandBar.OnItemLongClickListener? = null

    var orientation: Int = LinearLayout.HORIZONTAL
        set(value) {
            field = value
            options.orientation = value
        }

    fun addItemGroup(itemGroup: CommandItemGroup) {
        commandItemGroups.add(itemGroup)

        flatItemGroup()
    }

    fun setGroupSpace(space: Int) {
        options.groupSpace = space
    }

    fun setItemSpace(space: Int) {
        options.itemSpace = space
    }

    private fun flatItemGroup() {
        flattenCommandItems.clear()
        commandItemGroups.forEach {
            flattenCommandItems.addAll(it.items)
        }
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        return ViewHolder(LayoutInflater
            .from(parent.context)
            .inflate(R.layout.view_command_item, parent, false))
    }

    override fun onBindViewHolder(vh: RecyclerView.ViewHolder, position: Int) {
        val viewType = getItemViewType(position)
        val viewHolder = vh as ViewHolder
        val commandItem = flattenCommandItems[position]
        val isItemSelected = commandItem.isSelected()
        val isItemEnabled = commandItem.isEnabled()
        val label = commandItem.getLabel()
        val icon = commandItem.getIcon()
        val bitmapIcon = commandItem.getBitmapIcon()
        val description = commandItem.getContentDescription()
        if (icon != 0) {
            // Using icon as primary display content
            viewHolder.label.isVisible = false
            with(viewHolder.icon) {
                isVisible = true
                setImageResource(icon)
                contentDescription = description
                isEnabled = isItemEnabled
                isSelected = isItemSelected
            }

        } else if (bitmapIcon != null) {
            viewHolder.label.isVisible = false
            with(viewHolder.icon) {
                isVisible = true
                setImageBitmap(bitmapIcon)
                contentDescription = description
                isEnabled = isItemEnabled
                isSelected = isItemSelected
            }
        } else if (!label.isNullOrEmpty()) {
            viewHolder.icon.isVisible = false
            with(viewHolder.label) {
                isVisible = true
                text = label
                contentDescription = description
                isEnabled = isItemEnabled
                isSelected = isItemSelected
            }
        } else {
            // Return if no icon and label
            return
        }

        // Update the UI of command item
        with(viewHolder.itemView) {
            isEnabled = isItemEnabled
            isSelected = isItemSelected
            when (viewType) {
                VIEW_TYPE_GROUP_CENTER_ITEM -> {
                    (layoutParams as RecyclerView.LayoutParams).apply {
                        marginEnd = options.itemSpace
                    }

                    background = ContextCompat.getDrawable(
                        context,
                        R.drawable.contextual_command_bar_center_item_background
                    )
                }

                VIEW_TYPE_GROUP_START_ITEM -> {
                    (layoutParams as RecyclerView.LayoutParams).apply {
                        marginEnd = options.itemSpace
                    }
                    background = ContextCompat.getDrawable(
                        context,
                        R.drawable.contextual_command_bar_start_item_background
                    )
                }

                VIEW_TYPE_GROUP_END_ITEM -> {
                    (layoutParams as RecyclerView.LayoutParams).apply {
                        marginEnd = if (position == flattenCommandItems.size - 1) 0
                        else options.groupSpace
                    }

                    background = ContextCompat.getDrawable(
                        context,
                        R.drawable.contextual_command_bar_end_item_background
                    )
                }

                VIEW_TYPE_GROUP_SINGLE_ITEM -> {
                    (layoutParams as RecyclerView.LayoutParams).apply {
                        if (options.orientation == LinearLayout.VERTICAL) {
                            bottomMargin = if (position == flattenCommandItems.size - 1) 0
                            else options.groupSpace / 2
                        } else {
                            marginEnd = if (position == flattenCommandItems.size - 1) 0
                            else options.groupSpace
                        }
                    }
                    background = ContextCompat.getDrawable(
                        context,
                        R.drawable.contextual_command_bar_single_item_background
                    )
                }
            }
            setOnClickListener(object : View.OnClickListener {
                override fun onClick(v: View?) {
                    itemClickListener?.onItemClick(commandItem, viewHolder.itemView)
                }
            })
            setOnLongClickListener(object : View.OnLongClickListener {
                override fun onLongClick(v: View?): Boolean {
                    itemLongClickListener?.onItemLongClick(commandItem, viewHolder.itemView)
                    return true
                }
            })
        }
    }

    override fun getItemViewType(position: Int): Int {
        if (flattenCommandItems.size == 0) {
            return super.getItemViewType(position)
        }
        var pendingSearch = position + 1
        for (group in commandItemGroups) {
            val itemsSize = group.items.size
            if (itemsSize == 0) {
                continue
            }

            if (pendingSearch > itemsSize) {
                pendingSearch -= itemsSize
                continue
            } else {
                return when {
                    itemsSize == 1 -> {
                        VIEW_TYPE_GROUP_SINGLE_ITEM
                    }
                    pendingSearch == 1 -> {
                        VIEW_TYPE_GROUP_START_ITEM
                    }
                    pendingSearch == itemsSize -> {
                        VIEW_TYPE_GROUP_END_ITEM
                    }
                    else -> {
                        VIEW_TYPE_GROUP_CENTER_ITEM
                    }
                }
            }
        }

        return super.getItemViewType(position)
    }

    override fun getItemCount(): Int = flattenCommandItems.size

    private class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val icon: ImageView = itemView.findViewById(R.id.contextual_command_item_icon)
        val label: TextView = itemView.findViewById(R.id.contextual_command_item_label)
        init {
            itemView.layoutParams = RecyclerView.LayoutParams(
                RecyclerView.LayoutParams.WRAP_CONTENT,
                RecyclerView.LayoutParams.WRAP_CONTENT
            )
        }
    }

    data class CommandListOptions(
        var groupSpace: Int,
        var itemSpace: Int,
        var orientation: Int
    )

    companion object {
        const val VIEW_TYPE_GROUP_CENTER_ITEM = 0
        const val VIEW_TYPE_GROUP_START_ITEM = 1
        const val VIEW_TYPE_GROUP_END_ITEM = 2
        const val VIEW_TYPE_GROUP_SINGLE_ITEM = 3
    }
}

open class LayerCommandItem(
    var layerId: Int = -1,
    var id: Int = 0,
    var gId: Int = 0,
    @DrawableRes icon: Int = 0,
    label: String? = null,
    contentDescription: String? = null,
    enabled: Boolean = true,
    selected: Boolean = false,
    bitmap: Bitmap? = null,
) : DefaultCommandItem(icon, label, contentDescription, enabled, selected, bitmap) {
    override fun getId(): Int? {
        return id
    }

    fun getGroupId(): Int {
        return gId
    }
}

internal class FkCommandMenu(val resources: Resources) : Menu {
    private var items = ArrayList<FkCommandMenuItem>()

    fun asCommandItemGroups(): ArrayList<CommandItemGroup> {
        val map = HashMap<Int, CommandItemGroup>()
        items.forEach {
            if (!map.containsKey(it.groupId)) {
                map[it.groupId] = CommandItemGroup()
                map[it.groupId]?.id = it.groupId.toString()
            }
            map[it.groupId]?.addItem(it.asCommandItem())
        }
        return ArrayList(map.values)
    }

    override fun add(title: CharSequence?): MenuItem {
        val item = FkCommandMenuItem(
            0,
            0,
            title?.toString() ?: ""
        )
        items.add(item)
        return item
    }

    override fun add(titleRes: Int): MenuItem {
        val item = FkCommandMenuItem(
            0,
            0,
            resources.getString(titleRes)
        )
        items.add(item)
        return item
    }

    override fun add(groupId: Int, itemId: Int, order: Int, title: CharSequence?): MenuItem {
        val item = FkCommandMenuItem(
            groupId,
            itemId,
            title?.toString() ?: ""
        )
        items.add(item)
        return item
    }

    override fun add(groupId: Int, itemId: Int, order: Int, titleRes: Int): MenuItem {
        val item = FkCommandMenuItem(
            groupId,
            itemId,
            resources.getString(titleRes)
        )
        items.add(item)
        return item
    }

    override fun addSubMenu(title: CharSequence?): SubMenu? {
        return null
    }

    override fun addSubMenu(titleRes: Int): SubMenu? {
        return null
    }

    override fun addSubMenu(groupId: Int, itemId: Int, order: Int, title: CharSequence?): SubMenu? {
        return null
    }

    override fun addSubMenu(groupId: Int, itemId: Int, order: Int, titleRes: Int): SubMenu? {
        return null
    }

    override fun addIntentOptions(
        groupId: Int,
        itemId: Int,
        order: Int,
        caller: ComponentName?,
        specifics: Array<out Intent>?,
        intent: Intent?,
        flags: Int,
        outSpecificItems: Array<out MenuItem>?
    ): Int {
        TODO("Not yet implemented")
    }

    override fun removeItem(id: Int) {
    }

    override fun removeGroup(groupId: Int) {
    }

    override fun clear() {
    }

    override fun setGroupCheckable(group: Int, checkable: Boolean, exclusive: Boolean) {
    }

    override fun setGroupVisible(group: Int, visible: Boolean) {
    }

    override fun setGroupEnabled(group: Int, enabled: Boolean) {
    }

    override fun hasVisibleItems(): Boolean {
        return true
    }

    override fun findItem(id: Int): MenuItem? {
        return null
    }

    override fun size(): Int {
        return items.size
    }

    override fun getItem(index: Int): MenuItem? {
        return null
    }

    override fun close() {
    }

    override fun performShortcut(keyCode: Int, event: KeyEvent?, flags: Int): Boolean {
        return false
    }

    override fun isShortcutKey(keyCode: Int, event: KeyEvent?): Boolean {
        return false
    }

    override fun performIdentifierAction(id: Int, flags: Int): Boolean {
        return false
    }

    override fun setQwertyMode(isQwerty: Boolean) {
    }
}

internal class FkCommandMenuItem(
    val gId: Int,
    val id: Int,
    var title: String,
    var contentDescription: String? = null
) : MenuItem {
    private var iconDrawable: Drawable? = null
    private var iconRes: Int = 0

    fun asCommandItem(): LayerCommandItem {
        return LayerCommandItem(
            layerId = 0,
            id = id,
            gId = gId,
            iconRes,
            label = title,
            contentDescription = contentDescription
        )
    }

    override fun getItemId(): Int {
        return id
    }

    override fun getGroupId(): Int {
        return gId
    }

    override fun getOrder(): Int {
        return 0
    }

    override fun setTitle(title: CharSequence): MenuItem {
        this.title = title.toString()
        return this
    }

    override fun setTitle(title: Int): MenuItem {
        return this
    }

    override fun getTitle(): CharSequence {
        return title
    }

    override fun setTitleCondensed(title: CharSequence?): MenuItem {
        return this
    }

    override fun getTitleCondensed(): CharSequence {
        TODO("Not yet implemented")
    }

    override fun setIcon(icon: Drawable): MenuItem {
        this.iconDrawable = icon
        return this
    }

    override fun setIcon(iconRes: Int): MenuItem {
        this.iconRes = iconRes
        return this
    }

    override fun getIcon(): Drawable {
        return iconDrawable!!
    }

    override fun setIntent(intent: Intent?): MenuItem {
        return this
    }

    override fun getIntent(): Intent {
        TODO("Not yet implemented")
    }

    override fun setShortcut(numericChar: Char, alphaChar: Char): MenuItem {
        return this
    }

    override fun setNumericShortcut(numericChar: Char): MenuItem {
        return this
    }

    override fun getNumericShortcut(): Char {
        TODO("Not yet implemented")
    }

    override fun setAlphabeticShortcut(alphaChar: Char): MenuItem {
        return this
    }

    override fun getAlphabeticShortcut(): Char {
        TODO("Not yet implemented")
    }

    override fun setCheckable(checkable: Boolean): MenuItem {
        return this
    }

    override fun isCheckable(): Boolean {
        TODO("Not yet implemented")
    }

    override fun setChecked(checked: Boolean): MenuItem {
        return this
    }

    override fun isChecked(): Boolean {
        TODO("Not yet implemented")
    }

    override fun setVisible(visible: Boolean): MenuItem {
        return this
    }

    override fun isVisible(): Boolean {
        TODO("Not yet implemented")
    }

    override fun setEnabled(enabled: Boolean): MenuItem {
        return this
    }

    override fun isEnabled(): Boolean {
        TODO("Not yet implemented")
    }

    override fun hasSubMenu(): Boolean {
        TODO("Not yet implemented")
    }

    override fun getSubMenu(): SubMenu {
        TODO("Not yet implemented")
    }

    override fun setOnMenuItemClickListener(menuItemClickListener: MenuItem.OnMenuItemClickListener?): MenuItem {
        return this
    }

    override fun getMenuInfo(): ContextMenu.ContextMenuInfo {
        TODO("Not yet implemented")
    }

    override fun setShowAsAction(actionEnum: Int) {
        TODO("Not yet implemented")
    }

    override fun setShowAsActionFlags(actionEnum: Int): MenuItem {
        return this
    }

    override fun setActionView(view: View?): MenuItem {
        return this
    }

    override fun setActionView(resId: Int): MenuItem {
        return this
    }

    override fun getActionView(): View {
        TODO("Not yet implemented")
    }

    override fun setActionProvider(actionProvider: ActionProvider?): MenuItem {
        return this
    }

    override fun getActionProvider(): ActionProvider {
        TODO("Not yet implemented")
    }

    override fun expandActionView(): Boolean {
        TODO("Not yet implemented")
    }

    override fun collapseActionView(): Boolean {
        TODO("Not yet implemented")
    }

    override fun isActionViewExpanded(): Boolean {
        TODO("Not yet implemented")
    }

    override fun setOnActionExpandListener(listener: MenuItem.OnActionExpandListener?): MenuItem {
        return this
    }

}