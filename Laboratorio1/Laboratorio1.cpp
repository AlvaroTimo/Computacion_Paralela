#include <iostream>
#include <cstdlib>
#include <ctime>

#define MAX 100000

using namespace std;

double A[MAX][MAX], x[MAX], y[MAX];
void initializeA()
{
    for (long long i = 0; i < MAX; i++)
        for (long long j = 0; j < MAX; j++)
        {
            A[i][j] = rand();
        }
}
void funcion1()
{
    for (long long i = 0; i < MAX; i++)
        for (long long j = 0; j < MAX; j++)
            y[i] += A[i][j] * x[j];
}
void funcion2()
{
    for (long long j = 0; j < MAX; j++)
        for (long long i = 0; i < MAX; i++)
            y[i] += A[i][j] * x[j];
}
int main()
{
    initializeA();
    unsigned t0, t1;
    double time;
    t0 = clock();
    funcion1();
    t1 = clock();
    time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Primera funcion: " << time << endl;
    t0 = clock();
    funcion2();
    t1 = clock();
    time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "Segunda funcion: " << time << endl;
    return 0;
}