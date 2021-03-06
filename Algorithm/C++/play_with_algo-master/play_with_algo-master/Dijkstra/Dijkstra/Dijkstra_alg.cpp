#include "Dijkstra_alg.h"


void init_dijkstra(Dijkstra_T *dj, Problem_T *p_data)
{
    //将T初始化为只包含起始点S的状态，即T{ S }
    dj->T[0] = p_data->start_vertex;
    dj->tc = 1;

    //初始化dist数组的值为起始点S到顶点Vx的边的权重，即dist[Vx] = w(S, Vx)
    dj->vertexs = p_data->vertexs;
    for (int i = 0; i < dj->vertexs; i++)
    {
        dj->dist[i].is_in_t = false;
        //对无穷大的处理，已经隐含在邻接矩阵w的处理中了
        dj->dist[i].value = p_data->w[p_data->start_vertex][i];
    }
    //起始点特殊处理一下
    dj->dist[p_data->start_vertex].is_in_t = true;  //起始点初始化在T中了
    dj->dist[p_data->start_vertex].value = 0; //对于起始点S，则设置dist[S] = 0；
}

void dijkstra(Dijkstra_T *dj, Problem_T *p_data)
{
    while (dj->tc < dj->vertexs) //直到集合T包含了所有顶点，第5步描述的循环终止条件
    {
        //第3步
        //对于所有不属于集合T的顶点Vi，从dist数组中寻找最小值dist[Vi]
        int min_dist = INFINITE;
        int min_vi = -1;
        for (int i = 0; i < dj->vertexs; i++)
        {
            if (!dj->dist[i].is_in_t && (dj->dist[i].value < min_dist))
            {
                min_dist = dj->dist[i].value;
                min_vi = i;
            }
        }

        //将顶点Vi加入到集合T中，完成第3步
        dj->T[dj->tc] = min_vi; //新加入顶点min_vi
        dj->tc++;
        dj->dist[min_vi].is_in_t = true;

        //第4步
        //对于新加入的顶点Vi，查看Vi能够到达的所有顶点
        for (int j = 0; j < dj->vertexs; j++)
        {
            if (p_data->w[min_vi][j] < INFINITE) //从min_vi能直接到达j
            {
                int new_dist = dj->dist[min_vi].value + p_data->w[min_vi][j];
                //从起点到min_vi的最小距离加上min_vi到j的距离之和小于从起点直接到j的距离
                if(new_dist < dj->dist[j].value)
                {
                    //替换dist数组中对应顶点的值
                    dj->dist[j].value = new_dist;
                }
            }
        }
    }

}

