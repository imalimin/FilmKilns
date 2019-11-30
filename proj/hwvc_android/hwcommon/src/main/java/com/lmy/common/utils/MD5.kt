package com.lmy.common.utils

import java.security.MessageDigest
import java.security.NoSuchAlgorithmException

object MD5 {
    // 全局数组
    private val strDigits = arrayOf("0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f")

    // 返回形式为数字跟字符串
    private fun byteToArrayString(bByte: Byte): String {
        var iRet = bByte.toInt()
        // System.out.println("iRet="+iRet);
        if (iRet < 0) {
            iRet += 256
        }
        val iD1 = iRet / 16
        val iD2 = iRet % 16
        return strDigits[iD1] + strDigits[iD2]
    }

    // 返回形式只为数字
    private fun byteToNum(bByte: Byte): String {
        var iRet = bByte.toInt()
        println("iRet1=$iRet")
        if (iRet < 0) {
            iRet += 256
        }
        return iRet.toString()
    }

    // 转换字节数组为16进制字串
    private fun byteToString(bByte: ByteArray): String {
        val sBuffer = StringBuffer()
        for (i in bByte.indices) {
            sBuffer.append(byteToArrayString(bByte[i]))
        }
        return sBuffer.toString()
    }

    fun getMD5Code(strObj: String): String? {
        try {
            val md = MessageDigest.getInstance("MD5")
            // md.digest() 该函数返回值为存放哈希值结果的byte数组
            return byteToString(md.digest(strObj.toByteArray()))
        } catch (ex: NoSuchAlgorithmException) {
            ex.printStackTrace()
        }

        return null
    }
}