#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <limits>

using namespace std;
struct record
{
    int length; // number of cities
    vector<int> city;
    int minimum_cost;
    int minimum_dist;
};
#define size_t int

void print(vector<vector<int>> &matrix)
{
    // auxiliary output function
    for (auto i : matrix)
    {
        for (auto j : i)
            cout << j << " ";
        cout << endl;
    }
}
void load_matrix(string filename, vector<vector<int>> &matrix)
{
    // load a 2-dimensions array to matrix from file filename
    vector<int> tmp;
    string line;
    ifstream in(filename);           //open file filename to ifstream
    regex pat_regex("[[:digit:]]+"); //match a type int number
    while (getline(in, line))
    {
        for (sregex_iterator it(line.begin(), line.end(), pat_regex), end_it; it != end_it; ++it)
        {
            tmp.push_back(stoi(it->str()));
        }
        matrix.push_back(tmp);
        tmp.clear();
    }
}
void floyd(vector<vector<int>> &graph)
{
    int count = graph.size();
    for (size_t k = 0; k < count; k++)
        for (size_t i = 0; i < count; i++)
            for (size_t j = 0; j < count; j++)
                graph[i][j] = min(graph[i][j], graph[i][k] + graph[k][j]);
}
record branch_and_bound(vector<vector<int>> &dist, vector<vector<int>> &cost, vector<vector<int>> &mindist, vector<vector<int>> &mincost)
{
    /*
        depth is the top of stack, cur is the current city, cur_next is the next feasible city of current city, visited is flag for all cities if they are visited
    */
    int n = dist.size(), depth = 0;
    record res;
    int cost_bound = 1500, distance_bound = INT32_MAX, cur_distance = 0, cur_cost = 0;
    int *stack = new int[n + 2];
    bool *visited = new bool[n];
    memset(stack, 0, sizeof(int) * (n + 2));
    memset(visited, false, sizeof(bool) * n);
    visited[0] = true; //push first city 0 to stack and visit it
    while (true)
    {
        int found = -1, cur = stack[depth], cur_next = stack[depth + 1];
        for (size_t i = cur_next + 1; i < n; i++)
        {
            /* attempt all neighbor cities for city cur */
            if ((dist[cur][i] == 9999) || visited[i])
                continue;
            else if ((cur_cost + mincost[cur][n - 1] > cost_bound) || (cur_distance + mindist[cur][n - 1] > distance_bound))
                continue; // prune operation
            else if (i < n)
            {
                found = i; // find a new feasible unvisited city i, break the for loop
                // cout << "found city " << i << " for cur city " << cur << endl;
                break;
            }
        }
        if (found == -1)
        {
            // no feasible next city for cur city, backtracking
            depth--;
            if (depth < 0)
                break;
            cur_distance -= dist[stack[depth]][stack[depth + 1]];
            cur_cost -= cost[stack[depth]][stack[depth + 1]];
            visited[stack[depth + 1]] = false;
        }
        else
        {
            // found a feasible next neighbor city for current city, update current path with cost, distance, path stack and visited record
            cur_cost += cost[stack[depth]][found];
            cur_distance += dist[stack[depth]][found];
            depth++;
            stack[depth] = found, stack[depth + 1] = 0, visited[found] = true;
            if (found == n - 1)
            {
                // arrive at terminal city, found a new feasible solution
                if (cur_cost > 1500)
                    continue;
                // update bound and record current best result
                distance_bound = cur_distance;
                res.length = depth + 1;
                res.city.clear();
                for (size_t i = 0; i < res.length; i++)
                    res.city.push_back(stack[i] + 1);
                res.minimum_cost = cur_cost;
                res.minimum_dist = cur_distance;
                // backtracking for other feasible solution
                for (size_t i = 0; i < 2; i++)
                {
                    depth--;
                    cur_distance -= dist[stack[depth]][stack[depth + 1]];
                    cur_cost -= cost[stack[depth]][stack[depth + 1]];
                    visited[stack[depth + 1]] = false;
                }
            }
        }
    }
    return res;
}
int main(int argc, char const *argv[])
{
    time_t start = clock();
    // load data
    string dist_filename = "m1.txt", cost_filename = "m2.txt";
    vector<vector<int>> dist, cost, mindist, mincost;
    load_matrix(dist_filename, dist);
    mindist = dist;
    floyd(mindist);
    load_matrix(cost_filename, cost);
    mincost = cost;
    floyd(mincost);
    record res = branch_and_bound(dist, cost, mindist, mincost);
    cout << "number of cities in best path:" << res.length << "\nbest path:";
    for (size_t i = 0; i < res.length; i++)
        cout << res.city[i] << " ";
    cout << "\nminimum cost:" << res.minimum_cost << "\nminimum distance:" << res.minimum_dist << endl;
    cout << "time cost:" << (clock() - start) / double(CLOCKS_PER_SEC) << "s" << endl;
    return 0;
}
