/*
 * @Descripttion: 
 * @version: 
 * @Author: KongJHong
 * @Date: 2019-08-13 20:27:46
 * @LastEditors: KongJHong
 * @LastEditTime: 2019-08-13 22:22:50
 */
#include "SkipList_Int.h"

#include <cstdio>
static unsigned int seed = NULL; //随机种子

bool SkipList_Int::randomVal(){
    if(seed == NULL){
        seed = (unsigned)time(NULL);
    }

    ::srand(seed);
    int ret = ::rand() % 2;
    seed = ::rand();
    
    return ret == 0;
}


void SkipList_Int::traverse(){
    node *cursor = head;
    while(cursor->down != nullptr)cursor = cursor->down;
    cursor = cursor->right;
    while(cursor->right != nullptr){
        printf("%d ",cursor->value);
        cursor = cursor->right;
    }
}


void SkipList_Int::insert(int val){
    /* 首先查找L1层 */
    node *cursor = head;
    node *new_node = nullptr;
    while(cursor->down != nullptr){
        cursor = cursor->down;
    }

    node *cur_head = cursor;//当前层链表头
    while(cursor->right != nullptr){
        if(val < cursor->right->value && new_node == nullptr){
            new_node = new node(val);
            new_node->right = cursor->right;
            cursor->right = new_node;
            break;
        }
        cursor = cursor->right; //向右移动游标
    }

    if(new_node == nullptr){
        new_node = new node(val);
        cursor->right = new_node;
    }

    /*L1层插入完成*/
    int cur_lv = 1; //当前所在层
    while(randomVal()){
        cur_lv++;
        if(lv_num < cur_lv){
            lv_num++;
            node *new_head = new node();
            new_head->down = head;
            head->up = new_head;
            head = new_head;
        }

        cur_head = cur_head->up;    //当前链表头上移一层
        cursor = cur_head;          //继续获取游标
        node *skip_node = nullptr;  //非L1层的节点
        while(cursor->right != nullptr){
            if(val < cursor->right->value && skip_node == nullptr){
                skip_node = new node(val);
                skip_node->right = cursor->right;
                cursor->right = skip_node;
                break;
            }
            cursor = cursor->right;
        }

        if(skip_node == nullptr){
            skip_node = new node(val);
            cursor->right = skip_node;
        }

        while(new_node->up != nullptr){
            new_node = new_node->up;
        }

        //连接上下两个节点
        skip_node->down = new_node;
        new_node->up = skip_node;
    }
    return ;
}


bool SkipList_Int::search(int val){
    node *cursor = nullptr;
    if(head == nullptr)
        return false;
        
    /*初始化游标指针*/
    cursor = head;
    while(cursor->down != nullptr){    //第一层循环游标向下
        while(cursor->right != nullptr){    //第二层循环向右
            if(val <= cursor->right->value){
                break;
            }
            cursor = cursor->right;
        }
        cursor = cursor->down;  //一直到底
    }


    //L1层循环开始具体查询
    while(cursor->right != nullptr){
        if(val > cursor->right->value){
            cursor = cursor->right;             //如果查找的值大于右侧则游标可以继续向右
        }
        else if(val == cursor->right->value){
            return true;
        }
        else if(val < cursor->right->value){
            return false;
        }
    }
    return false;
}

void SkipList_Int::remove(int val){
    node *cursor = head;    //获得游标
    node *pre_head = nullptr;   //上一行的头指针，删除时使用（降维）
    while(true){
        node *cur_head = cursor;    //当前头指针
        if(pre_head != nullptr){//降维----------
            cur_head->up = nullptr;
            pre_head->down = nullptr;   //解除上下级指针
            delete pre_head;
            pre_head = nullptr;         //指针归零
            lv_num--;                   //层数-1
            head = cur_head;            //重新制定起始指针     
        }

        while(cursor != nullptr && cursor->right != nullptr){ //在当前行中查询val
            if(val == cursor->right->value){
                node *delptr = cursor->right;
                cursor->right = cursor->right->right;
                delete delptr;
                break;
            }
            cursor = cursor->right;
        }

        if(cur_head->right == nullptr){ //判断当前行是否还存在其他元素，如果不存在则删除该行并将整个跳跃表降维
            pre_head = cur_head;
        }

        if(cur_head->down == nullptr){
            break;
        }
        else{
            cursor = cur_head->down;
        }
    }
}