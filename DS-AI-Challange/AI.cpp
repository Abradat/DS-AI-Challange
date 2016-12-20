#include "AI.h"

#include <vector>
#include <cstdlib>
#include <iostream>
#include "Warshall.hpp"
#include "BfsNode.hpp"
#include "NodeList.hpp"


bool AI::BFS(World *myWorld, Node *start, Node *dst, std::vector<Node*> path)
{
    std::vector<Node*> allNodes = myWorld -> getMap() -> getNodes();
    int nSize = allNodes.size();
    
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
        
        
        
        warshall = new Warshall(world);
        size = world -> getMap() -> getNodes().size();
        NodeList = new NODE_LIST[size];
        for(int  i = 0; i < size; i++)
            NodeList[i] = *new NODE_LIST();
        
        init = false;
    }
    
    // Every Rounds Executeables Start
    
    myNodes = world -> getMyNodes();
    totalNodes = world -> getMap() -> getNodes();
    decRoles(myNodes, totalNodes);
    decAttackerStatus(attackers);
    updateGraph(world, graph);
    
    //toSupporters(world, myNodes, supporters, transporters);
    
    toAttackers(world, supporters, attackers);
    
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
    
    std::cout<< attackers.size() << " attackers"<<std::endl;
    std::cout<< supporters.size() << " supporters" <<std::endl;
    //std::cout<< transporters.size() << " transporters"<< std::endl;
    if(map == 1)
    {
        if(tactics == 1)
        {
            attackers = world -> getMyNodes();
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
            attackStrategy(world, myNodes, attackers);
        }
    }
    if(map == 2)
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

void AI::decRoles(std::vector<Node*> myNodes, std::vector<Node*> totalNodes)
{
    attackers.clear();
    supporters.clear();
    //transporters.clear();
    std::vector<Node*> neighbours;
    
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
    neighbours.clear();
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
        std::cout<< dst-> getArmyCount() << " ";
        return;
    }
    
    printDijkstra(myNodes, dst -> parent );
    std::cout<< dst->getArmyCount() << " ";
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
