#include "mainwindow.h"
#include <vector>
#include "wta.h"
using namespace std;
extern vector<double> X ;

int Calculate_WTA(int u[], double** p, int n, double Ta,int Number_of_loop, bool check_box)
{
    int save = For_loop(u,p,n,Ta,Number_of_loop,check_box);
    Analyze(n);
    return save;
}

int Stop_Condition(double v_dot[], int n, float epsilon)
{
    int check = 0;
    for (int i = 0 ; i < 2*n; i++)
    {
        if (abs(v_dot[i]) > epsilon) check++;
    }
    return check;
}

int For_loop(int u[], double** p, int n, double Ta,int Number_of_loop, bool check_box)
{
    //Khai bao

    double* w = new double[n * n];
    double* v = new double[2 * n];

    double* x_dot = new double[n * n];
    double* w_dot = new double[n * n];
    double* v_dot = new double[2 * n];

    double* g = new double[n];

    for (int i = 0; i < n * n; i++)
    {
        w[i] = 1;
        w_dot[i] = 0;
        x_dot[i] = 0;
    }
    for (int i = 0; i < 2 * n; i++)
    {
        v[i] = 1;
        v_dot[i] = 0;
    }
    int save_cycle = 0;
    for (int k = 0; k < Number_of_loop; k++)
    {
        //Cal g
        for (int i = 0; i < n; i++)
        {
            g[i] = 1;
            for (int j = 0; j < n; j++)
            {
                g[i] = g[i] * pow((double)(1 - p[i][j]), (double)(X[i * n + j]));
            }
        }
        // Cal x_dot
        for (int i = 0; i < n*n; i++)
        {
            int a = i/n;
            int b = i%n;
            x_dot[i] = -(u[a] * g[a] * log(1 - p[a][b]) - 0.5 * w[i] * w[i] + v[a] + v[n + b]);
        }
        //Cal w_dot
        for (int i = 0; i < n * n; i++)
        {
            w_dot[i] = -1 * w[i] * X[i];
        }
        //Cal v_dot
        for (int i = 0; i < 2 * n; i++)
        {
            v_dot[i] = -1;
        }
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                v_dot[i] = v_dot[i] + X[n * i + j];
                v_dot[n + i] = v_dot[n + i] + X[n * j + i];
            }
        }
        //Update x,w,v
        for (int i = 0; i < n * n; i++)
        {
            X[i] += x_dot[i] * Ta;
            w[i] += w_dot[i] * Ta;
        }
        for (int i = 0; i < 2 * n; i++)
        {
            v[i] += v_dot[i] * Ta;
        }
        if (k >= 100000 && Stop_Condition(v_dot,n,0.05) == 0 && check_box == true)
        {
            save_cycle = k;
            break;
        }
    }
    delete []w;
    delete []v;
    delete []g;
    delete []x_dot;
    delete []w_dot;
    delete []v_dot;
    return save_cycle;
}

void Analyze(int n)
{
    for (int i = 0; i < n*n; i++)
    {
        if (X[i] > 0.5)
        {
            X[i] = 1;
            int a = i / n;
            int b = i % n;
            for (int j = 0; j < n; j++)
            {
                if (a * n + j != i) X[a * n + j] = 0;
            }
            for (int j = 0; j < n; j++)
            {
                if (b + n * j != i) X[b + n * j] = 0;
            }
        }
    }
    for (int i = 0; i < n * n; i++)
    {
        if (X[i] != 0 && X[i] != 1)
        {
            int Qcheck=0;
            int a = i / n;
            int b = i % n;
            for (int j = 0; j < n; j++)
            {
                if (X[a * n + j] > X[i] ) Qcheck += 1;
            }
            for (int j = 0; j < n; j++)
            {
                if (X[b + n * j] > X[i]) Qcheck += 1;
            }
            if (Qcheck == 0) X[i] = 1;
        }
    }
    for (int i = 0; i < n * n; i++)
    {
        if (X[i] != 1) X[i] = 0;
    }

    for (int i = 0; i < n * n; i++)
    {
        if (X[i] == 0)
        {
            int Qcheck=0;
            int a = i / n;
            int b = i % n;
            for (int j = 0; j < n; j++)
            {
                if (X[a * n + j] != 0 ) Qcheck += 1;
            }
            for (int j = 0; j < n; j++)
            {
                if (X[b + n * j] != 0 ) Qcheck += 1;
            }
            if (Qcheck == 0) X[i] = 1;
        }
    }
}
