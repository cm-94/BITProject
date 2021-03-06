package com.example.fx.ui

import android.graphics.Color
import android.os.Bundle
import android.text.Spannable
import android.text.SpannableStringBuilder
import android.text.style.AbsoluteSizeSpan
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentTabHost
import androidx.viewpager.widget.ViewPager
import com.example.fx.R
import com.example.fx.data.NewOrder
import com.example.fx.fragment.FragmentAdapter
import com.example.fx.fragment.FragmentFirst
import com.example.fx.fragment.FragmentSecond
import com.example.fx.fragment.FragmentThird
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    var host : FragmentTabHost?=null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // TODO TabBar 세팅


        // TODO 통신 연결

        // TODO 데이터 세팅(NewOrder Object)
        setNewOrderData()

        // TODO View 세팅
        initView()
    }


    private fun setNewOrderData(){
        /** 고가 */
        NewOrder.maxprice = 105.597F
        /** 저가 */
        NewOrder.minprice = 105.453F
        /** 시가비 */
        NewOrder.fluctate = 0.042F


        /** 매도 호가 */
        NewOrder.sellprice = 105.498F
        /** 매수 호가 */
        NewOrder.callprice = 105.503F
        /** 스프레드 */
        NewOrder.spread = 0.5F


        /** 내 잔액 */
        NewOrder.balance = 500000
        /** 주문 총 수량 */
        NewOrder.callAmount = 0

        /** 주문 단위(1,000 or 10,000) */
        NewOrder.callUnit = 1000

        /** 주문 개별 수량(총 수량/단위) */
        NewOrder.callUnitCount = 1
        /** 주문 개별 Pips/단위 */
        NewOrder.pipsCount = 3.0F


        /** 설정된 지정 pips */
        NewOrder.sellAutoPips = 2.5F
        NewOrder.buyAutoPips = 2.5F

    }

    /**
     * - activity_main의 각 View들을 초기화 한다
     * - NewOrder object property 사용
     * TODO 사전에 데이터를 수신해 NewOrder를 초기화 해주는 작업이 필요하다
     */
    private fun initView(){
        /** Title Bar */
        tabTitle.text = getString(R.string.new_order)
        tabTitle.textSize = 18F

        /** high, low, opening price */
        // TODO 데이터 변동에 따른 이미지 변경( ↑ , ↓ )
//        pmImage.setImageResource(R.drawable.ic_minus)

        /** 매도,매수 가격 & 차액(spread) => call_price, sell_price & spr */
        val callStr = SpannableStringBuilder(NewOrder.callprice.toString())            // 데이터(.1f) -> String으로
        callStr.setSpan(AbsoluteSizeSpan(230),4,6, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE) // 마지막 두글자 크기 ↑
        call_price.text = callStr

        val sellStr = SpannableStringBuilder(NewOrder.sellprice.toString())            // 데이터(.1f) -> String으로
        sellStr.setSpan(AbsoluteSizeSpan(230),4,6, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE) // 마지막 두글자 크기 ↑
        sell_price.text = sellStr

        high_price.text = NewOrder.maxprice.toString() // 고거
        low_price.text = NewOrder.minprice.toString()  // 저가
        fluctate.text = NewOrder.fluctate.toString()   // 시가 대비(변동)

        val spreadStr = SpannableStringBuilder(resources.getString(R.string.spread, NewOrder.spread))          // 데이터(.3f) -> String으로
        spreadStr.setSpan(AbsoluteSizeSpan(45),5,9, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE)// 마지막 세글자 크기 ↑
        spread_text.text = spreadStr // Spread( 세번째 레이아웃 가운데 Text )
//        spread_text.text = resources.getString(R.string.spread,NewOrder.spread)


        /** Viewpager & adpater */
        // fragment adapter instance 생성
        val mFragmentAdapter = FragmentAdapter(supportFragmentManager)
        // fragment adapter에 fragment 생성 후 추가(fragment,Title)
        mFragmentAdapter.addFragment(FragmentFirst(), "fragment1")
        mFragmentAdapter.addFragment(FragmentSecond(), "fragment2")
        mFragmentAdapter.addFragment(FragmentThird(), "fragment3")
        // viewpager.adapter 초기화
        viewpager.adapter = mFragmentAdapter

        /**
         *  Radio Button Click
         * - click된 radio button의 해당 fragment로 변경
         * - Scroll or 버튼 클릭 시 둘 다 발생!! ( 스크롤 시 checked 변경하도록 해서..!! )
         */
        radio_group_tab.setOnCheckedChangeListener { _, i ->
            when(i){
                R.id.radio_button_tab1 -> {
//                    var tran = supportFragmentManager.beginTransaction()
//                    tran.replace(R.id.viewpager,mFragmentAdapter.getItem(0))
//                    tran.commit()

//                    tran.remove(mFragmentAdapter.getItem(0))
                    /** 페이지 변경 시 새 데이터 적용 */
//                    mFragmentAdapter.replaceFragment(0,FragmentFirst())
//                    viewpager.adapter = mFragmentAdapter

                    /**
                     *  FragmentFirst가 Main에 올라가있으면 onStart() 실행 후 페이지 변경!!
                     *  => onStart()에서 새 데이터 반영해서 View 구성
                     */
                    if(mFragmentAdapter.getItem(0).isAdded){
                        mFragmentAdapter.getItem(0).onStart()
                    }
                    viewpager.currentItem = 0
                }
                R.id.radio_button_tab2 -> {
                    if(mFragmentAdapter.getItem(1).isAdded){
                        mFragmentAdapter.getItem(1).onStart()
                    }
                    viewpager.currentItem = 1
                }
                R.id.radio_button_tab3 -> {
                    if(mFragmentAdapter.getItem(2).isAdded){
                        mFragmentAdapter.getItem(2).onStart()
                    }
                    viewpager.currentItem = 2
                }
            }
        }

        /**
         *  ViewPager Selected
         * - Page Scroll 발생 시 해당 Radio Button으로 Check상태 변경
         */
        viewpager.setOnPageChangeListener (object : ViewPager.OnPageChangeListener {
            override fun onPageScrolled(
                position: Int,
                positionOffset: Float,
                positionOffsetPixels: Int,
            ) {

            }
            override fun onPageSelected(position: Int) {
                /** position번째 => check */
                when(position){
                    0->{
                        radio_group_tab.check(R.id.radio_button_tab1)               /** Radio button check된 것 변경 */
                        sellLayout.setBackgroundResource(R.drawable.ic_order_sell)  /** Order(sell) 배경 변경 */
                        callLayout.setBackgroundResource(R.drawable.ic_order_buy)   /** Order(call) 배경 변경 */

                        sellText.setTextColor(Color.WHITE)                           /** Order(sell) Text색 변경 */
                        sell_price.setTextColor(Color.WHITE)
                        callText.setTextColor(Color.WHITE)                          /** Order(call) Text색 변경 */
                        call_price.setTextColor(Color.WHITE)

                        chartButton.visibility = View.VISIBLE                       /** chart 버튼 보이게 */
                        tabTitle.text = getString(R.string.new_order)                     /** Title TextView 변경 */
                        tabTitle.textSize = 18F
                    }
                    1->{
                        radio_group_tab.check(R.id.radio_button_tab2)
                        sellLayout.setBackgroundResource(R.drawable.ic_order_sell_underline)
                        callLayout.setBackgroundResource(R.drawable.ic_order_buy_underline)

                        sellText.setTextColor(Color.BLUE)
                        sell_price.setTextColor(Color.BLUE)
                        callText.setTextColor(Color.RED)
                        call_price.setTextColor(Color.RED)

                        chartButton.visibility = View.GONE
                        tabTitle.text = getString(R.string.new_appointment_order)
                        tabTitle.textSize = 15F
                    }
                    2->{
                        radio_group_tab.check(R.id.radio_button_tab3)
                        sellLayout.setBackgroundResource(R.drawable.ic_order_sell_underline)
                        callLayout.setBackgroundResource(R.drawable.ic_order_buy_underline)

                        sellText.setTextColor(Color.BLUE)
                        sell_price.setTextColor(Color.BLUE)
                        callText.setTextColor(Color.RED)
                        call_price.setTextColor(Color.RED)

                        chartButton.visibility = View.GONE
                        tabTitle.text = getString(R.string.new_IFD_order)
                        tabTitle.textSize = 18F
                    }
                }
            }
            override fun onPageScrollStateChanged(state: Int) {

            }
        })
    }
}
