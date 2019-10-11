package com.lmy.hwvcnative

import org.junit.Assert.assertEquals
import org.junit.Test
import java.io.FileInputStream
import java.io.FileWriter
import java.util.*

/**
 * Example local unit test, which will execute on the development machine (host).
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
class ExampleUnitTest {
    @Test
    fun addition_isCorrect() {
        assertEquals(4, 2 + 2)
    }

    @Test
    fun png2Base64String() {
        val pngPath = "/Users/lmy/Filter_05/hs_abaose/hs_abaose.png"
        val encoder = Base64.getEncoder()
        val ins = FileInputStream(pngPath)
        val result = encoder.encodeToString(ins.readBytes())
        val writer = FileWriter("$pngPath.txt")
        writer.write(result)
        writer.close()
        ins.close()
    }
}
