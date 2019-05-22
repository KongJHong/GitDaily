#include <iostream>
#include <string>
#include <set>

using namespace std;
class Message;

class Folder
{
public:
    Folder();
    ~Folder();

    void addMsg(Message *);
    void remMsg(Message *);

private:
    set<Message*> messages;
    void remove_Fldr_from_Messages();
};


class Message
{
public:
    //folders被隐式初始化为空集合
    explicit Message(const string &str=""):contents(str){};

private:
    string contents;            //实际消息文本
    set<Folder*> folders;       //包含本Message的Folder
    
    //将本Message添加到指定参数的Folder中
    void add_to_Folders(const Message&);
    //从folders中的每个Folder中删除本Message
    void remove_from_Folders();
};