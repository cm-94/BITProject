package com.example.fx.fragment

import android.os.Bundle
import android.text.Editable
import android.text.TextWatcher
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import com.example.fx.R
import com.example.fx.data.NewOrder
import com.example.fx.utils.Constants
import kotlinx.android.synthetic.main.button_item.view.*
import kotlinx.android.synthetic.main.fragment_first.*
import java.text.DecimalFormat
import java.text.NumberFormat

// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"

var formatter: NumberFormat = DecimalFormat("#,###")

/**
 * A simple [Fragment] subclass.
 * Use the [FragmentFirst.newInstance] factory method to
 * create an instance of this fragment.
 */
class FragmentFirst : Fragment() {
    // TODO: Rename and change types of parameters
    private var param1: String? = null
    private var param2: String? = null


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
            Log.d("FirstFragment!!", "param1:$param1,param2:$param2")
        }
        savedInstanceState.let{
            Log.d("FirstFragment!!", "Bundle!!" + it.toString())
        }
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?,
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_first, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
    }


    override fun onStart() {
        super.onStart()
        /** 발주가능수량  & 주문 수량 TextView*/
        val maxAmount :String =formatter.format(NewOrder.balance)
        Log.d("Error!!", "maxAmount:$maxAmount, NewOrder.balance:${NewOrder.balance}")
        max_quantity_value1.text = maxAmount
        NewOrder.callAmount = NewOrder.callUnit*NewOrder.callUnitCount
        call_quantity_value1.text = formatter.format(NewOrder.callAmount)

        /** 주문 수량 & pips SPButton */
        call_button.setDataType(NewOrder.callUnitCount, Constants.CALL_QUANTITY)
//        Log.d("Fragment1","call_button data:"+call_button.dataEditText.text.toString())
        pips_button.setDataType(NewOrder.pipsCount, Constants.PIPS_QUANTITY)
//        Log.d("Fragment1","pips_button data:"+pips_button.dataEditText.text.toString())

        /** 주문수량 Radio Button ( x1,000 x10,000 )*/
        // 초기 NewOrder.callUnit 값 (0 or 1,000 or 10,000) 에 따른 Radio Button 체크 상태 설정
        when(NewOrder.callUnit){
            0 -> radio_group_call.check(R.id.radio_button_1000)
            1000 -> radio_group_call.check(R.id.radio_button_1000)
            10000 -> radio_group_call.check(R.id.radio_button_10000)
        }
        // call Radio Button 클릭 시 NewOrder.callUnit 변경 & 주문수량 TextView 변경
        radio_group_call.setOnCheckedChangeListener { _, i ->
            when(i){
                R.id.radio_button_1000 -> NewOrder.callUnit = 1000
                R.id.radio_button_10000 -> NewOrder.callUnit = 10000
            }
            // 주문 수량 데이터 변경( 수량 x 단위 )
            NewOrder.callAmount = call_button.getData().toInt()*NewOrder.callUnit
            call_quantity_value1.text = formatter.format(NewOrder.callAmount)

        }

        /** Max 버튼 리스너 */
        maxCallButton.setOnClickListener {
            /** 주문 수량 */
            NewOrder.callUnitCount = NewOrder.balance/ NewOrder.callUnit
            call_button.setDataType(NewOrder.callUnitCount, Constants.CALL_QUANTITY)
            call_quantity_value1.text = maxAmount
        }

        /** 주문수량 증감(+,-) 버튼 리스너 */
        call_button.minusButton.setOnClickListener {
            NewOrder.callUnitCount = call_button.calcData(Constants.SUB_NUMBER).toInt() // 주문 개별 수량
            NewOrder.callAmount = NewOrder.callUnitCount*NewOrder.callUnit              // 주문 총 수량 = 주문 개별 수량 X 주문 단위
            call_quantity_value1.text = formatter.format(NewOrder.callAmount)
        }
        call_button.plusButton.setOnClickListener {
            NewOrder.callUnitCount = call_button.calcData(Constants.ADD_NUMBER).toInt() // 주문 개별 수량
            NewOrder.callAmount = NewOrder.callUnitCount*NewOrder.callUnit              // 주문 총 수량 = 주문 개별 수량 X 주문 단위
            call_quantity_value1.text = formatter.format(NewOrder.callAmount)
        }

        /** SP Button 길게 클릭 시 해당 EditText값을 바로 call_quantity_value(주문수량) TextView에 반영!! */
        call_button.inputData.addTextChangedListener(object : TextWatcher {
            override fun onTextChanged(s: CharSequence, start: Int, before: Int, count: Int) {
                // 입력되는 텍스트에 변화가 있을 때
                NewOrder.callUnitCount = s.toString().toInt()                   // 주문 개별 수량
                NewOrder.callAmount = NewOrder.callUnitCount*NewOrder.callUnit  // 주문 총 수량 = 주문 개별 수량 X 주문 단위
                call_quantity_value1.text = formatter.format(NewOrder.callAmount)
            }

            override fun afterTextChanged(arg0: Editable) {
                // 입력이 끝났을 때
            }

            override fun beforeTextChanged(s: CharSequence, start: Int, count: Int, after: Int) {
                // 입력하기 전에
            }
        })

        /** pips 증감(+,-) 버튼 리스너 */
        pips_button.minusButton.setOnClickListener {
            // ex) pips 데이터 계산 결과 => 3.2pips(String) => 3.2 => NewOrder.pipsCount 변경
            NewOrder.pipsCount = pips_button.calcData(Constants.SUB_NUMBER).let{it.substring(0,
                it.length - 4).toFloat()}
            // 새 NewOrder.pipsCount로 pips_button의 EditText 변경
            pips_button.setDataType(NewOrder.pipsCount, Constants.PIPS_QUANTITY)
        }
        pips_button.plusButton.setOnClickListener {
            NewOrder.pipsCount = pips_button.calcData(Constants.ADD_NUMBER).let{it.substring(0,
                it.length - 4).toFloat()}
            pips_button.setDataType(NewOrder.pipsCount, Constants.PIPS_QUANTITY)
        }


        /** 자동 손절매 Switch */
        pipAutoSwitch.isChecked = false  // 초기 값 : false / OFF
        pipAutoSellValue.text = resources.getString(R.string.pipAutoOff)
        pipAutoBuyValue.text = resources.getString(R.string.pipAutoOff)

        // 리스너 등록
        pipAutoSwitch.setOnCheckedChangeListener { _, b ->
            if(b){
                pipAutoSellValue.text = resources.getString(R.string.pipData, NewOrder.sellAutoPips)
                pipAutoBuyValue.text = resources.getString(R.string.pipData, NewOrder.buyAutoPips)
            }else{
                pipAutoSellValue.text = resources.getString(R.string.pipAutoOff)
                pipAutoBuyValue.text = resources.getString(R.string.pipAutoOff)
            }
        }
    }


    fun refresh(){

        val tr = fragmentManager?.beginTransaction()
        tr?.detach(this)?.attach(this)

    }

    companion object {

        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment FragmentFirst.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance() =
            FragmentFirst().apply {
                arguments = Bundle().apply {

                }
            }




    }
}