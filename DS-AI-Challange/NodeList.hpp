//
//  NodeList.hpp
//  DS-AI-Challange
//
//  Created by Shayan Khorsandi on 12/20/16.
//  Copyright © 2016 Shayan Khorsandi. All rights reserved.
//

#ifndef NodeList_hpp
#define NodeList_hpp

#include "Node.h"
#include "World.h"

class NODE_LIST
{
public:
    Node *node; // contains index
    int type;
    std::vector<int> q;
    bool underAttack;
    
    NODE_LIST();
    int circulateQueuePoll();
    void myMerge(std::vector<int>newF);
};

#endif /* NodeList_hpp */
