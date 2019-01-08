#include <iostream>
using namespace std;

#define size_t int

int main(int argc, char const *argv[])
{
    int a[5] = {0, 2, 3, 2, 4}; // 表示第0到第4个月的需求分别为0,2,3,2,4
    double f[5][7] = {0};       //表示f_k(x_k)的花费值

    for (size_t i = 0; i < 5; i++)
    {
        /* code */

        for (size_t j = 0; j < 7; j++)
        {
            /* code */
            f[i][j] = INT_MAX;
        }
    }
    f[5][0] = 0;

    for (size_t k = 4; k >= 1; k--) // 看表示月份从4月到1月
    {
        for (size_t s = 0; s <= 6; s++)
        {
            int tmp = 0;
            for (size_t j = k; j <= 4; j++)
                tmp += a[j];
            if (s > tmp)
                continue;
            double min_value = INT_MAX;
            int u = -1;
            for (size_t r = 0; r <= 6; r++)
            {
                int w = 3 + r;
                if (r == 0)
                    w = 0;
                if (s + r - a[k] >= 0 && s + r - a[k] <= 6)
                {
                    if (min_value > f[k + 1][s + r - a[k]] + w + 0.5 * s)
                    {
                        min_value = f[k + 1][s + r - a[k]] + w + 0.5 * s;
                        u = r;
                    }
                }
            }
            f[k][s] = min_value;
        }
    }
    cout << "min_cost:" << f[1][0] << endl;
    return 0;
}
