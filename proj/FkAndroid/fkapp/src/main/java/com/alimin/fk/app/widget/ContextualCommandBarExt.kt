package com.alimin.fk.app.widget

import android.content.ComponentName
import android.content.Intent
import android.content.res.Resources
import android.graphics.drawable.Drawable
import android.view.*
import com.microsoft.fluentui.contextualcommandbar.CommandItemGroup
import com.microsoft.fluentui.contextualcommandbar.ContextualCommandBar
import com.microsoft.fluentui.contextualcommandbar.DefaultCommandItem


fun ContextualCommandBar.setMenu(menuRes: Int) {
    val menu = FkCommandMenu(context.resources)
    val menuInflater = MenuInflater(context)
    menuInflater.inflate(menuRes, menu)
    setItemGroups(menu.asCommandItemGroups())
}

class FkCommandMenu(val resources: Resources) : Menu {
    private var items = ArrayList<FkCommandMenuItem>()

    fun asCommandItemGroups(): ArrayList<CommandItemGroup> {
        val map = HashMap<Int, CommandItemGroup>()
        items.forEach {
            if (!map.containsKey(it.groupId)) {
                map[it.groupId] = CommandItemGroup()
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

class FkCommandMenuItem(
    val gId: Int,
    val id: Int,
    var title: String,
    var contentDescription: String? = null
) : MenuItem {
    private var iconDrawable: Drawable? = null
    private var iconRes: Int = 0

    fun asCommandItem(): DefaultCommandItem {
        return DefaultCommandItem(
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