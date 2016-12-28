#include "AI.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include "Warshall.hpp"
#include "BfsNode.hpp"
//#include "NodeList.hpp"
#include <queue>

bool AI::BFS(World *myWorld, Node *start, Node *dst, std::vector<Node*> path)
{
    std::vector<Node*> allNodes = myWorld -> getMap() -> getNodes();
    long nSize = allNodes.size();
    std::queue<int> q;
    
    
    BFS_NODE **nodes = new BFS_NODE*[nSize];
    for(int i = 0; i < nSize; i++)
        nodes[i] = new BFS_NODE();
    
    nodes[dst -> getIndex()] -> distance = 0;
    nodes[dst -> getIndex()] -> parent = dst -> getIndex();
    q.push(dst -> getIndex());
    
    Node *from;
    bool found  = false;
    
    while(q.size()!= 0)
    {
        int front = q.front();
        q.pop();
        from = allNodes[front];
        
        std::vector<Node*> neighbours = from -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if(nodes[neighbour -> getIndex()] -> distance != INT_MAX)
                continue;
            if(start -> getIndex() == neighbour ->getIndex())
                found = true;
            nodes[neighbour -> getIndex()] -> distance = nodes[from -> getIndex()] -> distance + 1;
            nodes[neighbour -> getIndex()] -> parent = from -> getIndex();
            q.push(neighbour -> getIndex());
        }
        if(found)
            break;
        
    }
    if(!found)
        return false;
    from = start;
    do
    {
        path.push_back(allNodes[from -> getIndex()] -> parent);
        from = path.at(path.size() - 1);
    }while(dst -> getIndex() != from -> getIndex());
    
    return true;
}

void AI::getNodesIndexbyRole(int role, std::vector<int> *nodesIndex)
{
    for(int i = 0; i < size; i++)
    {
        if(totalNodes[i] -> role == role)
            nodesIndex -> push_back(i);
    }
    std::cout<<"Size ;:" << nodesIndex->size() << "\n\n";
}

bool AI::isAttackerOnFreePath(World *myWorld, Node *attacker)
{
    std::vector<Node*> neighbours;
    if(attackerDecisions(attacker) <= 1)
    {
        for(auto& neighbour : neighbours)
        {
            if(neighbour -> role == 3 && isPathFree(myWorld, neighbour, myTeamId))
                return true;
        }
    }
    return false;
}

int AI::toAttackerPoint(World *myWorld, int nodeIndex, std::vector<Node*> enemies)
{
    int point = 500;
    Node *attacker = myWorld -> getMap() -> getNode(nodeIndex);
    
    if(isAttackerOnFreePath(myWorld, attacker))
        point -= 1000;
    
    int avgDist = warshall -> avgDistanceList(attacker, enemies);
    int minDist = warshall -> minDistanceFrom(attacker, enemies);
    int req = attacker -> getArmyCount();
    
    point -= avgDist * CLOSE_ENEMY_ATT_SUPP_AVG;
    point -= minDist * CLOSE_ENEMY_ATT_SUPP_MIN;
    point -= req * EMPTINESS_ATT_SUPP;
    
    return point;
    
}

int AI::attackerDecisions(Node *attacker)
{
    std::vector<Node*> neighbours = attacker -> getNeighbours();
    int cnt = 0;
    for(auto& neighbour : neighbours)
    {
        if(neighbour -> role > 2)
            cnt++;
    }
    return cnt;
}

bool AI::relativeNeighbour(Node *myNode)
{
    std::vector<Node*> neighbours = myNode -> getNeighbours();
    for(auto& neighbour : neighbours)
    {
        if(neighbour -> getOwner() == myTeamId)
            return true;
    }
    return false;
}

void AI::sortNodes(std::vector<Node*> Nodes, bool ascending)
{
    
    unsigned long int size = Nodes.size();
    Node *aux;
    if(ascending)
    {
        for(int cnt1 = 0; cnt1 < size; cnt1++)
        {
            for(int cnt2 = cnt1; cnt2 < size; cnt2++)
            {
                if((Nodes[cnt1] -> getArmyCount() < Nodes[cnt2] -> getArmyCount()) != ascending)
                {
                    aux = Nodes[cnt1];
                    Nodes[cnt1] = Nodes[cnt2];
                    Nodes[cnt2] = Nodes[cnt1];
                }
            }
        }
    }
}

int AI::getEnemyPower(Node *enemy)
{
    if(enemy -> getArmyCount() == 0)
        return lArmyMaxConst;
    else if(enemy -> getArmyCount() == 1)
        return mArmyMaxConst;
    else
        return 100;
}

long AI::getMaxMinIndex(std::vector<int> myVec, int situation)
{
    unsigned long int res = myVec[0], resIndex = 0, vecSize = myVec.size();
    for(int cnt = 1; cnt < vecSize; cnt++ )
    {
        if((situation == 1 && myVec[cnt] > res) || (situation == 0 && myVec[cnt] < res))
        {
            res = myVec[cnt];
            resIndex = cnt;
        }
    }
    return resIndex;
}

bool AI::isPathFree(World *myWorld, Node *node, int role)
{
    //bool *marker = new bool[nodesSize];
    bool marker[nodesSize];
    for(int cnt = 0; cnt < nodesSize; cnt++)
        marker[cnt] = false;
    marker[node -> getIndex()] = true;
    std::queue<int> myQ;
    myQ.push(node -> getIndex());
    int head;
    std::vector<Node*> neighbours;
    
    if(node -> getOwner() >= 0 && node -> getOwner() != role)
        return false;
    
    
    while(myQ.size() > 0)
    {
        head = myQ.front();
        myQ.pop();
        
        neighbours = myWorld -> getMap() -> getNode(head) -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if(neighbour -> getOwner() == role)
                continue;
            if(neighbour -> getOwner() >= 0)
                return false;
            else if(!marker[neighbour -> getIndex()])
            {
                marker[neighbour -> getIndex()] = true;
                myQ.push(neighbour -> getIndex());
            }
        }
    }
    return true;
}

int AI::pathToRelative(Node *src, Node *node)
{
    if(node -> getOwner() == myTeamId)
        return 0;
    
    int minDist = warshall -> MY_MAX;
    for(int cnt = 0; cnt < myNodes.size(); cnt++)
    {
        if(warshall -> D[node -> getIndex()][myNodes[cnt] -> getIndex()] < minDist)
        {
            if(myNodes[cnt] -> getIndex() != src -> getIndex())
                minDist = warshall -> D[node -> getIndex()][myNodes[cnt] -> getIndex()];
        }
    }
    return minDist;
}

int AI::Score(Node *src, Node *dst)
{
    int score = 0;
    std::vector<Node*> srcNeighbours = src -> getNeighbours();
    std::vector<Node*> dstNeighbours = dst -> getNeighbours();
    
    score += srcNeighbours.size() * VERTEX_DEGREE;
    
    int myPower;
    if(src -> getArmyCount() <= 10)
        myPower = 0;
    else if(src -> getArmyCount() <= 30)
        myPower = 1;
    else
        myPower = 2;
    
    if(dst -> getOwner() != myTeamId && dst -> getOwner() >= 0)
    {
        if(dst -> getArmyCount() > myPower)
            score -= NOT_ATTACK_ENEMY;
        else
            score += (myPower - dst -> getArmyCount() + 1) * ENEMY_POWER;
    }
    
    bool isFriend = false;
    
    for(auto& srcNeighbour : srcNeighbours)
    {
        if(srcNeighbour -> role <= 1)
            isFriend = true;
    }
    
    if(!isFriend)
    {
        if(MAX_DISTANCE_TO_FRIEND > pathToRelative(src, dst))
            score += (MAX_DISTANCE_TO_FRIEND - pathToRelative(src, dst)) * DISTANCE_TO_OUR_UNIT;
    }
    
    int enemyCounter;
    if(dst -> role == 4)
    {
        enemyCounter = 0;
        for(auto& dstNeighbour : dstNeighbours)
        {
            if(dstNeighbour -> getOwner() >= 0 && dstNeighbour -> getOwner() != myTeamId)
                enemyCounter++;
        }
        
        if(src -> getArmyCount() > getEnemyPower(dst))
            score += (getEnemyPower(dst) - enemyCounter * escapeConst) * ENEMY_REMAIN;
    }
    
    if(dst -> underAttack)
        score -= UNDER_ATTACK;
    
    return score;
}

void AI::supportStrategy(World *myWorld)
{
    std::vector<int> suppList;
    getNodesIndexbyRole(1, &suppList);
    
    std::cout<<"Supp : " << suppList.size();
    if(suppList.size() <= 0)
        return;
    
    
    std::vector<int> enemyIx;
    getNodesIndexbyRole(4, &enemyIx);
    std::vector<Node*> enemy;
    for(int cnt = 0; cnt < enemyIx.size(); cnt++)
        enemy.push_back(myWorld -> getMap() -> getNode(enemyIx[cnt])); // check
    
    
    
    //for(std::vector<int>::iterator supp = suppList.begin(); supp != suppList.end(); supp++)
    //{
    //    D2F = warshall -> D[myWorld -> getMap() -> getNode(supp) -> q[0]][]
    //}
    for(int cnt = 0; cnt < suppList.size(); cnt++)
    {
        int D2F = warshall -> D[myWorld -> getMap() -> getNode(suppList[cnt]) -> q[0]][suppList[cnt]];
        if(D2F <= SUPP_THRESHOLD)
        {
            //int *priority = new int[myWorld -> getMap() -> getNode(suppList[cnt]) -> q.size()];
            std::vector<int> priority;
            for(int cnt2 = 0; cnt2 < myWorld -> getMap() -> getNode(suppList[cnt]) -> q.size(); cnt2++)
                priority.push_back(toAttackerPoint(myWorld, myWorld -> getMap() -> getNode(suppList[cnt]) -> q[cnt2], enemy));
            
            long int maxIdx = getMaxMinIndex(priority, 1); // check
            int dst = myWorld -> getMap() -> getNode(suppList[cnt]) -> q[maxIdx];
            std::cout<<dst;
            std::cout<<"\n";
            int countVal = myWorld -> getMap() -> getNode(suppList[cnt]) -> getArmyCount();
            myWorld -> moveArmy(suppList[cnt], warshall -> isNextHop(myWorld -> getMap() -> getNode(suppList[cnt]),
                                                                     myWorld -> getMap() -> getNode(dst)), countVal);
        }
        
        else
        {
            int countVal = myWorld -> getMap() -> getNode(suppList[cnt]) -> getArmyCount();
            int dst = myWorld -> getMap() -> getNode(suppList[cnt]) -> circulateQueuePoll();
            
            if(isAttackerOnFreePath(myWorld, myWorld -> getMap() -> getNode(dst)))
                dst = myWorld -> getMap() -> getNode(suppList[cnt]) -> circulateQueuePoll();
            myWorld -> moveArmy(suppList[cnt], warshall -> isNextHop(myWorld -> getMap() -> getNode(suppList[cnt]),
                                                                     myWorld -> getMap() -> getNode(dst)), countVal);
            
        }
    }
}

void AI::attackStrategy2(World *myWorld)
{
    std::vector<int> attList;
    getNodesIndexbyRole(2, &attList);
    //std::cout<<"att : " << attList.size() << "\n\n";
    
    std::vector<Node*> neighbours;
    for(int cnt = 0; cnt < attList.size(); cnt++)
    {
        Node *source = myWorld -> getMap() -> getNode(attList[cnt]);
        neighbours = source -> getNeighbours();
        Node *finalDst = nullptr;
        int max = 0, hasEnemy =0;
        for(auto& neighbour : neighbours)
        {
            if(neighbour -> role > 2)
            {
                if(max < Score(source, neighbour))
                {
                    max = Score(source, neighbour);
                    finalDst = neighbour;
                }
            }
            if(neighbour -> role == 4)
                hasEnemy++;
        }
        
        if(finalDst == nullptr)
            continue;
        int armyCount = 0;
        if(isPathFree(myWorld, finalDst, myTeamId))
            armyCount = 1;
        else
        {
            armyCount = source -> getArmyCount();
            armyCount -= hasEnemy * armyCount * ENEMY_EXISTENCE;
            
            if(armyCount > 50)
                armyCount -= armyCount * REMAIN_UNITS;
        }
        
        myWorld -> moveArmy(source, finalDst, armyCount);
        finalDst -> underAttack = true;
    }
}

void AI::doTurn(World *world)
{
    srand(time(NULL));
    
    if(init)
    {
        /*
         //std::cout<<INT_MAX<<std::endl;
         myNodes = world -> getMyNodes();
         //attackers = world ->getMyNodes();
         
         myTeamId = world -> getMyId();
         
         if(myTeamId == 1)
         oppTeamId = 0;
         else
         oppTeamId = 1;
         
         escapeConst = world -> getEscapeConstant();
         nodeBounsConst = world -> getNodeBonusConstant();
         edgeBounsConst = world -> getEdgeBonusConstant();
         lArmyMaxConst = world -> getLowArmyBound();
         mArmyMaxConst = world -> getMediumArmyBound();
         lCasConst = world -> getLowCasualtyCoefficient();
         mCasConst = world -> getMediumCasualtyCoefficient();
         nodesSize = world -> getMap() -> getNodes().size();
         
         if(world -> getMap() ->getNodes().size() == 37)
         map = 1;
         else if(world -> getMap() -> getNodes().size() == 30)
         map = 2;
         else if(world -> getMap() -> getNodes().size() == 14)
         map = 3;
         else
         map = 4;
         
         totalTurn = world -> getTotalTurns();
         tactics = 1;
         
         graph = new int*[world -> getMap() -> getNodes().size()];
         for(int cnt = 0; cnt < world -> getMap() -> getNodes().size(); cnt ++)
         graph[cnt] = new int [world -> getMap() -> getNodes().size()];
         */
        
        
        myTeamId = world -> getMyId();
        
        if(myTeamId == 1)
            oppTeamId = 0;
        else
            oppTeamId = 1;
        
        escapeConst = world -> getEscapeConstant();
        nodeBounsConst = world -> getNodeBonusConstant();
        edgeBounsConst = world -> getEdgeBonusConstant();
        lArmyMaxConst = world -> getLowArmyBound();
        mArmyMaxConst = world -> getMediumArmyBound();
        lCasConst = world -> getLowCasualtyCoefficient();
        mCasConst = world -> getMediumCasualtyCoefficient();
        nodesSize = world -> getMap() -> getNodes().size();
        
        
        warshall = new Warshall(world);
        size = world -> getMap() -> getNodes().size();
        //NodeList = new NODE_LIST*[size];
        //for(int  i = 0; i < size; i++)
         //   NodeList[i] = new NODE_LIST();
        
        init = false;
    }
    
    try{
        
        //for(auto& goh : totalNodes)
        //    std::cout<<goh -> role << " ";
        //std::cout<<"\n\n";
        decRoles(world);
        //std::cout<< "Total : " <<totalNodes.size() << "\n" << "MyNodes : "<< myNodes.size() << "\n" << "Att: " << attackers.size() << "\n" << "Supp : " << supporters.size() << "\n\n\n#####\n\n";
        supportStrategy(world);
        attackStrategy2(world);
    }
    catch(std::exception e)
    {
        
    }
    // Every Rounds Executeables Start
    
    /*myNodes = world -> getMyNodes();
     totalNodes = world -> getMap() -> getNodes();
     //decRoles(myNodes, totalNodes);
     decAttackerStatus(attackers);
     updateGraph(world, graph);
     */
    //toSupporters(world, myNodes, supporters, transporters);
    
    //toAttackers(world, supporters, attackers);
    
    //for(auto& test: myNodes)
    //{
    //   if(test -> getIndex() == 0)
    //       dijkstra(myNodes, supporters, test);
    //}
    //dijkstra(myNodes, supporters, test);
    /*for(auto& myNode : myNodes)
     {
     std::cout<< myNode->getArmyCount() << " : " << myNode -> dist << std::endl;
     }
     std::cout<<"#######\n\n\n";
     */
    /*
     for(auto& test: myNodes)
     {
     if(test -> getIndex() == 0)
     an = test;
     }
     
     for(auto& myNode: myNodes)
     {
     if(myNode == an)
     continue;
     
     printDijkstra(myNodes, myNode);
     std::cout<<"\n";
     predictDijkstra(an, myNode);
     std::cout<<nextNode -> getArmyCount() <<"\n";
     }
     
     std::cout<<"\n\n\n\n######\n";
     */
    //Every Round Executables Finish
    
    //Debugging Prints Start
    /*
     std::cout<< attackers.size() << " attackers"<<std::endl;
     std::cout<< supporters.size() << " supporters" <<std::endl;
     std::cout<< transporters.size() << " transporters"<< std::endl;
     a =0, b=0, c=0;
     for(auto& attacker : attackers)
     {
     if(attacker -> status == 1)
     a += 1;
     else if(attacker -> status == 2)
     b += 1;
     else if(attacker -> status == 3)
     c += 1;
     }
     std::cout<< a << " conq att\n";
     std::cout<< b <<" att\n";
     std::cout<< c <<" conq";
     std:: cout<<"\n\n\n\n";
     */
    //Debugging Prints Finish
    
    //std::cout<< attackers.size() << " attackers"<<std::endl;
    //std::cout<< supporters.size() << " supporters" <<std::endl;
    //std::cout<< transporters.size() << " transporters"<< std::endl;
    //if(map == 1)
    //{
    //    if(tactics == 1)
    //    {
    //        attackers = world -> getMyNodes();
    /*for(auto& attacker : attackers)
     {
     int attackerPower = attacker -> getArmyCount();
     int aMeasured = measurePower(attackerPower);
     neighbours = attacker -> getNeighbours();
     for(auto& neighbour : neighbours)
     {
     if(neighbour -> getOwner() == -1 || neighbour -> getOwner() == oppTeamId)
     {
     if(attackerPower > 40)
     world -> moveArmy(attacker, neighbour, attackerPower - 1);
     else if(attackerPower > 30 )
     world -> moveArmy(attacker, neighbour, 20);
     else if(attackerPower > 20)
     world -> moveArmy(attacker, neighbour, 10);
     else if(attackerPower > 15)
     world -> moveArmy(attacker, neighbour, 5);
     //else
     //    world -> moveArmy(attacker, neighbour, 3);
     }
     }
     }
     */
    //      attackStrategy(world, myNodes, attackers);
    //  }
    //}
    /*if(map == 2)
     {
     if(tactics == 1)
     {
     attackers = world -> getMyNodes();
     for(auto& attacker : attackers)
     {
     int attackerPower = attacker -> getArmyCount();
     int aMeasured = measurePower(attackerPower);
     neighbours = attacker -> getNeighbours();
     for(auto& neighbour : neighbours)
     {
     if(neighbour -> getOwner() == -1 || neighbour -> getOwner() == oppTeamId)
     {
     if(attackerPower > 40)
     world -> moveArmy(attacker, neighbour, attackerPower/2);
     else if(attackerPower > 30 )
     world -> moveArmy(attacker, neighbour, 20);
     else if(attackerPower > 20)
     world -> moveArmy(attacker, neighbour, 10);
     else if(attackerPower > 15)
     world -> moveArmy(attacker, neighbour, 5);
     else
     world -> moveArmy(attacker, neighbour, 3);
     }
     }
     }
     }
     }
     */
    /*for(auto& myNode: myNodes)
     {
     std::cout<<myNode->getIndex() << " : " << myNode->getArmyCount()<<std::endl;
     }
     std::cout<<"#####"<<std::endl<<std::endl;
     */
    
}

int AI::measurePower(int armyCount)
{
    
    if(armyCount > 30)
        return 2;
    else if(armyCount <= 30 && armyCount > 10)
        return 1;
    else
        return 0;
}

void AI::decRoles(World *myWorld)
{
    attackers.clear();
    supporters.clear();
    newAttackers.clear();
    newSupporters.clear();
    std::vector<Node*> neighbours;
    bool flag;
    
    myNodes = myWorld -> getMyNodes();
    totalNodes = myWorld -> getMap() -> getNodes();
    
    
    for(auto& node : totalNodes)
    {
        if(node -> getOwner() == -1)
            node -> role = 3;
        else if(node -> getOwner() == oppTeamId)
            node -> role = 4;
        else
        {
            flag = true; // assumes the node is support
            neighbours = node -> getNeighbours();
            for(auto& neighbour : neighbours)
            {
                if(neighbour -> getOwner() != myTeamId)
                {
                    flag = false;
                    break;
                }
            }
            if(flag)
                node -> role = 1;
            else
                node -> role = 2;
        }
        
        if(node -> role == 1)
            supporters.push_back(node);
        else if(node -> role == 2)
            attackers.push_back(node);
        
        
    }
    
    for(auto& attacker : attackers)
    {
        if(!isAttackerOnFreePath(myWorld, attacker))
            newAttackers.push_back(attacker);
        
    }
    
    attackers.clear();
    for(auto& newAttacker : newAttackers)
        attackers.push_back(newAttacker);
    
    for(auto& supporter : supporters)
    {
        int minF = warshall -> minDistanceFrom(supporter, attackers);
        if(minF < SUPP_THRESHOLD)
            minF = SUPP_THRESHOLD;
        
        std::vector<int>newQ;
        for(auto& attacker : attackers)
        {
            if(warshall -> D[supporter -> getIndex()][attacker -> getIndex()] <= minF)
                newQ.push_back(attacker -> getIndex());
            supporter -> myMerge(newQ);
        }
        
    }
    
    //transporters.clear();
    /*
     for(auto& myNode : myNodes)
     myNode -> role = 0;
     
     for (auto& myAttacker : myNodes)
     {
     neighbours = myAttacker -> getNeighbours();
     for(auto& neighbour : neighbours)
     {
     if(neighbour -> getOwner() != myTeamId)
     {
     myAttacker-> role = 1;
     attackers.push_back(myAttacker);
     break;
     }
     }
     }
     */
    //neighbours.clear();
    /*
     for(auto &mySupporter : attackers)
     {
     neighbours = mySupporter -> getNeighbours();
     for(auto & neighbour : neighbours)
     {
     if(neighbour -> role != 1 && neighbour -> role != 2 && neighbour -> getOwner() == myTeamId)
     {
     neighbour -> role = 2;
     supporters.push_back(neighbour);
     }
     }
     }
     neighbours.clear();
     for(auto& myTransporter : myNodes)
     {
     if(myTransporter-> role != 1 && myTransporter -> role != 2)
     {
     myTransporter -> role = 3;
     transporters.push_back(myTransporter);
     }
     }
     neighbours.clear();
     */
    /*
     for(auto& mySupporter : myNodes)
     {
     if(mySupporter -> role != 1)
     {
     mySupporter -> role = 2;
     supporters.push_back(mySupporter);
     }
     
     }
     
     for(auto& node : totalNodes)
     {
     if(node -> getOwner() == -1)
     node -> role = 2;
     else if( node -> getOwner() == oppTeamId)
     node -> role = 3;
     }
     */
    
    
}

void AI::decAttackerStatus(std::vector<Node*> myAttackers)
{
    int conq, att;
    for(auto& myAttacker : myAttackers)
    {
        conq = 0, att = 0;
        myAttacker -> status = 0;
        std::vector<Node*> neighbours = myAttacker -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if(neighbour -> getOwner() == oppTeamId )
                att = 1;
            else if(neighbour -> getOwner() == -1)
                conq = 1;
        }
        if(att == 1 && conq == 1)
            myAttacker -> status = 1;
        else if(att == 1 && conq == 0)
            myAttacker -> status = 2;
        else if(att == 0 && conq == 1)
            myAttacker -> status = 3;
    }
}

void AI::dijkstra(std::vector<Node*> myNodes, Node *src)
{
    for(auto& myNode: myNodes)
        myNode -> dist = INT_MAX, myNode -> sptSet = false, myNode -> parent = nullptr;
    
    src -> dist = 0;
    int min = INT_MAX;
    Node *pickedNode = nullptr;
    std::vector<Node*> neighbours;
    
    for(auto& myNode : myNodes)
    {
        //if(myNode == src)
        //    continue;
        min = INT_MAX;
        for(auto &mNode : myNodes)
        {
            if(!mNode -> sptSet)
            {
                if(min > mNode -> dist)
                {
                    min = mNode -> dist;
                    pickedNode = mNode;
                }
            }
        }
        //std::cout<<pickedNode -> getArmyCount()<<std::endl;
        pickedNode -> sptSet = true;
        neighbours = pickedNode -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if(!neighbour -> sptSet && pickedNode -> dist != INT_MAX && (pickedNode -> dist + 1 < neighbour -> dist) && neighbour ->getOwner() == myTeamId)
            {
                neighbour -> dist = pickedNode -> dist + 1;
                neighbour -> parent = pickedNode;
            }
        }
    }
    
}

void AI::dijkstra2(std::vector<Node*> nodes, Node *src)
{
    for(auto& node : nodes)
        node -> dist = INT_MAX, node -> sptSet = false, node -> parent = nullptr;
    
    src -> dist = 0;
    int min = INT_MAX;
    Node *pickedNode = nullptr;
    std::vector<Node*> neighbours;
    for(auto& node : nodes)
    {
        min = INT_MAX;
        for(auto& mNode : nodes)
        {
            if(!mNode -> sptSet)
            {
                if(min > mNode -> dist)
                {
                    min = mNode -> dist;
                    pickedNode = mNode;
                }
            }
        }
        pickedNode -> sptSet = true;
        neighbours = pickedNode -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if(!neighbour -> sptSet && pickedNode -> dist != INT_MAX && (pickedNode -> dist + 1 < neighbour -> dist))
            {
                neighbour -> dist = pickedNode -> dist + 1;
                neighbour -> parent = pickedNode;
            }
        }
    }
}

void AI::printDijkstra(std::vector<Node*> myNodes, Node *dst)
{
    if( dst -> parent == nullptr )
    {
        //std::cout<< dst-> getArmyCount() << " ";
        return;
    }
    
    printDijkstra(myNodes, dst -> parent );
    //std::cout<< dst->getArmyCount() << " ";
}

void AI::predictDijkstra(Node *src, Node *dst)
{
    Node *aux;
    aux = dst;
    if(aux -> parent == nullptr)
        return;
    else
    {
        while(aux -> parent != src)
            aux = aux -> parent;
        nextNode = aux;
    }
}

void AI::toSupporters(World *myWorld, std::vector<Node*> myNodes, std::vector<Node*> supporters, std::vector<Node*> transporters)
{
    int min = INT_MAX;
    Node *aux = nullptr;
    nextNode = nullptr;
    for(auto& transporter : transporters)
    {
        dijkstra(myNodes, transporter);
        min = INT_MAX;
        aux = nullptr;
        //std::cout<< transporter -> getArmyCount() << " : \n";
        for(auto& supporter : supporters)
        {
            //std::cout<<supporter->getArmyCount() << " ";
            if(min > supporter -> dist)
            {
                min = supporter -> dist;
                aux = supporter;
            }
        }
        //std::cout<<"\n\n";
        //printDijkstra(myNodes, transporter);
        //std::cout<<"\n";
        predictDijkstra(transporter, aux);
        //if(transporter ->getArmyCount() > 5)
        myWorld -> moveArmy(transporter, nextNode, transporter->getArmyCount() - 1);
    }
    //std::cout<<"\n\n\n\n###########\n";
}

void AI::toAttackers(World *myWorld, std::vector<Node*> supporters, std::vector<Node*> attackers)
{
    std::vector<Node*> neighbours;
    std::vector<Node*> attNeighbours;
    for(auto& supporter : supporters)
    {
        attNeighbours.clear();
        neighbours = supporter -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if( neighbour -> role == 1)
                attNeighbours.push_back(neighbour);
        }
        for(auto& attNeighbour : attNeighbours)
            myWorld -> moveArmy(supporter, attNeighbour, ((supporter -> getArmyCount()) / static_cast<int>(attNeighbours.size()))-1);
    }
}

void AI::measureTactics(World *myWorld)
{
    
}

void AI::sort(std::vector<Node*> nodes, int procedure)
{
    Node *aux;
    if(procedure == 1) // ascending
    {
        
    }
    else
    {
        for(int nodeCnt1 = 0; nodeCnt1 < nodes.size(); nodeCnt1++)
        {
            for(int nodeCnt2 = nodeCnt1 ; nodeCnt2 < nodes.size(); nodeCnt2++)
            {
                if(nodes[nodeCnt1] -> dist < nodes[nodeCnt2] -> dist)
                {
                    aux = nodes[nodeCnt1];
                    nodes[nodeCnt1] = nodes[nodeCnt2];
                    nodes[nodeCnt2] = aux;
                }
            }
        }
    }
}

void AI::attackStrategy(World *myWorld, std::vector<Node*> myNodes, std::vector<Node*> attackers)
{
    std::vector<Node*> neighbours;
    std::vector<Node*> opps;
    std::vector<Node*> allNodes;
    Node *aux;
    int selfPower, oppPower, oppCounts, suppPower, suppCount, attPower, attCount, freeCount;
    for(auto& attacker : attackers)
    {
        selfPower = 0, oppPower = 0, oppCounts = 0, suppPower = 0, suppCount = 0, attCount = 0, freeCount = 0;
        attacker -> freeNodes.clear(), attacker -> suppNodes.clear(), attacker -> oppNodes.clear();
        selfPower = measurePower(attacker -> getArmyCount());
        neighbours = attacker -> getNeighbours();
        
        allNodes = myWorld -> getMap() -> getNodes();
        //opps.push_back(attacker);
        dijkstra2(allNodes, attacker);
        opps = myWorld -> getOpponentNodes();
        //sorting Start
        //sort(opps, 1);
        std::cout<<"From " << attacker -> getArmyCount() << " : \n";
        for(auto& opp: opps)
            std::cout<<opp->dist << " ";
        std::cout<<"\n\n#######\n\n\n";
        //sorting finish
        attacker -> oppNodes = opps;
        getAttTarget(attacker);
        predictDijkstra(attacker, attacker -> target);
        myWorld -> moveArmy(attacker, nextNode , attacker -> getArmyCount());
        
        
    }
}

void AI::getAttTarget(Node *src)
{
    int min = INT_MAX;
    Node *target = nullptr;
    for(auto& opp : src -> oppNodes)
    {
        if(min > opp -> dist)
        {
            target = opp;
            min = opp -> dist;
        }
    }
    src -> target = target;
}

void AI::getNextMove(World *myWorld, Node *src, Node *dst)
{
    
}

void AI::updateGraph(World *myWrold, int **myGraph)
{
    // for opp : -1 , free node : 1, att node : 2, supp node : 3, trans node : 4
    std::vector<Node*> nodes = myWrold -> getMap() -> getNodes();
    std::vector<Node*> neighbours;
    
    for(int cnt1 = 0; cnt1 < nodesSize; cnt1++)
    {
        for(int cnt2 = 0; cnt2 < nodesSize; cnt2++)
            myGraph[cnt1][cnt2] = 0;
    }
    for(auto& node : nodes)
    {
        neighbours = node -> getNeighbours();
        for(auto& neighbour : neighbours)
        {
            if(neighbour -> getOwner() == oppTeamId)
                myGraph[node -> getIndex()][neighbour -> getIndex()] = -1;
            else if( neighbour -> getOwner() == -1)
                myGraph[node -> getIndex()][neighbour -> getIndex()] = 1;
            else if( neighbour -> role == 1)
                myGraph[node -> getIndex()][neighbour -> getIndex()] = 2;
            else if( neighbour -> role == 2)
                myGraph[node -> getIndex()][neighbour -> getIndex()] = 3;
            else if(neighbour -> role == 3)
                myGraph[node -> getIndex()][neighbour -> getIndex()] = 4;
        }
    }
    
    /*for (int cnt1 = 0; cnt1 < nodesSize; cnt1++)
     {
     for(int cnt2 =0; cnt2 < nodesSize; cnt2++)
     std::cout<<myGraph[cnt1][cnt2] << " ";
     std::cout<<"\n";
     }
     std::cout<<"\n";
     */
}
