package com.example.booksearch.data

import com.google.gson.annotations.SerializedName

/**
 * title        책 제목
 * publisher    출판사
 * author       저자
 * price        가격
 * image        이미지(URL)
 * link         상세정보(URL)
 */
data class BookItem(
    @SerializedName("title")
    val title: String,
    @SerializedName("link")
    val link: String,
    @SerializedName("image")
    val image: String,
    @SerializedName("author")
    val author: String,
    @SerializedName("price")
    val price: String,
    @SerializedName("publisher")
    val publisher: String
    /*,
    @SerializedName("discount")
    val discount : String,
    @SerializedName("pubdate")
    val pubdate : String,
    @SerializedName("isbn")
    val isbn : String,
    @SerializedName("description")
    val description :String*/
) {
    override fun toString(): String {
        return "title: $title, link: $link, image: $image, author: $author, price: $price"
        /*+"discount: $discount, publisher: $publisher, pubdate: $pubdate, isbn: $isbn, description: $description"*/
    }
}

