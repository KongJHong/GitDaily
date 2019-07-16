#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

class QueryResult;      //保存查询结果的类
class TextQuery         //保存待查询文本，并对文本中各单词与其出现行号的集合的指针构建map
{
public:
    using line_no = vector<string>::size_type;
    TextQuery(ifstream &);                  //从文本中读取文本信息构建TextQuery类
    QueryResult query(const string&)const;  //返回对一个单词的查询结构，优点类似WordQuery
private:
    shared_ptr<vector<string>> file;        //动态分配内存保存文本内容，每行当做一个string放进vector(file)中
    map<string,shared_ptr<set<line_no>>> wm;//注意这里的map是文本中各单词与其（出现行号的集合）的只恨之间的映射
};

TextQuery::TextQuery(ifstream &is):file(new vector<string>)//并没有在初始化列表一步到位进行初始化，具体的初始化细节在函数体里实现的
{
    string text;
    while(getline(is,text))
    {
        file->push_back(text);          //对file进行初始化
        line_no n = file->size() - 1;   // 记录当前行号
        istringstream line(text);       //将istringstream绑定在text上，便与对其中的各单词进行操作
        string word;
        while(line>>word)   //提取每行中的单词
        {
            //之所以加引号是因为lines是指向set的shared_ptr
            auto &lines = wm[word];
            if(!lines)//如果单词第一次出现lines将会是一个空指针，可我们并不想让它是个空指针,我们只想让它指向一个空的set
                lines.reset(new set<line_no>);
            lines->insert(n);
        }
    
    }
}

class QueryResult
{
    friend ostream &print(ostream &,const QueryResult&);//非成员函数的类接口
public:
    using line_no = TextQuery::line_no;
    QueryResult() = default;    //默认构造，虽然在实际使用中会出错，但还是显式定义了一个
    QueryResult(string s,shared_ptr<set<line_no>> p,shared_ptr<vector<string>> f)
                :sought(s),lines(p),file(f){}

    auto begin() const
    {
        return lines->begin();
    }
    auto end() const
    {
        return lines->end();
    }
    shared_ptr<vector<string>> get_file() const
    {
        return file;
    }
private:
    string sought;                  //查询的内容（准确来说是一个式子）
    shared_ptr<set<line_no>> lines; //单词在文本中出现的行号
    shared_ptr<vector<string>> file;//文本内容，用于在print中进行输出，一行一个string
};

QueryResult TextQuery::query(const string &sought) const
{
    //假如找不到单词，就返回一个指向内容空的指针
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    //loc是指向pair的迭代器
    auto loc = wm.find(sought);
    if(loc == wm.end())//没找到
        return QueryResult(sought,nodata,file);
    else//找到了
        return QueryResult(sought,loc->second,file);//这里不能用wm[sought]，因为query是一个常量成员函数，而map的下标操作默认改变map
}

//之所以用输出流作为参数和返回值是便于不同的输出操作，比如可以标准输出也可以输出到文件里
ostream &print(ostream &os,const QueryResult &qr)
{
    os<<qr.sought<<" occurs "<<qr.lines->size() << " " << "time(s)"<<endl;
    for(auto n:*qr.lines)//n是line_no
    {
        os<<"(line_no:"<<n+1<<")"<<" "<<(*qr.file)[n]<<endl;
    }
    return os;
}

//抽象基类
class Query_base
{
    friend class Query;
protected:
    using line_no = TextQuery::line_no;
    virtual ~Query_base(){}
private:
    //将两个接口（纯虚）函数放在private里，是不想用户(派生类)直接使用Query_base
    //后面接口类将两个函数放在public里就行了，毕竟各种query实际都是Query类的对象
    virtual QueryResult eval(const TextQuery&) const = 0;//eval返回与当前Query匹配的QueryResult
    virtual string rep() const = 0;//rep是表示查询的一个string
};

//接口类，用于隐藏Query_base的集成体系
class Query
{
    //这些运算符都用到了从指针创建Query对象的构造函数
    friend Query operator~(const Query &);
    friend Query operator|(const Query &,const Query &);
    friend Query operator&(const Query &,const Query &);
public:
    Query(const string&);   //与其他类型的query不同，WordQuery可以直接由Query类构造得到
    QueryResult eval(const TextQuery &t) const
    {
        return q->eval(t);//为什么在Query_base里两个函数都是private,在这里却能从外部调用
        //因为Query是Query_base的友元类，用q调用eval/rep函数都是在静态类型中进行名字查找的，所以Query_base的
    }
    string rep() const {return q->rep();}
private:
    Query(shared_ptr<Query_base> query):q(query){}
    shared_ptr<Query_base> q;
};

//不能定义在Query类中（定义在类内改变某种对象的该运算符，定义在类外则会改变所有对象的该运算符操作）
ostream &operator<<(ostream &os,const Query &query)
{
    return os << query.rep();//因为rep在基类里是虚函数，所以这里的调用会执行动态绑定
}

class WordQuery:public Query_base
{
    friend class Query;//Query类用到了WordQuery的构造函数，而友元关系不能继承，只能再次声明
    WordQuery(const string &s):query_word(s){}
    QueryResult eval(const TextQuery &t) const
    {
        return t.query(query_word);//WordQuery的查询可以直接由TextQuery实现
    }
    string rep() const
    {
        return query_word;
    }
    string query_word;
};

//new 返回的普通指针可以直接用来初始化智能指针
inline Query::Query(const string &s):q(new WordQuery(s)){}
class NotQuery:public Query_base
{
    //不需要对Query类进行友元声明
    friend Query operator~(const Query&);//~运算符使用了NotQuery的构造函数
    NotQuery(const Query &q):query(q){}
    //对两个接口函数进行覆盖
    QueryResult eval(const TextQuery&) const;//其实真正区分各种Query的是eval函数
    string rep() const override
    {
        return "~("+query.rep()+")";
    }
    Query query;//要对哪个Query取反
};

inline Query operator~(const Query &q)
{
    return shared_ptr<Query_base>(new NotQuery(q));
}


class BinaryQuery:public Query_base //抽象基类
{
protected://下面这些成员都要在派生类中使用
    BinaryQuery(const Query &q1,const Query &q2,string os):lhs(q1),rhs(q2),opSym(os){}
    //不定义eval函数，直接继承一个纯虚函数，反正自己也是一个抽象基类
    string rep() const override
    {
        return "("+lhs.rep()+" "+opSym+" "+rhs.rep()+")";
    }
    Query lhs;
    Query rhs;
    string opSym;//运算符
};

class AndQuery:public BinaryQuery
{
    friend Query operator&(const Query &,const Query &);
    AndQuery(const Query &q1,const Query &q2):BinaryQuery(q1,q2,"&"){}
    QueryResult eval(const TextQuery&)const;
};

inline Query operator&(const Query &q1,const Query &q2)
{
    return shared_ptr<Query_base>(new AndQuery(q1,q2));
}

class OrQuery:public BinaryQuery
{
    friend Query operator|(const Query &,const Query &);
    OrQuery(const Query &q1,const Query &q2):BinaryQuery(q1,q2,"|"){};
    QueryResult eval(const TextQuery&)const;
};

inline Query operator|(const Query &q1,const Query &q2)
{
    return shared_ptr<Query_base>(new OrQuery(q1,q2));
}

//eval实际就是求lines，lines是创建QueryResult的关键
QueryResult NotQuery::eval(const TextQuery &t) const
{
    //先求得单词出现的行号
    auto result = query.eval(t);
    auto ret_lines = make_shared<set<line_no>>();
    //对于输入文件中的每一行，如果没有出现在result中，则把它添加到ret_lines里
    auto beg = result.begin();
    auto end = result.end();
    auto sz = result.get_file()->size();
    for(size_t n = 0;n != sz;++n)//在文本中遍历
    {
        if(beg == end || n != *beg)
        {   //result中遍历已经结束或者当前行号不在result中，只有file中的指针后移一位
            //之所以这么写，是因为result和file中的行号都是按顺序排列的
            ret_lines->insert(n);
        }
        else if(beg != end)
        {   //当前行号在result当中且result还没遍历完，result和file中的指针都向后一位
            ++beg;
        }
        
    }
    return QueryResult(rep(),ret_lines,result.get_file());
}

QueryResult OrQuery::eval(const TextQuery &t) const
{
    //得到两个Query对象的QueryResult
    auto right = rhs.eval(t);
    auto left = lhs.eval(t);

    //创建行号列表的指针
    auto ret_lines = make_shared<set<line_no>>(left.begin(),left.end());
    ret_lines->insert(right.begin(),right.end());//set会自动忽略重复的关键字
    return QueryResult(rep(),ret_lines,left.get_file());
}

QueryResult AndQuery::eval(const TextQuery &t) const
{
    //得到两个Query对象的QueryResult
    auto right = rhs.eval(t);
    auto left = lhs.eval(t);

    //创建行号列表的指针
    auto ret_lines = make_shared<set<line_no>>();
    //这个函数将两个查询结果的交集添加到ret_lines中
    set_intersection(left.begin(),left.end(),right.begin(),right.end(),inserter(*ret_lines,ret_lines->begin()));
    return QueryResult(rep(),ret_lines,left.get_file());
}

int main()
{
    ifstream ifs("map_file.txt");
    TextQuery tq(ifs);
    Query q = Query("Alice") | Query("wind");
    QueryResult qr = q.eval(tq);
    print(cout,qr);
    system("pause");
    return 0;
}