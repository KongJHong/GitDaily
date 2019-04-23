/**
 *  使用模拟退火算法(SAA Simulate Annealing Arithmetic)来求解TSP问题
 *  公众号连接:https://mp.weixin.qq.com/s?__biz=MzI3NTkyODIzNg==&mid=2247483874&idx=1&sn=1e49b469ce87b96618e391f64f1b22c8&chksm=eb7c0421dc0b8d37e4e4de828f1d48b7a1bae39b1378131a511e2d8159333e66759830eea2e0&mpshare=1&scene=1&srcid=0419vudkvm597DSecfArImoG&key=68eaf11150cb6642689f2c58edb0fa0de5f615440cf5c7f2e971593a45b2b5e91ca31d2c966ae65ffed0ffe035294481433c564301f55ec3cf6a941b89666812aceec06d5c001c67910945b202b3ef94&ascene=1&uin=MTU2Mjg3MTQ2Mg%3D%3D&devicetype=Windows+10&version=62060739&lang=zh_CN&pass_ticket=j6Pp3dkNVOVMFXKmUFcTLOG4tPW2gGJGlNPVuLlY0fGUt%2FXOd9WtC9hS3CCxFTDa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define T0 50000.0 //初始温度
#define T_end (1e-8)
#define q 0.98 //退火系数
#define L 1000 //每个温度时的迭代次数，即链长
#define N 31 //城市数量

int city_list[N]; //用于存放一个解

//中国31个城市坐标
double city_pos[N][2] = {
                            {1304,2312},{3639,1315},{4177,2244},{3712,1399},
                            {3488,1535},{3326,1556},{3238,1229},{4196,1004},
                            {4312,790} ,{4386,570} ,{3007,1970},{2562,1756},
                            {2788,1491},{2381,1676},{1332,695} ,{3715,1678},
                            {3918,2179},{4061,2370},{3780,2212},{3676,2578},
                            {4029,2838},{4263,2931},{3429,1908},{3507,2367},
                            {3394,2643},{3439,3201},{2935,3240},{3140,3550},
                            {2545,2357},{2778,2826},{2370,2974}
                        };

//函数声明
double distance(double *,double *);//计算两个城市距离
double path_len(int *);     //计算路径长度
void init();                //初始化函数
void create_new();          //产生新解

//距离计算
double distance(double *city1,double *city2)
{
    double x1 = *city1;
    double y1 = *(city1 + 1);
    double x2 = *city2;
    double y2 = *(city2 + 1);
    double dis = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return dis;
}

//计算路径长度
double path_len(int *arr)
{
    double path = 0;    //初始化路径长度
  //  int index = *arr;   //定位到第一个数字（城市序号）
    for(int i = 0;i < N-1; i++)
    {
        int index1 = *(arr+i);
        int index2 = *(arr+i+1);
        double dis = distance(city_pos[index1-1],city_pos[index2-1]);
        path += dis;
    }

    int last_index = *(arr + N -1); //最后一个城市序号
    int first_index = *arr;         //第一个城市序号

    double last_dis = distance(city_pos[last_index - 1],city_pos[first_index - 1]);//形成闭环

    path = path + last_dis;
    return path;
}

//初始化函数
void init()
{
    for(int i = 0; i < N; i++)
        city_list[i] = i + 1;
}

//产生一个新解
//此处采用随机交叉位置的方式产生新的解
void create_new()
{
    double r1 = ((double)rand())/(RAND_MAX+1.0); //产生0.0~1.0的随机数
    double r2 = ((double)rand())/(RAND_MAX+1.0);
    int pos1 = (int)(N*r1); //第一个交叉点的位置
    int pos2 = (int)(N*r2);
    int temp = city_list[pos1];
    city_list[pos1] = city_list[pos2];
    city_list[pos2] = temp; //交换两个点
}

//主函数
int main(void)
{
    srand((unsigned)time(NULL)); //初始化随机种子
    time_t start,finish;
    start = clock();            //程序运行开始计时
    double T;
    int count = 0;              //记录降温次数
    T = T0;                     //初始温度
    init();                     //初始化一个解

    int city_list_copy[N];      //用于保存原始解
    double f1,f2,df;            //f1为初始解目标函数值
                                //f2为新解目标函数值，df为二者差值
    double r;                   //0~1之间的随机数，用来决定是否接受新解
    while(T > T_end)            //当温度低于结束温度时，退火结束
    {
        for(int i = 0;i < L;i++)
        {
            //复制数组
            memcpy(city_list_copy,city_list,N*sizeof(int));
            create_new();   //产生新解
            f1 = path_len(city_list_copy);  //旧
            f2 = path_len(city_list);       //新
            df = f2 - f1;

            //以下是Metropolis准则
            if(df >= 0)                     //新值总长大于旧值
            {
                r = ((double)rand())/(RAND_MAX);
                if(exp(-df/T) <= r) //以一定概率拒绝，不执行则以一定概率接收
                    memcpy(city_list,city_list_copy,N*sizeof(int));
            }
        }
        T *= q; //自然降温
        count++;
    }

    finish = clock();   //退火过程结束

    //计算时间
    double duration = ((double)(finish-start))/CLOCKS_PER_SEC;

    printf("模拟退火算法，初始温度T0=%.2f,降温系数q=%.2f,\
            每个温度迭代%d次，共降温%d次，得到的TSP最优路径为:\n",
            T0,q,L,count);

    for(int i = 0; i < N-1 ; i++)   //输出最优路径
    {
        printf("%d------->",city_list[i]);
    }

    printf("%d\n",city_list[N-1]);
    double len = path_len(city_list);   //最优路径长度
    printf("最优路径长度为:%lf\n",len);
    printf("程序运行耗时:%lf秒\n",duration);
    system("pause");
    return 0;
}