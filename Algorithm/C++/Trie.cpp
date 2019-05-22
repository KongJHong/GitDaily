/**
 *  前缀树Trie
 */

#include <iostream>
#include <cstring>

using namespace std;

const int branchNum = 26;    //26个英文字母

/**
 * Trie树每个节点，指出是否为完整字符串
 * 以及其下一个字符
 */
struct TrieNode
{
    bool isStr;                 //标记是否是一个完整的字符串
    TrieNode *next[branchNum];  //每个字符的Trie树分支可能存在26个分支
};

//函数声明
void Insert(TrieNode *root,const char *word);
bool Search(TrieNode *root,const char *word);
void Delete(TrieNode *node);

int main()
{
    //初始化前缀树的根节点，注意这里结构体指针的初始化
    //使用TrieNode *root = new TrieNode();
    TrieNode *root = new TrieNode();
    root->isStr = false;

    //前缀树中每个节点的下一个节点，分配空间，注意memset的使用
    memset(root->next,NULL,sizeof(root->next));

    Insert(root,"a");
    Insert(root,"bcd");
    Insert(root,"xyz");
    Insert(root,"abcdef");

    if(Search(root,"a"))
    {
        cout<<"a exist"<<endl;
    }

    Delete(root);
    return 0;
}


/**
 *  前缀树的建立过程
 */
void Insert(TrieNode *root,const char *word)
{
    TrieNode *location = root;
    while(*word)
    {
        //节点的字节点为空，新建一个字节点
        if(location->next[*word-'a'] == NULL)
        {
            TrieNode *newNode = new TrieNode();
            newNode->isStr = false;
            memset(newNode->next,NULL,sizeof(newNode->next));

            location->next[*word-'a'] = newNode;
        }
        location = location->next[*word - 'a'];
        word++;
    }

    //字符串已经全部添加到前缀树中
    //表示前缀树到该节点为止是完整字符串
    location->isStr = true;
}

/**
 * 遍历前缀树，查看指定字符串是否存在
 */
bool Search(TrieNode *root,const char *word)
{
    TrieNode *location = root;
    while(*word && location != NULL)
    {
        location = location->next[*word - 'a'];
        word++;
    }
    return (location != NULL && location->isStr);
}

/**
 * 删除前缀树
 */
void Delete(TrieNode *location)
{
    for(int i = 0;i < branchNum;i++)
    {
        if(location->next[i] != NULL)
        {
            Delete(location->next[i]);
        }
    }
    delete location;
}