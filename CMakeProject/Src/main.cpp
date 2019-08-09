/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-05-20 08:53:27
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-09 21:45:06
 */
#include <iostream>
#include <chrono>
#include <thread>
using namespace std;

#include "MyTimer.h"


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
	cout<<"func3..."<<endl;
}


int main(){

	TimerManager manager;
	MyTimer timer1(manager);
	timer1.start(&func1,2000,MyTimer::TimerType::CIRCLE);
	MyTimer timer2(manager);
	timer2.start(&func2,10000,MyTimer::TimerType::ONCE);
	MyTimer timer3(manager);
	timer3.start(&func3,4000,MyTimer::TimerType::CIRCLE);

	while(1)
	{
		this_thread::sleep_for(chrono::microseconds(200));
		manager.detect_timers();
	}

	return 0;
}