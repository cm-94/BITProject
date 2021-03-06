package com.example.bit.data.remote

import com.example.bit.data.ExchangeData
import com.example.bit.data.RootTickerData
import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Path

interface BitService {
    @GET("ticker/{order_currency}_KRW")
    fun getAllTicker(@Path("order_currency") order:String): Call<HashMap<String, Any>>?

    @GET("ticker/{order_currency}_{payment_currency}")
    fun oneTicker(@Path("order_currency") order:String,@Path("payment_currency") payment:String): Call<RootTickerData>?






    // 환율 데이터 수신 URL
    @GET("exchange/rate/KRW/KRW,USD,JPY.json")
    fun getExchangeRate(): Call<List<ExchangeData>>?

}