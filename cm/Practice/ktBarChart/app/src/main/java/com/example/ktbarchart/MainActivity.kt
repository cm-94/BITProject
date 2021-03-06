package com.example.ktbarchart

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.app.AppCompatDelegate
import com.example.ktbarchart.data.ChartData
import com.example.ktbarchart.data.ChartDataSet
import com.example.ktbarchart.view.BongItem
import com.example.ktbarchart.view.ChartLayout

class MainActivity : AppCompatActivity() {
    lateinit var chartLayout : ChartLayout
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
    }

    override fun onResume() {
        super.onResume()
        chartLayout = findViewById(R.id.chart)
        val data1 = ChartData(200,250,260,180)
        val data2 = ChartData(265,265,280,220)
        val data3 = ChartData(220,180,230,120)
        val data4 = ChartData(100,145,160,90)
        val data5 = ChartData(125,150,180,125)
        val data6 = ChartData(120,100,165,100)
        val data7 = ChartData(110,80,130,60)

        val chartDataSet = ChartDataSet(arrayListOf(data1,data2,data3,data4,data5,data6,data7))

        chartLayout.setData(chartDataSet)
    }
}