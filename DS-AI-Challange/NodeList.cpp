//
//  NodeList.cpp
//  DS-AI-Challange
//
//  Created by Shayan Khorsandi on 12/20/16.
//  Copyright © 2016 Shayan Khorsandi. All rights reserved.
//

#include "NodeList.hpp"
#include <vector>
#include <cstdlib>
#include <iostream>

NODE_LIST::NODE_LIST()
{
    underAttack = false;
}

int NODE_LIST::circulateQueuePoll()
{
    int a = -1;
    if(q.size() > 0)
    {
        a = q[0];
        q.erase(q.begin());
        q.push_back(a);
    }
    return a;
}

void NODE_LIST::myMerge(std::vector<int> newF)
{
    std::vector<int> newQ;
    for(int i = 0; i < q.size(); i++)
    {
        if(std::find(newF.begin(), newF.end(), q[i]) != newF.end())
            newQ.push_back(q[i]);
    }
    
    for(int i = 0; i < newF.size(); i++)
    {
        if(std::find(newQ.begin(), newQ.end(), newQ[i]) != newQ.end())
        {
            continue;
        }
        else
            newQ.push_back(newF[i]);
    }
    q = newQ;
}
