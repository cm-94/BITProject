package com.example.bit.utils

import android.content.Context
import com.example.bithumb.data.remote.BitService
import com.example.bithumb.data.remote.RetrofitClient

object RetrofitUtils {
    fun getBitService(context: Context?): BitService {
        /** Retrofit Client를 생성한 후 BitService를 적용한 후 해당 Service를 return **/
        return RetrofitClient.getClient(context).create(BitService::class.java)
    }
}