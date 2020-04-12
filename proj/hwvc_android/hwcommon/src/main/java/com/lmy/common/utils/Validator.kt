package com.lmy.common.utils

import android.text.TextUtils
import java.util.regex.Pattern

object Validator {
    /**
     * 正则表达式：验证用户名
     */
    val REGEX_USERNAME = "^[a-zA-Z]\\w{5,17}$"

    /**
     * 正则表达式：验证密码
     */
    val REGEX_PASSWORD = "^[a-zA-Z0-9]{6,16}$"

    /**
     * 正则表达式：验证手机号
     */
    val REGEX_MOBILE = "^((13[0-9])|(15[0-9])|(18[0-9])|(17[0-9]))\\d{8}$"

    /**
     * 正则表达式：验证邮箱
     */
    val REGEX_EMAIL = "^[A-Za-z0-9][\\w\\-\\.]{3,12}@([\\w\\-]+\\.)+[\\w]{2,3}$"

    /**
     * 正则表达式：验证汉字
     */
    val REGEX_CHINESE = "^[\u4e00-\u9fa5],{0,}$"

    /**
     * 正则表达式：验证身份证
     */
    val REGEX_ID_CARD = "(^\\d{18}$)|(^\\d{15}$)"
//    public static final String REGEX_ID_CARD = "/^(\\d{15}$|^\\d{18}$|^\\d{17}(\\d|X|x))$/";

    /**
     * 正则表达式：验证URL
     */
    val REGEX_URL = "http(s)?://([\\w-]+\\.)+[\\w-]+(/[\\w- ./?%&=]*)?"

    /**
     * 正则表达式：验证IP地址
     */
    val REGEX_IP_ADDR = "(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)"

    /**
     * 校验用户名
     *
     * @param username
     * @return 校验通过返回true，否则返回false
     */
    fun isUsername(username: String): Boolean {
        return Pattern.matches(REGEX_USERNAME, username)
    }

    /**
     * 校验密码
     *
     * @param password
     * @return 校验通过返回true，否则返回false
     */
    fun isPassword(password: String): Boolean {
        return Pattern.matches(REGEX_PASSWORD, password)
    }

    /**
     * 校验手机号
     *
     * @param mobile
     * @return 校验通过返回true，否则返回false
     */
    fun isMobile(mobile: String): Boolean {
        return Pattern.matches(REGEX_MOBILE, mobile)
    }

    /**
     * 校验邮箱
     *
     * @param email
     * @return 校验通过返回true，否则返回false
     */
    fun isEmail(email: String): Boolean {
        return Pattern.matches(REGEX_EMAIL, email)
    }

    /**
     * 校验汉字
     *
     * @param chinese
     * @return 校验通过返回true，否则返回false
     */
    fun isChinese(chinese: String): Boolean {
        return Pattern.matches(REGEX_CHINESE, chinese)
    }

    /**
     * 校验身份证
     *
     * @param idCard
     * @return 校验通过返回true，否则返回false
     */
    fun isIDCard(idCard: String): Boolean {
        return Pattern.matches(REGEX_ID_CARD, idCard)
    }

    /**
     * 校验URL
     *
     * @param url
     * @return 校验通过返回true，否则返回false
     */
    fun isUrl(url: String): Boolean {
        return Pattern.matches(REGEX_URL, url)
    }

    /**
     * 校验IP地址
     *
     * @param ipAddr
     * @return
     */
    fun isIPAddr(ipAddr: String): Boolean {
        return Pattern.matches(REGEX_IP_ADDR, ipAddr)
    }

    fun isName(s: String): Boolean {
        return !TextUtils.isEmpty(s) && s.length >= 2 && s.length <= 16
    }
}