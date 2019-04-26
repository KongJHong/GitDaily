#include <cstdio>
#include <cstring>
#include <random>
#include <ctime>
#include <iostream>
#include <queue>
#define MAXVEX 20   //最大顶点数

using namespace std;
typedef char VertexType;    //顶点类型
int visited[MAXVEX] = {0};        //标志数组

//邻接矩阵存储结构
typedef struct 
{
    VertexType vexs[MAXVEX];    //顶点数组
    int arc[MAXVEX][MAXVEX];    //边的权数
    int numVertexes;   //实际顶点数
}MGraph;

//创建邻接矩阵
void Create(MGraph *G)
{
    int i,j;

    printf("输入顶点数:\n");
    scanf("%d",&G->numVertexes);

    //输入顶点信息，创建顶点表
    printf("输入顶点信息(A~Z 空格隔开，输入个数与定点数相等)：\n");
    for(i=0;i<G->numVertexes;i++){
        getchar();
        scanf("%c",&G->vexs[i]);
    }

    //随机生成邻接矩阵
    for(i=0;i<G->numVertexes;i++){
        for(j=i;j<G->numVertexes;j++){
            if(i==j){
                G->arc[i][j] = 0;
                continue;
            }
            G->arc[i][j] = rand() % 2;
            G->arc[j][i] = G->arc[i][j]; //对称
        }
    }

    //输出邻接表
    printf("邻接矩阵为:\n");
    for(i = 0;i<G->numVertexes;i++){
        for(j = 0;j<G->numVertexes;j++)
            printf("%3d",G->arc[i][j]);
        printf("\n");
    }
}

void DFS(MGraph G,int i){
    int j;
    visited[i] = 1;
    printf("%c ",G.vexs[i]);
    for(j=0;j<G.numVertexes;j++)
        if(G.arc[i][j] != 0 && visited[j] == 0)
            DFS(G,j);
}

//邻接矩阵的深度遍历操作
void DFSTraverse(MGraph G){
    int i;
    for(i=0;i<G.numVertexes;i++)
        visited[i] = 0;
    for(i=0;i<G.numVertexes;i++){
        if(visited[i] == 0){
            DFS(G,i);
            //if(i != G.numVertexes-1)printf(",");
        }
    }
}

void BFS(MGraph G,int i){
    queue<int> Q;
    Q.push(i);
    visited[i] = 1;
    while(!Q.empty())
    {
        int front = Q.front();
        cout<<G.vexs[front]<<" ";
        Q.pop();
        for(int idx = 0; idx <= G.numVertexes;idx++)
        {
            if(!visited[idx] && G.arc[front][idx] == 1){
                visited[idx] = 1;
                Q.push(idx);
            }
        }
    }
}

void BFSTraverse(MGraph G){
    for(int i =0;i < G.numVertexes;i++){
        if(visited[i] == 1)continue;
        BFS(G,i);
    }
}

int main(void)
{
    srand((unsigned)time(NULL));
    MGraph G;
    Create(&G);

    cout<<"深度遍历:";
    DFSTraverse(G);
    cout<<endl;

    memset(visited,0,sizeof(visited));
    
    cout<<"广度遍历:"; 
    BFSTraverse(G);


    system("pause");
    return 0;
}


