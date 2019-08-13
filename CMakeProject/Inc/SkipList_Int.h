/*
 * @Descripttion: 跳表的实现
 * @version: https://www.cnblogs.com/learnhow/p/6749648.html
 * @Author: KongJHong
 * @Date: 2019-08-13 20:27:29
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-13 22:12:01
 * 
 * 简单的跳跃表，它允许简单的插入和删除元素，并提供O(logn)的查询时间复杂度
 * SkipList_Int的性质
 * 1. 由很多层结构组成，level是通过一定的概率随机产生的，基本是50%的产生几率
 * 2. 每一层都是一个有序的链表，默认是升序，每一层的链表头作为跳点
 * 3. 最底层(level 1)的链表包含所有元素
 * 4. 如果一个元素出现在level i的链表中，则它在level i之下的链表都会出现
 * 5. 每个节点包含4个指针，但有可能为nullptr
 * 6. 每一层链表横向为单向连接，纵向为双向连接
 */
#ifndef SKIPLIST_INT_H
#define SKIPLIST_INT_H

#include <cstdlib>      /* srand,rand */
#include <ctime>        /* time */
#include <climits>      /* INT_MIN */

//Simple SkipList_Int 表头始终是列表最小的节点
class SkipList_Int{
private:
    /*节点元素*/
    typedef struct Node{
        Node(int val = INT_MIN):value(val),up(nullptr),down(nullptr),left(nullptr),right(nullptr){}

        int value;
        //设置4个方向上的指针
        struct Node *up;    //上
        struct Node *down;  //下
        struct Node *left;  //左
        struct Node *right; //右
    }node;

public:
    SkipList_Int():lv_num(1){
        head = new node();
    }

    /* 插入新元素 */
    void insert(int val);

    /* 查询元素 */
    bool search(int val);

    /* 删除元素 */
    void remove(int val);

    /* 遍历元素 */
    void traverse();

protected:
    node *head;     //头节点，查询起始点
    int lv_num;     //当前链表层数
    /*随机判断*/
    bool randomVal();

};




#endif