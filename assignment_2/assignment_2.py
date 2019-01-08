#!/usr/bin/env python
# -*- coding:utf-8 -*-
'''
 * @Author: shifaqiang(石发强)--[14061115@buaa.edu.cn] 
 * @Date: 2019-01-07 16:10:00 
 * @Last Modified by:   shifaqiang 
 * @Last Modified time: 2019-01-07 16:10:00 
 * @Desc: python implementation for algorithm analysis assignment_3
'''

import numpy as np
import time


def floyd(graph):
    '''
    from wikipad (https://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm),
    floyd algorithm is an method for finding shortest paths in a weighted graph 
    with positive or negative edge weights (but with no negative cycles).
    A single execution of the algorithm will find the lengths (summed weights) 
    of shortest paths between all pairs of vertices. 

    1 let dist be a |V| × |V| array of minimum distances initialized to ∞ (infinity)
    2 for each edge (u,v)
    3    dist[u][v] ← w(u,v)  // the weight of the edge (u,v)
    4 for each vertex v
    5    dist[v][v] ← 0
    6 for k from 1 to |V|
    7    for i from 1 to |V|
    8       for j from 1 to |V|
    9          if dist[i][j] > dist[i][k] + dist[k][j] 
    10             dist[i][j] ← dist[i][k] + dist[k][j]
    11         end if
    '''
    assert isinstance(graph, np.ndarray)
    # define 9999 as infinity
    n = graph.shape[0]
    for k in range(n):
        for i in range(n):
            for j in range(n):
                graph[i][j] = min(graph[i][j], graph[i][k]+graph[k][j])
    return graph


def dfs(distance, cost, mindist, mincost):
    """
    dfs with branch and bound
    """
    # check if parameters are legal
    assert isinstance(distance, np.ndarray) and isinstance(mindist, np.ndarray)
    assert isinstance(cost, np.ndarray) and isinstance(mincost, np.ndarray)
    assert distance.shape == cost.shape and distance.shape == mindist.shape
    assert cost.shape == mincost.shape
    # set some variable
    n = distance.shape[0]
    res = None
    # depth is the top of stack, cur is the current city, cur_next is the next feasible city of current city, visited is flag for all cities if they are visited
    stack, depth, cur, cur_next, visited = [0]*(n+2), 0, 0, 0, [False]*n
    # initialization of the dfs stack with start point city 0
    visited[0] = True
    # seting the bound
    cost_bound, distacne_bound, cur_distance, cur_cost = 1500, np.inf, 0, 0
    while depth >= 0:
        found, cur, cur_next = -1, stack[depth], stack[depth+1]
        for i in range(cur_next+1, n):
            # attempt all neighbor cities for city cur
            if distance[cur][i] == 9999 or visited[i]:
                continue
            elif cur_cost+mincost[cur][n-1] > cost_bound or cur_distance+mindist[cur][n-1] > distacne_bound:
                continue  # prune operation
            elif i < n:
                found = i
                break  # find a new feasible unvisited city, break the for loop
        if found == -1:
            # no feasible next city for cur city, backtracking
            depth -= 1
            cur_distance -= distance[stack[depth]][stack[depth+1]]
            cur_cost -= cost[stack[depth]][stack[depth+1]]
            visited[stack[depth+1]] = False
        else:
            # found a feasible next neighbor city for current city, update current path with cost, distance, path stack and visited record
            cur_cost += cost[stack[depth]][found]
            cur_distance += distance[stack[depth]][found]
            depth += 1
            stack[depth], stack[depth+1], visited[found] = found, 0, True
            if found == n-1:
                # arrive at terminal city, found a new feasible solution
                if cur_cost > 1500:
                    continue
                res = stack[:depth+1], cur_cost, cur_distance
                # update bound
                distacne_bound = cur_distance
                # backtracking
                for i in range(2):
                    depth -= 1
                    cur_distance -= distance[stack[depth]][stack[depth+1]]
                    cur_cost -= cost[stack[depth]][stack[depth+1]]
                    visited[stack[depth+1]] = False
    return res


if __name__ == "__main__":
    # load data
    start_time = time.time()
    distance = np.genfromtxt("m1.txt", skip_header=False,
                             delimiter='\t', dtype=np.int32)
    cost = np.genfromtxt("m2.txt", skip_header=False,
                         delimiter='\t', dtype=np.int32)
    assert distance.shape == cost.shape
    # compute result
    mindist = floyd(np.copy(distance))
    mincost = floyd(np.copy(cost))
    res = dfs(distance, cost, mindist, mincost)
    print("number of cities in best path:{}\nbest path:{}\nminimum cost:{}\nminimum distance:{}".format(
        len(res[0]), np.array(res[0])+1, res[1], res[2]))
    print("time cost:{}s".format(time.time()-start_time))
