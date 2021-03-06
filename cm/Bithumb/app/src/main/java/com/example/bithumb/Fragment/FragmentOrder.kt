package com.example.bithumb.Fragment

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.GridLayout
import android.widget.TextView
import com.example.bit.utils.Constants
import com.example.bit.utils.RetrofitUtils
import com.example.bithumb.R
import com.example.bithumb.data.order.Order
import com.example.bithumb.data.order.OrderResponse
import kotlinx.android.synthetic.main.fragment_order.*
import retrofit2.Call
import retrofit2.Response

// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"

// TickerMain Data
var paymentCurrency : String? = null// 현재 적용 환율
private var currentExchangeRate : Float? = null// 현재 적용 환율

/**
 * A simple [Fragment] subclass.
 * Use the [SecondFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class FragmentOrder : Fragment() {
    // TODO: Rename and change types of parameters
    private var param1: String? = null
    private var param2: String? = null

    /** bid, ask Data */
    private var bidArray : ArrayList<Order> = arrayListOf()
    private var askArray : ArrayList<Order> = arrayListOf()

    /** Grid layout */
    private var dataCount = 10
    private var rowCount = 20
    private var columnCount = 3


    /** bid, ask TextView */
    private var bidTextView : MutableList<TextView> = mutableListOf()
    private var askTextView : MutableList<TextView> = mutableListOf()
    private var priceTextView : MutableList<TextView> = mutableListOf()

    private var searchOrder = "BTC"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            var paymentCurrency = it.getString("param1") ?: Constants.PAYMENT_CURRENCY_KRW
            var currentExchangeRate = it.getString("param2")?.toFloat() ?: 1F
            Log.d("Fragment2_onCreate","currency: "+paymentCurrency+", rate: "+currentExchangeRate)
        }
    }


    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_order, container, false)
    }

    override fun onActivityCreated(savedInstanceState: Bundle?) {
        super.onActivityCreated(savedInstanceState)
        setTextView()
        bidArray.clear()
        askArray.clear()
        getOrder(searchOrder,dataCount)
        if(bidArray.size!=0&&askArray.size!=0){
            setData()
        }


    }

    fun setTextView(){
//        rowCount = dataCount*2
//        orderlayout.columnCount = columnCount
//        orderlayout.rowCount = rowCount
//        var mView: TextView
//        for(i in 0..rowCount-1){// rowCount == 10 => 0~9
//            for (j in 0..2){
//                mView = TextView(context)
//                if(j == 0&&i<dataCount){
//                    mView.id = R.id.ask + i
//                    askTextView.add(mView)
//                    orderlayout.addView(mView)
//                }else if (j == 1){
//                    mView.id = R.id.price + i
//                    priceTextView.add(mView)
//                    orderlayout.addView(mView)
//                }else if (j == 2 &&i>dataCount-1){
//                    mView.id = R.id.bid + i-dataCount
//                    bidTextView.add(mView)
//                    orderlayout.addView(mView)
//                }
//            }

//            if (i <dataCount){
//                mView.id = R.id.bid + i
//                bidTextView.add(mView)
//                orderlayout.addView(mView)
//            }else{
//                mView.id = R.id.ask + i
//                askTextView.add(mView)
//                orderlayout.addView(mView)
//            }
        bidTextView.add(bid1)
        bidTextView.add(bid2)
        bidTextView.add(bid3)
        bidTextView.add(bid4)
        bidTextView.add(bid5)
        bidTextView.add(bid6)
        bidTextView.add(bid7)
        bidTextView.add(bid8)
        bidTextView.add(bid9)
        bidTextView.add(bid10)

        askTextView.add(ask1)
        askTextView.add(ask2)
        askTextView.add(ask3)
        askTextView.add(ask4)
        askTextView.add(ask5)
        askTextView.add(ask6)
        askTextView.add(ask7)
        askTextView.add(ask8)
        askTextView.add(ask9)
        askTextView.add(ask10)

        priceTextView.add(price1)
        priceTextView.add(price2)
        priceTextView.add(price3)
        priceTextView.add(price4)
        priceTextView.add(price5)
        priceTextView.add(price6)
        priceTextView.add(price7)
        priceTextView.add(price8)
        priceTextView.add(price9)
        priceTextView.add(price10)
        priceTextView.add(price11)
        priceTextView.add(price12)
        priceTextView.add(price13)
        priceTextView.add(price14)
        priceTextView.add(price15)
        priceTextView.add(price16)
        priceTextView.add(price17)
        priceTextView.add(price18)
        priceTextView.add(price19)
        priceTextView.add(price20)
    }


    private fun getOrder(order:String,count:Int){
        RetrofitUtils.getBitService(context).getOrder(order,count)?.enqueue(object : retrofit2.Callback<OrderResponse> {
            // 요청이 성공했을 경우(서버에 요청이 전달 된 상태)
            override fun onResponse(call: Call<OrderResponse>?, response: Response<OrderResponse>) {
                // 정상 Callback을 받은 경우 ( status == 0000 )
                if (response.body()?.status=="0000") {
                    response.body()?.data?.let{ item ->
                        /** 종목명 & DataArray setting */
                        order_currency.text = item.order_currency
                        item.bids.let { bidArray = it }
                        item.asks.let { askArray = it }
                    }
                }
                // 정상 response 받지 못한 경우( ex. 404 error )
                else {
                    Log.d("Second_Fragment", "Not Successful or Empty Response")
                }
            }
            // 요청이 실패했을 경우(서버에 요청이 전달되지 못한 상태)
            override fun onFailure(
                call: Call<OrderResponse>?, t: Throwable ) {
                Log.d("MainActivity", "Connect_Error "+t.message)
                t.printStackTrace()
            }
        })
    }

    fun setData (){
//        orderlayout.
        if(bidArray.size==dataCount&&askArray.size==dataCount){
            bidTextView.forEachIndexed { index, textView ->
                textView.text = bidArray[index].quantity
            }
            askTextView.forEachIndexed { index, textView ->
                textView.text = askArray[index].quantity
            }
            priceTextView.forEachIndexed { index, textView ->
                if (index<dataCount){
                    textView.text = askArray[index].price
                }else{
                    textView.text = bidArray[index-dataCount].price
                }
            }

//            bid1.text = bidArray[0].quantity
//            bid2.text = bidArray[1].quantity
//            bid3.text = bidArray[2].quantity
//            bid4.text = bidArray[3].quantity
//            bid5.text = bidArray[4].quantity
//            bid6.text = bidArray[5].quantity
//            bid7.text = bidArray[6].quantity
//            bid8.text = bidArray[7].quantity
//            bid9.text = bidArray[8].quantity
//            bid10.text = bidArray[9].quantity
//
//            price1.text = bidArray[0].price
//            price2.text = bidArray[1].price
//            price3.text = bidArray[2].price
//            price4.text = bidArray[3].price
//            price5.text = bidArray[4].price
//            price6.text = bidArray[5].price
//            price7.text = bidArray[6].price
//            price8.text = bidArray[7].price
//            price9.text = bidArray[8].price
//            price10.text = bidArray[9].price
//
//            ask1.text = askArray[0].quantity
//            ask2.text = askArray[1].quantity
//            ask3.text = askArray[2].quantity
//            ask4.text = askArray[3].quantity
//            ask5.text = askArray[4].quantity
//            ask6.text = askArray[5].quantity
//            ask7.text = askArray[6].quantity
//            ask8.text = askArray[7].quantity
//            ask9.text = askArray[8].quantity
//            ask10.text = askArray[9].quantity
//
//            price11.text = askArray[0].price
//            price12.text = askArray[1].price
//            price13.text = askArray[2].price
//            price14.text = askArray[3].price
//            price15.text = askArray[4].price
//            price16.text = askArray[5].price
//            price17.text = askArray[6].price
//            price18.text = askArray[7].price
//            price19.text = askArray[8].price
//            price20.text = askArray[9].price
        }
    }

    companion object {
        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment SecondFragment.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            FragmentOrder().apply {
                arguments = Bundle().apply {
                    putString("param1", param1)
                    putString("param2", param2)//putString(ARG_PARAM2, param2)
                    Log.d("Fragment2_newInstance","param1: "+param1+", param1: "+param2)
                    paymentCurrency = param1
                    currentExchangeRate = param2.toFloat()
                }
            }
    }
}