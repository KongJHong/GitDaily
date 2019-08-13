/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-05-20 08:53:27
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-13 22:23:24
 */
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

#include "MyTimer.h"
#include "SkipList_Int.h"


void func1()
{
	cout<<"func1..."<<endl;
}

void func2()
{
	cout<<"func2..."<<endl;
}

void func3()
{
	//添加一个任务进去多线程队列中，是一个回调方法和函数
}

void func(int a)
{
	printf("%d----",a);
}


int main(){

	// TimerManager manager;
	// MyTimer timer1(manager);
	// timer1.start(&func1,2000,MyTimer::TimerType::CIRCLE);
	// MyTimer timer2(manager);
	// timer2.start(&func2,10000,MyTimer::TimerType::ONCE);
	// MyTimer timer3(manager);
	// timer3.start(&func3,4000,MyTimer::TimerType::CIRCLE);

	// while(1)
	// {
	// 	this_thread::sleep_for(chrono::microseconds(200));
	// 	manager.detect_timers();
	// }

	SkipList_Int sk;

	for(int i = 0;i < 100;i++){
		sk.insert(i * 10);
	}

	srand((unsigned)time(NULL));

	for(int i = 0;i < 10;i++){
		int num = (rand() % 200)*10;
		cout<<"find num:"<<num<<"-"<<sk.search(num)<<endl;
	}

	sk.traverse();
	

	return 0;
}