// integration.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <iomanip>
#include <cmath>
#include <functional>

double trapezium(std::function<double(double)> func, double a, double b, int n)
{
    double step = (b - a) / n;
    double sum = (func(a) + func(b)) / 2.0;

    for (int j = 1; j < n; j++)
    {
        double xj = a + j * step;
        sum = sum + func(xj);
    }

    sum *= step;

    return sum;
}

double vs_trapezium(std::function<double(double)> func, double a, double b, double eps)
{
    int n = 1;   //初始分割一个大梯形
    double t1 = (b - a) * (func(a) + func(b)) / 2.0; //计算初始梯形的面积
    double diff = eps + 1.0;
    do
    {
        n = 2 * n;    //梯形分割加倍
        double t = trapezium(func, a, b, n);
        diff = std::fabs(t1 - t);      //计算两次迭代的结果差值
        t1 = t;
    } while (diff >= eps);

    return t1;
}

double simpson(std::function<double(double)> func, double a, double b, int n)
{
    double s1, s2;
    int i;

    double step = (b - a) / n;
    s1 = s2 = 0;
    for (i = 1; i < n; i++)
    {
        s2 += func(a + step * i);   //xi求和
    }
    for (i = 1; i <= n; i++)
    {
        s1 += func(a + step * i - step / 2);  //（xi - step/2） 求和
    }

    double s = step * (func(a) + func(b) + 4 * s1 + 2 * s2) / 6.0;

    return s;
}

double vs_simpson(std::function<double(double)> func, double a, double b, double eps)
{
    int n = 1;   //初始分割一个大梯形
    double s1 = (b - a) * (func(a) + func(b) + 4 * func((a + b) / 2.0)) / 6.0; //计算初始梯形的面积
    double diff = eps + 1.0;
    do
    {
        n = 2 * n;    //梯形分割加倍
        double t = simpson(func, a, b, n);
        diff = std::fabs(s1 - t);      //计算两次迭代的结果差值
        s1 = t;
    } while (diff >= eps);

    return s1;
}
const int LINES = 2;
const int STATIONS = 7;
typedef struct 
{
    int assemble_time[LINES][STATIONS];
    int transport_time[LINES][STATIONS];
    int enter_time[LINES];
    int exit_time[LINES];
}Program_T;
typedef struct
{
    int line[STATIONS];  //遍历过程中的当前结果记录
    int fs;
    int fline[STATIONS];  //当前已知的最优结果
    int ffs;
}Result_T;

void search_stations_sequence(Result_T *rt, Program_T *para, int line, int station)
{
    if (station == (STATIONS - 1)) //1. 完成装配，整理一次结果，退出当前递归子结构
    {
        rt->fs += para->assemble_time[line][station];
        rt->fs += para->exit_time[line];
        rt->line[station] = line;
        if (rt->fs < rt->ffs) /*当前穷举到的路径时间开销更小*/
        {
            rt->ffs = rt->fs;
            memmove(rt->fline, rt->line, STATIONS * sizeof(int));
        }
        return;
    }

    //2. 记录中间结果到 line 属性中
    int curCost = rt->fs + para->assemble_time[line][station];
    rt->line[station] = line;

    //3. 调整 line 和 station 参数，完成递归子结构的动作
    /*选择本装配线的下一个装配站，开销忽略不计*/
    rt->fs = curCost;
    search_stations_sequence(rt, para, line, station + 1);

    /*选择另一条装配线的下一个装配站，需要计算转移开销*/
    rt->fs = curCost;
    rt->fs += para->transport_time[line][station + 1];
    int nextline = (line + 1) % LINES;
    search_stations_sequence(rt, para, nextline, station + 1);
}


int main()
{
    auto func1 = [](double x)-> double { return sin(x)/x; };
    double result = trapezium(func1, 0.000000001, 1.0, 10000);
    std::cout << "trapezium value (10000 steps) " << std::setprecision(12) << result << std::endl;

    result = vs_trapezium(func1, 0.000000001, 1.0, 0.000000001);
    std::cout << "vs_trapezium value (EPS = 0.000000001) " << std::setprecision(12) << result << std::endl;

    result = simpson(func1, 0.000000001, 1.0, 10000);
    std::cout << "simpson value (10000 steps) " << std::setprecision(12) << result << std::endl;

    result = vs_simpson(func1, 0.000000001, 1.0, 0.000000001);
    std::cout << "vs_simpson value (EPS = 0.000000001) " << std::setprecision(12) << result << std::endl;
    system("pause");
    return 0;
}

