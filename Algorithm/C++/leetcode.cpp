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


int calculate(string s) {
        int ans = 0,inter_ans = 0,num = 0;
        int sz = s.size();
        char op = '+';
        char ch;
        for(int pos = s.find_first_not_of(' ');pos<sz;pos=s.find_first_not_of(' ',pos))
        {      
            ch = s[pos];
            if(isdigit(ch))
            {
                num = ch - '0';
                while(++pos<sz && isdigit(s[pos]))
                    num = num * 10 + ch - '0';
                switch(op)
                {
                    case '+':
                        inter_ans += num;
                        break;
                    case '-':
                        inter_ans -= num;
                        break;
                    case '*':
                        inter_ans *= num;
                        break;
                    case '/':
                        inter_ans /= num;
                        break;
                    default:break;
                }
            }
            else{
                if(ch == '+' || ch == '-')
                {
                    ans += inter_ans;
                    inter_ans = 0;
                }
                op = s[pos++];
            }
        }
        
        return ans + inter_ans;
    }


#include <string.h>
class Request{
public:
    void setContent(const char* str){
       // cout<<strlen(m_content)<<endl;
        m_content = str;
        cout<<strlen(m_content)<<endl;
        cout<<m_content<<endl;
    }

    void setStr(const std::string &str){
        cout<<m_str.size()<<endl;
        m_str = str;
        cout<<m_str<<endl;
        cout<<m_str.size()<<endl;
    }

    const char *m_content;
    std::string m_str;
};



#define TIME_OUT 120
int main(void)
{
    
    ostringstream os;
    os<<"timeout="<<TIME_OUT;

    string str = os.str();
    cout<<str<<endl;

    system("pause");
    return 0;
}


