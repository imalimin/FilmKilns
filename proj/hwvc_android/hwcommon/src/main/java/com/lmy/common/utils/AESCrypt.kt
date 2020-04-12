package com.lmy.common.utils

import android.util.Base64
import java.io.UnsupportedEncodingException
import java.nio.charset.Charset
import java.security.InvalidAlgorithmParameterException
import java.security.InvalidKeyException
import java.security.NoSuchAlgorithmException
import javax.crypto.BadPaddingException
import javax.crypto.Cipher
import javax.crypto.IllegalBlockSizeException
import javax.crypto.NoSuchPaddingException
import javax.crypto.spec.IvParameterSpec
import javax.crypto.spec.SecretKeySpec

object AESCrypt {
    private val AESTYPE = "AES"
    private val CIPHER_ALGORITHM = "AES/CBC/PKCS5Padding"
    private val VIPARA = "1269571569321021"
    private val CHARSET = "utf-8"

    fun encrypt(content: String, password: String): String? {
        try {
            val zeroIv = IvParameterSpec(VIPARA.toByteArray())
            val key = SecretKeySpec(password.toByteArray(), AESTYPE)
            val cipher = Cipher.getInstance(CIPHER_ALGORITHM)
            cipher.init(Cipher.ENCRYPT_MODE, key, zeroIv)
            val encryptedData = cipher.doFinal(content.toByteArray(charset(CHARSET)))
            return String(Base64.encode(encryptedData, Base64.DEFAULT))
        } catch (e: NoSuchAlgorithmException) {
            e.printStackTrace()
        } catch (e: NoSuchPaddingException) {
            e.printStackTrace()
        } catch (e: UnsupportedEncodingException) {
            e.printStackTrace()
        } catch (e: InvalidKeyException) {
            e.printStackTrace()
        } catch (e: IllegalBlockSizeException) {
            e.printStackTrace()
        } catch (e: BadPaddingException) {
            e.printStackTrace()
        } catch (e: InvalidAlgorithmParameterException) {
            // TODO Auto-generated catch block
            e.printStackTrace()
        }

        return null
    }

    fun decrypt(content: String, password: String): String? {
        try {
            val byteMi = Base64.decode(content, Base64.DEFAULT)
            val zeroIv = IvParameterSpec(VIPARA.toByteArray())
            val key = SecretKeySpec(password.toByteArray(), AESTYPE)
            val cipher = Cipher.getInstance(CIPHER_ALGORITHM)
            cipher.init(Cipher.DECRYPT_MODE, key, zeroIv)
            val decryptedData = cipher.doFinal(byteMi)
            return String(decryptedData, Charset.forName(CHARSET))
        } catch (e: NoSuchAlgorithmException) {
            e.printStackTrace()
        } catch (e: NoSuchPaddingException) {
            e.printStackTrace()
        } catch (e: InvalidKeyException) {
            e.printStackTrace()
        } catch (e: IllegalBlockSizeException) {
            e.printStackTrace()
        } catch (e: BadPaddingException) {
            e.printStackTrace()
        } catch (e: UnsupportedEncodingException) {
            e.printStackTrace()
        } catch (e: InvalidAlgorithmParameterException) {
            // TODO Auto-generated catch block
            e.printStackTrace()
        }

        return null
    }
}