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
using namespace std;

map<string,string> buildMap(ifstream &map_file)
{
    map<string,string> trans_map;   //保存转换规则
    string key;                     //要转换的单词
    string value;                   //替换后的内容
    //读取第一个单词存入key中，行中剩余内容存入value
    while(map_file >> key && getline(map_file, value)){
        if(value.size() > 1)
            trans_map[key] = value.substr(1);
        else
            throw runtime_error("no rule for "+key);
    }
    return trans_map;
}

const string & transform(const string &s,const map<string,string> &m)
{
    //实际的转换工作
    auto iter = m.find(s);
    if(iter != m.cend())
        return iter->second;
    else
        return s;
}

void word_transform(ifstream &map_file,ifstream &input)
{ 
    auto trans_map = buildMap(map_file);
    string text;
    while(getline(input,text)){
        istringstream stream(text);
        string word;
        bool firstword = true;  //控制是否打印首空格
        while(stream >> word){
            if(firstword)
                firstword = false;
            else
                cout<<" ";

            cout<< transform(word,trans_map);
        }
        cout<<endl;
    }
}

struct Base {
    Base():mem(0){}
    int get_mem(){return mem;}
protected:
    int mem;
};

struct Derived: Base{
    Derived(int i):mem(i){}
    int get_mem(){return mem;}
    int get_base_mem(){return Base::mem;}
protected:
    int mem;
};



int main(void)
{
    static char string[10] = " aefg";

    cout<<sizeof(string)<<endl;
    


    system("pause");
    return 0;
}


