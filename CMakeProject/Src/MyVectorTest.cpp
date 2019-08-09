/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-05-20 08:53:27
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-09 18:33:51
 */
#include <iostream>
using namespace std;
#include "MyVector.hpp"


int TestVector(){


	//vector的核心在于 start,finish,capcacity指针
	//1.当初始化时，其实是对这三个指针做操作
	//这就是为什么 allocate_and_fill可以不用返回，而allocate_and_copy需要返回getstart的原因
	
	MyVector<int>::iterator it;

	//默认构造函数
	MyVector<int> mvec;
	cout<<mvec.begin()<<" "<<mvec.end()<<endl;
	cout<<"size="<<mvec.size()<<endl;
	cout<<"capacity="<<mvec.capacity()<<endl;
	for(it = mvec.begin();it != mvec.end();it++){
		cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"--------------------"<<endl;

	//测试 根据元素个数和一个初始值的构造函数
	MyVector<int> mvecnt(2,9);
	cout<<"mvent"<<endl;
	cout<<mvecnt.begin()<<" "<<mvecnt.end() - 1<<endl;
	cout<<"size="<<mvecnt.size()<<endl;
	cout<<"capacity="<<mvecnt.capacity()<<endl;
	for(it=mvecnt.begin();it < mvecnt.end();++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"--------------------"<<endl;

	//从元素个数构造函数
	MyVector<int> mvecnt1(4);
	cout<<"-------mvent1--------"<<endl;
	cout<<mvecnt1.begin()<<" "<<mvecnt1.end() - 1<<endl;
	cout<<"size="<<mvecnt1.size()<<endl;
	cout<<"capacity="<<mvecnt1.capacity()<<endl;
	for(it=mvecnt1.begin();it < mvecnt1.end();++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"--------------------"<<endl;

	//复制构造函数
	MyVector<int> mvecc(mvecnt);
	cout<<"--------mvecc--------"<<endl;
	cout<<mvecc.begin()<<" "<<mvecc.end() - 1<<endl;
	cout<<"size="<<mvecc.size()<<endl;
	cout<<"capacity="<<mvecc.capacity()<<endl;
	for(it=mvecc.begin();it < mvecc.end();++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"--------------------"<<endl;

	//根据两个迭代器构造函数
	int arr[6] = {1,2,3,4,5,6};
	MyVector<int> mvecarr(&arr[0],&arr[5]);
	cout<<"--------mvecarr--------"<<endl;
	cout<<mvecarr.begin()<<" "<<mvecarr.end() - 1<<endl;
	cout<<"size="<<mvecarr.size()<<endl;
	cout<<"capacity="<<mvecarr.capacity()<<endl;
	for(it=mvecarr.begin();it < mvecarr.end();++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"--------------------"<<endl;

	
	//测试push_back
	mvecnt.push_back(10);
	mvecnt.push_back(20);
	mvecnt.push_back(30);
	mvecnt.push_back(10);
	mvecnt.push_back(20);
	mvecnt.push_back(30);
	mvecnt.push_back(10);
	mvecnt.push_back(20);
	mvecnt.push_back(30);
	cout<<mvecnt.begin()<<" "<<mvecnt.end() - 1<<endl;
	cout<<"size="<<mvecnt.size()<<endl;
	cout<<"capacity="<<mvecnt.capacity()<<endl;
	for(it=mvecnt.begin();it < mvecnt.end();++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;
	cout<<"--------------------"<<endl;

	//测试pop_back
	mvecnt.pop_back();
	mvecnt.pop_back();
	mvecnt.pop_back();
	cout<<mvecnt.begin()<<" "<<mvecnt.end() - 1<<endl;
	cout<<"size="<<mvecnt.size()<<endl;
	cout<<"capacity="<<mvecnt.capacity()<<endl;
	for(it=mvecnt.begin();it < mvecnt.end();++it)
	{
		cout<<*it<<" ";
	}
	cout<<endl;

	//测试find
	MyVector<int>::iterator it1 = mvecnt.find(10);
	if(it1 != mvecnt.end())cout<<*it1<<endl;
	else cout<<"no value"<<endl;

	MyVector<int>::iterator it2 = mvecnt.find(100);
	if(it2 != mvecnt.end())cout<<*it2<<endl;
	else cout<<"no value"<<endl;


	return 0;
}