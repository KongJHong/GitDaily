/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-04-27 12:06:19
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-08 21:28:59
 */
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <algorithm>
#include <initializer_list>
#include "stdarg.h"
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
#include <sys/time.h>
using namespace std;


//写一个函数，替代to_string(int)，不能调用函数
string m_to_string(int num)
{
    string str;
    char ch[10] = {0};

    int idx = 9;
    while(num > 0)
    {
        ch[idx] = '0' + num % 10;
        num /= 10;
        if(num == 0)break;
        idx--;
    }
    
    for(;idx < 10;idx++)
        str += ch[idx];
    
    return str;
}

void func(int a,int b,int c)
{
    return ;
}


template<typename Iterator,typename T>
void func_impl(Iterator iter,T t)
{
    T temp;
    cout<<sizeof(temp)<<endl;
}

template<typename Iterator>
void func(Iterator iter)
{
    func_impl(iter,*iter);
}

int main(int argc,char **argv)
{
    int i;
    func(&i);
    
    
    system("pause");
    return 0;
}


