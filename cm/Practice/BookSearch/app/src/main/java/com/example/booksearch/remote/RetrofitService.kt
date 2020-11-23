package com.example.booksearch.remote

import okhttp3.ResponseBody
import retrofit2.Call
import retrofit2.http.GET
import retrofit2.http.Query

interface RetrofitService {
//    @GET("book")
//    fun searchBookItem(@Query("query") query : String,
//                       /*@Query("start") start : Int,
//                       @Query("display") display : Int*/): Call<ResponseBody>
//    @Header({
//        "X-Naver-Client-Id: C5lgKUkRqRLJy79ksMpE",
//        "X-Naver-Client-Secret: hA9aUSVBAB"
//    })
    @GET("book.json")
    fun searchBookItem(@Query("query") query: String,
                       @Query("start") start: Int,
                       @Query("display") display: Int): Call<ResponseBody>
}