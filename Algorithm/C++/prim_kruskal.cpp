/**
 * 
 * https://blog.csdn.net/a2392008643/article/details/81781766
 * 
 */
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
#define INFINITE 0xFFFFFFFF
#define UINT unsigned
#define vexCounts 6

typedef unsigned VertexData;

char vextex[] = {'A','B','C','D','E','F'};

struct Node{//prim算法辅助节点
    VertexData data;
    UINT lowestcost;
}closedge[vexCounts];

typedef struct{
    VertexData u;
    VertexData v;
    UINT cost;  //边的代价
}Arc;           //原始图的边信息

void AdjMatrix(UINT adjMat[][vexCounts]){   //邻接矩阵表示
    for(int i = 0; i < vexCounts;i++)   //初始化邻接矩阵
        for(int j = 0; j < vexCounts;j++)
            adjMat[i][j] = INFINITE;
        
    adjMat[0][1] = 6;adjMat[0][2] = 1;adjMat[0][3] = 5;
    adjMat[1][0] = 6;adjMat[1][2] = 5;adjMat[1][4] = 3;
    adjMat[2][0] = 1;adjMat[2][1] = 5;adjMat[2][3] = 5;adjMat[2][4] = 6;adjMat[2][5] = 4;
    adjMat[3][0] = 5;adjMat[3][2] = 5;adjMat[3][5] = 2;
    adjMat[4][1] = 3;adjMat[4][2] = 6;adjMat[4][5] = 6;
    adjMat[5][2] = 4;adjMat[5][3] = 2;adjMat[5][4] = 6;
}

int MinCost(Node* closedge){                //返回最小代价边
    UINT min = INFINITE;
    int index = -1;
    for(int i = 0;i < vexCounts;i++)
    {
        if(closedge[i].lowestcost < min && closedge[i].lowestcost != 0)
        {
            min = closedge[i].lowestcost;
            index = i;
        }
    }
    return index;
}

void MiniSpanTree_Prim(UINT adjMat[][vexCounts],VertexData s){

    for(int i = 0; i < vexCounts;i++)
        closedge[i].lowestcost = INFINITE;
    
    closedge[s].data = s;       //从顶点s开始
    closedge[s].lowestcost = 0;

    for(int i =0; i < vexCounts;i++)//初始化辅助数组
    {
        if(i != s)
        {
            closedge[i].data = s;
            closedge[i].lowestcost = adjMat[s][i];
        }
    }    

    for(int e = 1; e < vexCounts;e++)//n-1条边时退出
    {
        int k = MinCost(closedge);
        cout<<vextex[closedge[k].data] << "--" << vextex[k] <<endl;//加入到最小生成树
        closedge[k].lowestcost = 0;         //代价设置为0
        for(int i = 0; i < vexCounts;i++)   //更新v中顶点最小代价边的信息
        {
            if(adjMat[k][i] < closedge[i].lowestcost)
            {
                closedge[i].data = k;
                closedge[i].lowestcost = adjMat[k][i];
            }
        }
    }
}

void ReadArc(UINT adjMat[][vexCounts],vector<Arc> &vexArc){ //保存图的边代价信息
    Arc *temp = nullptr;
    for(UINT i = 0; i < vexCounts;i++)
    {
        for(UINT j = 0; j < i;j++)
        {
            if(adjMat[i][j] != INFINITE)
            {
                temp = new Arc;
                temp->u = i;
                temp->v = j;
                temp->cost = adjMat[i][j];
                vexArc.push_back(*temp);
            }
        }
    }
}

bool compare(Arc A,Arc B){
    return A.cost<B.cost?true:false;
}

bool FindTree(VertexData u,VertexData v,vector<vector<VertexData>> &Tree){
    UINT index_u = INFINITE;
    UINT index_v = INFINITE;
    for(UINT i = 0; i < Tree.size();i++)//检查u,v分别属于哪颗树
    {
        if(find(Tree[i].begin(),Tree[i].end(),u)!=Tree[i].end())
            index_u = i;
        if(find(Tree[i].begin(),Tree[i].end(),v)!=Tree[i].end())
            index_v = i;
    }

    if(index_u != index_v)  //u,v不在一棵树上，合并两个树
    {
        for(UINT i = 0;i < Tree[index_v].size();i++)
        {
            Tree[index_u].push_back(Tree[index_v][i]);
        }
        Tree[index_v].clear();
        return true;
    }
    return false;
}

void MiniSpanTree_Kruskal(UINT adjMat[][vexCounts]){
    vector<Arc> vecArc;
    ReadArc(adjMat,vecArc); //读取边信息
    sort(vecArc.begin(),vecArc.end(),compare);//边按从小到大排序
    vector<vector<VertexData>> Tree(vexCounts); //6棵独立树
    for(UINT i = 0; i < vexCounts;i++)
    {
        Tree[i].push_back(i);   //初始化独立树的信息
    }

    for(UINT i = 0;i < vecArc.size();i++)   //依次从小到大取最小代价边
    {
        VertexData u = vecArc[i].u;
        VertexData v = vecArc[i].v;
        if(FindTree(u,v,Tree))//检查此边的两个顶点是否在一棵树内
        {
            cout<<vextex[u] << "---"<<vextex[v]<<endl;
        }
    }
}

int main(void)
{
    UINT adjMat[vexCounts][vexCounts] = {0};
    AdjMatrix(adjMat);//邻接矩阵
    cout<<"Prim :"<<endl;
    MiniSpanTree_Prim(adjMat,0);
    cout<<"-------------------------"<<endl<<"Kruskal :"<<endl;
    MiniSpanTree_Kruskal(adjMat);

    system("pause");
    return 0;
}


