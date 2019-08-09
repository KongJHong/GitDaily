/*
 * @Descripttion: Vector实现
 *  vector其实有三个核心函数，分别是 空间申请并赋值(__allocate_and_fill),空间复制并赋值(__allocate_and_copy),复制(__copy)
 *  由这三个函数组成vector的所有方法
 *  另外还有三个辅助内联函数   destory(T),destory(T begin,T end),construct(T1 *p,T2 value)
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-08-08 19:43:27
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-09 22:06:44
 */

#pragma once
#include <cstddef>

template<class T>
inline void destory(T *pointer){
    pointer->~T();
}

//用placement new在p所指的对象上创建一个对象，value是初始化对象的值
template<class T1,class T2>
inline void construct(T1 *p,const T2 &value){
    new(p) T1(value);   //placement new在p所指的对象上创建一个对象
}

template <class ForwardIterator>
inline void destory(ForwardIterator first,ForwardIterator last){
    for(;first<last;++first)
        ::destory(&*first);
}


template<class T>
class MyVector
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef const value_type*   const_pointer;
    typedef const value_type*   const_interator;
    typedef const value_type&   const_reference;
    typedef size_t  size_type;

protected:
    
    //分配空间，并填充初始值
    void __allocate_and_fill(size_type n,const T& value)
    {
        if(n < 0){
            std::cout<<"n <= 0 error"<<std::endl;
            exit(0); 
        }
        iterator result = (iterator)malloc(n * sizeof(T));
        if(result != nullptr)
        {
            //申请内存成功，在得到的内存上创建对象
            start = result;
            end_of_storage = start + n;
            finish = end_of_storage;
            while(n--)
            {
                construct(result,value);
                ++result;
            }
        }
        else
        {
            std::cout<<"内存不足，程序终止!"<<std::endl;
            exit(0);
        }

        return;
    }

    
    //分配空间，并复制值到空间中
    iterator __allocate_and_copy(iterator first,iterator last,size_type n)
    {
        iterator result = (iterator)malloc(n * sizeof(T));
        iterator _start = result;
        if(result != nullptr)
        {
            while(n--)
            {
                construct(result,*first);
                ++result;
                ++first;
            }
            std::cout<<std::endl;
        }
        else
        {
            std::cout<<"内存不足，程序终止"<<std::endl;
            exit(0);
        }
        return _start;
    }

    //将first到last迭代器之间[first,last)的元素拷贝到_start开始的内存中
    iterator __copy(iterator first,iterator last,iterator _start)
    {
        while(first < last)
        {
            *_start++ = *first++;
        }
        return _start;//返回最终更新位置的后一位
    }


public:
    //返回首元素指针
    iterator begin(){return start;}
    const iterator begin() const{return start;}

    //返回尾元素下一个位置的指针
    iterator end(){return finish;}
    const iterator end() const{return finish;}

    //容器的大小
    size_type size() const{return (size_type)(end() - begin());}
    //容器的实际大小
    size_type capacity() const{return (size_type(end_of_storage - begin()));}

    //判断容器是否为空
    bool empty(){return begin() == end();}
    //默认构造函数,不分配内存空间
    MyVector():start(nullptr),finish(nullptr),end_of_storage(nullptr){std::cout<<"default constructor,offer no space"<<std::endl;}

    //构造函数重载 C c(n,t)
    MyVector(size_type n,const T& value){__allocate_and_fill(n,value);}

    MyVector(int n,const T& value){__allocate_and_fill(static_cast<size_type>(n),value);}
    
    MyVector(long n,const T& value){__allocate_and_fill(n,value);}

    //构造函数重载,不设置初值 C(n)
    MyVector(size_type n){__allocate_and_fill(n,T());}
    MyVector(int n) {__allocate_and_fill(n,T());}
    MyVector(long n){__allocate_and_fill(n,T());}

    //复制构造函数
    MyVector(const MyVector<T> &rhs)
    {
        start = __allocate_and_copy(rhs.begin(),rhs.end(),rhs.end() - rhs.begin());
        finish = start + (rhs.end() - rhs.begin());
        end_of_storage = finish;
    }

    //构造函数重载 迭代器生成  C c(b,e)
    MyVector(const iterator& begin,const iterator &end)
    {
        start = __allocate_and_copy(begin,end,size_type(end-begin+1));
        finish = start + (end - begin + 1);
        end_of_storage = finish; 
    }

    //元素操作
    
    //删除最后一个元素
    void pop_back()
    {
        if(!empty())
        {
            --finish;
            destory(finish);
        }
    }

    //删除指定位置上的元素，返回指向删除元素的迭代器
    iterator erase(iterator position)
    {
        //后一位整体前进
        if(position > begin() && position < end())
        {
            __copy(position+1,finish,position);
        }
        --finish;
        ::destory(finish);
        return position;
    }

    //重载erase,根据迭代器范围删除,返回删除位置的首位
    iterator erase(iterator first,iterator last)
    {
        iterator i = __copy(last,finish,first);
        destory(i,finish);
        finish -= (last-first);
        return first;
    }

    //清楚全部元素
    void clear()
    {
        erase(begin(),end());
    }

    //寻找元素
    iterator find(size_type value){
        iterator it = start;
        while(it != finish)
        {
            if(*it == value)
                return it;
            it++;
        }

        return finish;
    }

    //在vector容器后面增加一个元素
    void push_back(const T&value)
    {
        if(finish != end_of_storage)
        {//如果还有备用空间
            construct(finish,value);
            ++finish;
        }
        else
        {
            //重新申请空间
            const size_type old_size = size();
            const size_type new_size = (old_size==0)?1:2*old_size;
            iterator new_start = (iterator)malloc(new_size *sizeof(T));
            iterator new_finish = new_start;
            //空间分配要有原子性，要么全部成功，要么全部失败
            try{
                //1.将原来的内容拷贝到新的vector
                //2.为新的元素设定初值x
                //3.调整new_finish
                for(iterator it=begin();it < end();it++)
                {
                    construct(new_finish++,*it);
                }
                construct(new_finish,value);
                ++new_finish;
            }
            catch(...)
            {
                //如果失败了
                destory(new_start,new_finish);
                //删除申请到的内存
                free(new_start);
                new_start = new_finish = nullptr;
                throw;  //抛出异常
            }

            //析构并释放原vector
            destory(begin(),end());
            //删除内存
            free(start);
            //调整迭代器，指向新的vecor
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }

    //重载操作符
    reference operator[] (size_type n){return *(begin() + n);}


    const_reference operator[] (size_type n) const{return *(begin()+n);}


    bool operator==(const MyVector &rhs)
    {
        if(rhs.size() != size())return false;
        iterator it = rhs.begin();
        for(;it < rhs.end();it++)
        {
            if(*it != *(begin() + (it-rhs.begin())))
                break;
        }
        if(it == rhs.end())return true;
        else return false;
    }
    

    bool operator!=(const MyVector& rhs)
    {
        return !(operator==(rhs));
    }


private:
    iterator start;
    iterator finish;    //始终指向尾元素的下一个
    iterator end_of_storage;
};

