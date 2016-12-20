//
//  Warshall.cpp
//  DS-AI-Challange
//
//  Created by Shayan Khorsandi on 12/20/16.
//  Copyright © 2016 Shayan Khorsandi. All rights reserved.
//

#include "Warshall.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>

Warshall::Warshall(World *myWorld)
{
    Nodes = myWorld -> getMap() -> getNodes();
    size = Nodes.size();
    MY_MAX = size + 100;
    
    graph = new int*[size];
    D = new int*[size];
    P = new int*[size];
    
    for(int cnt = 0; cnt < size; cnt++)
    {
        graph[cnt] = new int[size];
        D[cnt] = new int[size];
        P[cnt] = new int[size];
    }
    
    std::vector<Node*> neighbours;
    
    for(int i = 0; i < size; i++)
    {
        graph[i][i] = 1;
        neighbours = Nodes[i] -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            graph[i][neighbour -> getIndex()] = 1;
        }
    }
    
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(i == j)
                P[i][j] = MY_MAX;
            else if(graph[i][j] == 1)
            {
                D[i][j] = 1;
                P[i][j] = j;
            }
            else
            {
                D[i][j] = MY_MAX;
                P[i][j] = MY_MAX;
            }
        }
    }
    
    for(int k = 0; k < size ; k++)
    {
        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(D[i][k]+D[k][j] < D[i][j])
                {
                    D[i][j] = D[i][k]+D[k][j];
                    P[i][j] = P[i][k];
                }
            }
        }
    }
}

int Warshall::minDistanceFrom(Node *from, std::vector<Node*> list)
{
    int MIN = MY_MAX;
    for(auto& e : list)
    {
        if(D[from -> getIndex()][e -> getIndex()] < MIN)
            MIN = D[from -> getIndex()][e -> getIndex()];
    }
    return MIN;
}

int Warshall::avgDistanceList(Node *from, std::vector<Node*> list)
{
    int AVG = 0, cnt = 0;
    for(auto& e : list)
    {
        if(D[from -> getIndex()][e -> getIndex()] != MY_MAX)
        {
            cnt++;
            AVG += D[from -> getIndex()][e -> getIndex()];
        }
    }
    // check
    return static_cast<int>(AVG/cnt);
}

bool Warshall::pathExist(Node *start, Node *dst)
{
    if(D[start -> getIndex()][dst -> getIndex()] == MY_MAX)
        return false;
    return true;
}
bool Warshall::shortPath(Node *start, Node *dst, std::vector<Node*> path)
{
    // Note: START is not included in the path
    int s = start -> getIndex();
    int d = dst -> getIndex();
    
    if(!pathExist(start, dst))
        return false;
    
    while(s != d)
    {
        s = P[s][d];
        path.push_back(Nodes[s]);
    }
    return true;
}

int Warshall::isNextHop(Node *start, Node *dst)
{
    if(!pathExist(start, dst))
        return -1;
    else
        return P[start -> getIndex()][dst -> getIndex()];
}
