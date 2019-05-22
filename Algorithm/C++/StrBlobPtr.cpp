/**
 *  普通类及其指针类 
 */

#include <iostream>
#include <string>
#include <vector>
#include <initializer_list>
#include <memory>
using namespace std;

class StrBlobPtr;

class StrBlob{
public:
    typedef std::vector<std::string>::size_type size_type;

    StrBlob();
    StrBlob(std::initializer_list<std::string> il);
    size_type size() const{return data->size();}
    bool empty()  const{return data->empty();}

    //添加和删除元素
    void push_back(const string &t){data->push_back(t);}
    void pop_back();

    //元素访问
    std::string& front();
    std::string& back();

    //返回指向首元素和尾元素的StrBlobPtr
    StrBlobPtr begin(){return StrBlobPtr(*this);}
    StrBlobPtr end(){
        auto ret = StrBlobPtr(*this,data->size());
        return ret;
    }



    std::shared_ptr<std::vector<std::string>> data;
private:
    

    //如果data[i]不合法，抛出一个异常
    void check(size_type i, const std::string &msg) const;
};

//构造函数
StrBlob::StrBlob(): data(make_shared<vector<string>>()){}
StrBlob::StrBlob(std::initializer_list<string> il):data(make_shared<vector<string>>(il)){}

//check函数
void StrBlob::check(size_type i, const string &msg) const{
    if(i >= data->size());
        throw out_of_range(msg);
}


string& StrBlob::front(){
    //如果vector为空,check会抛出一个异常
    check(0,"front on empty StrBlob");
    return data->front();
}

string& StrBlob::back(){
    check(0,"back on empty StrBlob");
    return data->back();
}

void StrBlob::pop_back(){
    check(0,"pop_back on empty StrBlob");
    data->pop_back();
}

//---------------------------------------------------------------------------------------------------
//对应指针类
class StrBlobPtr{
public:
    StrBlobPtr():curr(0){}
    StrBlobPtr(StrBlob &a,size_t sz = 0):wptr(a.data),curr(sz){}

    string& deref() const;
    StrBlobPtr& incr(); //前缀递增

private:
    //若检查成功,check返回一个指向vector的shared_ptr
    shared_ptr<vector<string>> check(size_t,const string&) const;

    //保存一个weak_ptr,意味着底层vector可能被销毁
    weak_ptr<vector<string>> wptr;
    size_t curr;//在数组中的当前位置
};



shared_ptr<vector<string>> StrBlobPtr::check(size_t i,const string& msg) const{
    auto ret = wptr.lock();//vector还在吗?
    if(!ret)
        throw runtime_error("unbound StrBlobPtr");
    if(i>=ret->size())
        throw out_of_range(msg);
    return ret;
}


//解引用
string& StrBlobPtr::deref() const{
    auto p = check(curr,"dereference past end");
    return (*p)[curr];
}

//递增
StrBlobPtr& StrBlobPtr::incr(){
    //如果curr已经指向容器的末尾位置，就不能递增它
    check(curr,"increment past end of StrBlobPtr");
    ++curr;
    return *this;
}




