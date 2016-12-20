//
//  Warshall.hpp
//  DS-AI-Challange
//
//  Created by Shayan Khorsandi on 12/20/16.
//  Copyright © 2016 Shayan Khorsandi. All rights reserved.
//

#ifndef Warshall_hpp
#define Warshall_hpp

#include "Node.h"
#include "World.h"

class Warshall
{
public:
    // variables Declaring Started
    int size, MY_MAX;
    std::vector<Node*> Nodes;
    int **graph;
    int **D;
    int **P;
    // Variables Declaring Finished
    
    Warshall(World *myWorld);
    int minDistanceFrom(Node *from, std::vector<Node*> list);
    int avgDistanceList(Node *from, std::vector<Node*> list);
    bool pathExist(Node *start, Node *dst);
    bool shortPath(Node *start, Node *dst, std::vector<Node*> path);
    int isNextHop(Node *start, Node *dst);
    
};

#endif /* Warshall_hpp */
