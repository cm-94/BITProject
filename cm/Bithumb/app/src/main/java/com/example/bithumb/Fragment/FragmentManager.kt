package com.example.bithumb.Fragment

import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentPagerAdapter

class FragmentManager(fm: FragmentManager) : FragmentPagerAdapter(fm) {
    private val mFragmentList: MutableList<Fragment> = ArrayList()   // fragment
    private val mFragmentTitleList: MutableList<String> = ArrayList()// fragment title

    /**
     * - Activity에서 Fragment를 추가할 때 호출할 메서드
     * - mFragmentList에 Fragment 추가.
     * - mFragmentTitleList에 title 추가
     * @param fragment( Fragment() ),title( String )
     * @return Unit
     */
    fun addFragment(fragment: Fragment, title: String) {
        mFragmentList.add(fragment)
        mFragmentTitleList.add(title)
    }

    /**
     * 필수 override method
     * getItem : return fragment[i]
     * getCount : return fragment.size
     */

    override fun getItem(position: Int): Fragment {
        return mFragmentList[position]
    }

    override fun getCount(): Int {
        return mFragmentList.size
    }

    override fun getPageTitle(position: Int): CharSequence? {
        return mFragmentTitleList[position]
    }

}