#include <iostream>
#include <cmath>
using namespace std;

const double PRECISION = 0.000000001;
const double INVALID_VALUE = INT_MAX;
const int MAX_RUN_LOOP = 20;

typedef double (*FunctionPtr)(double);

double DichotomyEquation(double a,double b,FunctionPtr f)//二分逼近法
{
    double mid = (a+b)/2.0;
    int cnt = 0;
    while((b-a) > PRECISION)
    {
        if(f(a) * f(mid) <0.0)
            b = mid;
        else
            a = mid;

        mid = (a+b)/2.0;
        cnt++;
    }
    cout<<"二分逼近法times:"<<cnt<<endl;
    return mid;
}

double binFunc(double x)
{
    return (2.0*x*x+3.2*x-1.8);
}

double CalcDerivate(FunctionPtr f,double x) //求导数
{
    return (f(x + 0.000005) - f(x - 0.000005)) / 0.00001;
}

double NewtonRaphson(FunctionPtr f,double x0)
{
        double x = INVALID_VALUE;
        int cnt = 0;
        do
        {
            double x1 = x0 - f(x0)/CalcDerivate(f,x0);
            if(fabs(x1-x0) < PRECISION)
            {
                x = x1;
                break;
            }
            x0 = x1;
            cnt++;
        } while (cnt < MAX_RUN_LOOP);
        
        cout<<"牛顿迭代法times:"<<cnt<<endl;
        return x;
}


int main(void){
    double x = DichotomyEquation(-0.8,12.0,binFunc);
    cout<<"二分逼近法求解结果："<<x<<endl;


    double y = NewtonRaphson(binFunc,8.0);
    cout<<"牛顿迭代法法求解结果1："<<y<<endl;
    double z = NewtonRaphson(binFunc,-8.0);
    cout<<"牛顿迭代法法求解结果2："<<z<<endl;
    system("pause");
    return 0;
}


