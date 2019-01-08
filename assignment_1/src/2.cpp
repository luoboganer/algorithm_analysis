#include <iostream>
#include <memory.h>
#include <climits>
#include <algorithm>

using namespace std;

#define size_t int

int tsp(int n, int **d)
{
    int **f = new int *[n];
    //这里用1<<n表示2^(n-1)
    int count = 1 << n;
    for (size_t i = 0; i < n; i++)
        f[i] = new int[count];
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < count; j++)
            f[i][j] = INT_MAX;
    f[0][0] = 0;

    for (size_t s = 0; s < count; s++)
    {
        for (size_t i = 0; i < n; i++)
        {
            if (!(s & (1 << i) == 0))
            {
                for (size_t j = 0; j < n; j++)
                {
                    if (!((i == j) || ((j != 0) && (s & (1 << j)) == 0)))
                        f[i][s] = min(f[i][s], f[j][s ^ (1 << i)] + d[j][i]);
                }
            }
        }
    }
    return f[0][count - 1];
}

int main()
{
    /*
    input data
    6
    0	10	20	30	40	50
    12	0	18	30	25	21
    23	19	0	5	10	15
    34	32	4	0	8	16
    45	27	11	10	0	18
    56	22	16	20	12	0
    */
    int n; // number of cities
    cin >> n;
    int **d = new int *[n];
    for (size_t i = 0; i < n; i++)
    {
        d[i] = new int[n];
        for (size_t j = 0; j < n; j++)
            cin >> d[i][j];
    }
    cout << "Answer=" << tsp(n, d) << endl;
}