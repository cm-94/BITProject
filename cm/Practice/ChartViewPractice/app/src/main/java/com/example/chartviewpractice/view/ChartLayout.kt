package com.example.chartpractice.view

import android.annotation.SuppressLint
import android.content.Context
import android.util.AttributeSet
import android.util.Log
import android.view.GestureDetector
import android.view.LayoutInflater
import android.view.MotionEvent
import android.view.View
import android.widget.RelativeLayout
import android.widget.TextView
import com.example.chartviewpractice.R
import com.example.ktbarchart.data.ChartData
import com.example.ktbarchart.data.ChartDataSet
import kotlin.math.absoluteValue


class ChartLayout : RelativeLayout, GestureDetector.OnGestureListener{
    /** data **/
    private var dataCnt = 5
    var startCnt = 0

    private lateinit var dataSet: ChartDataSet
    private lateinit var chartLayout : BongChartLayout
    private lateinit var xAxis : TextView
    private lateinit var yAxis : TextView


    private var startX : Float = 0F
    private var startY : Float = 0F
    private var moveX : Float = 0F

    constructor(context: Context?) : super(context){
        if (context != null){
            init()
        }
    }

    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs){
        if (context != null){
            init()
        }
    }

    override fun onLayout(changed: Boolean, l: Int, t: Int, r: Int, b: Int) {
        super.onLayout(changed, l, t, r, b)
        if(changed){
//            chartLayout.setData(dataSet)
        }
    }

//    override fun onTouchEvent(event: MotionEvent?): Boolean {
//        Log.d("onTouch_ChartLayout","event: " + event.toString())
//        return super.onTouchEvent(event)
//    }

    @SuppressLint("ClickableViewAccessibility")
    fun init() {
        val v: View = LayoutInflater.from(context).inflate(R.layout.chart_layout, this, true)
        chartLayout = v.findViewById(R.id.bongChartLayout)
//        chartLayout.setOnTouchListener(this)
    }

    fun setData(chartDataSet: ChartDataSet){
        // 전체 데이터 세팅
        dataSet = chartDataSet
        // 차트에 표시할 데이터 세팅
        setChartData(CHART_MOVE_DIRECTION_DEFAULT)
//        invalidate()
    }

    /**
     * 전체 데이터 중 차트에 표시할 데이터를
     * 차트 레이아웃에 넘겨준다
     */
    fun setChartData(direction : Int){
        if(this.dataSet.data.size == 0){
            return
        }
        // 시작 데이터 잡기

        var chartData = ArrayList<ChartData>()

        if(direction == CHART_MOVE_DIRECTION_DEFAULT){
            if(dataSet.data.size > 5){
                startCnt = dataSet.data.size - dataCnt
            }else{
                startCnt = 0
            }
        } else if(direction == CHART_MOVE_DIRECTION_POSITIVE){
            startCnt ++
            if(startCnt > dataSet.data.size - dataCnt){
                startCnt = dataSet.data.size - dataCnt
            }
        } else if(direction == CHART_MOVE_DIRECTION_NAGATIVE){
            startCnt --
            if(startCnt < 0){
                startCnt = 0
            }
        }
        for(i in startCnt until startCnt + dataCnt){
            chartData.add(dataSet.data[i])
        }
        chartLayout.setData(ChartDataSet(chartData))
        // 시작위치 & 이동거리 초기화
        moveX = 0F
    }

    fun setXAxisTitle(title: String){
        xAxis.text = title
    }
    fun setYAxisTitle(title: String){
        yAxis.text = title
    }

//    override fun onTouch(v: View, event: MotionEvent): Boolean {
//        Log.d("ChartLayout_onTouch","event: " + event.toString())
//        if(v.id == chartLayout.id){
//            if(event.action == MotionEvent.ACTION_DOWN){
//                startX = event.x
//            } else if(event.action == MotionEvent.ACTION_MOVE){
//                if(startX == 0F){
//                    startX = event.x
//                }else{
//                    moveX = startX - event.x
//                }
//            } else if(event.action == MotionEvent.ACTION_UP){
//                initializeTouchPoint()
//            }
//            // 한 봉 길이보다 더 움직일 경우
//            if(moveX.absoluteValue > (chartLayout.width / dataCnt)){
//                if(moveX < 0){
//                    setChartData(CHART_MOVE_DIRECTION_NAGATIVE)
//                }else {
//                    setChartData(CHART_MOVE_DIRECTION_POSITIVE)
//                }
//            }
//        }
//        return false
//    }

    /** 시작위치 & 이동거리 초기화 */
    private fun initializeTouchPoint(){
        startX = 0F
        moveX = 0F
    }


    var gestureListener = GestureDetector(this)

    override fun onInterceptTouchEvent(ev: MotionEvent?): Boolean {
        onTouchEvent(ev)
        return true
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        Log.d("ChartLayout_onTouch","event: " + event.toString())
        return gestureListener.onTouchEvent(event)
    }

    override fun onShowPress(e: MotionEvent?) {
        Log.d("ChartLayout_onShowPress","event: " + e.toString())
    }

    override fun onSingleTapUp(e: MotionEvent?): Boolean {
        Log.d("ChartLayout_onSingleTap","event: " + e.toString())
        return true
    }

    override fun onDown(e: MotionEvent?): Boolean {
        Log.d("ChartLayout_onDown","event: " + e.toString())
        //        if(e1.action == MotionEvent.ACTION_DOWN){
//            startX = event.x
//        } else if(event.action == MotionEvent.ACTION_MOVE){
//
//        } else if(event.action == MotionEvent.ACTION_UP){
//            initializeTouchPoint()
//        }
//        // 한 봉 길이보다 더 움직일 경우
//        if(moveX.absoluteValue > (chartLayout.width / dataCnt)){
//            if(moveX < 0){
//                setChartData(CHART_MOVE_DIRECTION_NAGATIVE)
//            }else {
//                setChartData(CHART_MOVE_DIRECTION_POSITIVE)
//            }
//        }
        return true
    }

    override fun onFling(e1: MotionEvent?, e2: MotionEvent?, velocityX: Float, velocityY: Float): Boolean {
        Log.d("ChartLayout_onFling","e1: " + e1.toString() + ", e2: " + e2.toString() + ", velocityX: " + velocityX + ", velocityY: " + velocityY)
        return true
    }

    override fun onScroll(e1: MotionEvent, e2: MotionEvent, distanceX: Float, distanceY: Float): Boolean {
        Log.d("ChartLayout_onScroll","e1: " + e1.action + ", e2: " + e2.action + ", distanceX: " + distanceX + ", e1.x: " + e1.x)
        moveX += distanceX
        if(moveX.absoluteValue > (chartLayout.width / dataCnt)){
            if(moveX < 0){
                setChartData(CHART_MOVE_DIRECTION_NAGATIVE)
            }else {
                setChartData(CHART_MOVE_DIRECTION_POSITIVE)
            }
        }
        return true
    }

    override fun onLongPress(e: MotionEvent?) {
        Log.d("ChartLayout_onLongPress","e: " + e.toString())
    }

    private val CHART_MOVE_DIRECTION_POSITIVE = 1
    private val CHART_MOVE_DIRECTION_NAGATIVE = -1
    private val CHART_MOVE_DIRECTION_DEFAULT = 0
}