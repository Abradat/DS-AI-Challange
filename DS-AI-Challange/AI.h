#ifndef _AI_H
#define _AI_H

#include "Node.h"
#include "World.h"

/**
 * AI class.
 * You should fill body of the method {@link #doTurn}.
 * Do not change name or modifiers of the methods or fields
 * and do not add constructor for this class.
 * You can add as many methods or fields as you want!
 * Use world parameter to access and modify game's world!
 * See World interface for more details.
 */
class AI
{
private:
    bool init = true;
    int myTeamId;
    int oppTeamId;
    int escapeConst;
    int nodeBounsConst;
    int edgeBounsConst;
    int lArmyMaxConst;
    int mArmyMaxConst;
    int mCasConst;
    int lCasConst;
    unsigned long int nodesSize;
    int map; // 1 Dota, 2 Eye, 3 Sample, 4 Tiny
    int totalTurn;
    int tactics = 1; // 1 : Attack, 2: hold Position, 3: Retreat
    int a = 0, b = 0, c=0;
    Node *nextNode;
    Node *an;
    
    //calibration
    
    std::vector<Node*> supporters;
    std::vector<Node*> transporters;
    std::vector<Node*> attackers;
    std::vector<Node*> neighbours;
    std::vector<Node*> myNodes;
    
public:
	void doTurn(World *world);
    int measurePower(int armyCount);
    int changeTactics(std::vector<Node*> myNodes);
    int getTactics();
    void decRoles(std::vector<Node*> myNodes);
    void decAttackerStatus(std::vector<Node*> myAttackers);
    void dijkstra(std::vector<Node*> myNodes, std::vector<Node*> supporters, Node *src);
    void printDijkstra(std::vector<Node*> myNode, Node *dst);
    void predictDijkstra(Node *src, Node *dst);
    void toSupporters(World *myWorld, std::vector<Node*> myNodes, std::vector<Node*> supporters, std::vector<Node*> transporters);
    void toAttackers(World *myWorld, std::vector<Node*> supporters, std::vector<Node*> attackers);
    
    void measureTactics(World *myWorld);
    void attack(World *myWorld, std::vector<Node*> myNodes, std::vector<Node*> attackers);


};

#endif // _AI_H
