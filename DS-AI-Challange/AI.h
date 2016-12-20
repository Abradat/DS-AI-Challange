#ifndef _AI_H
#define _AI_H

#include "Node.h"
#include "World.h"
#include "Warshall.hpp"
#include "BfsNode.hpp"
#include "NodeList.hpp"

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
    
    // -------------------------------- OUR CONSTANTS
    int DEFAULT_FRONT_MIN = 5;
    int DEFAULT_RESOURCE_MIN = 1;
    int CLOSE_ENEMY_FL_RM_MIN = 15; // How close is FL to the enemy (MIN)
    int CLOSE_ENEMY_FL_RM_AVG = 2; 	// How close is FL to the enemy (AVG)
    int EMPTINESS_FL_RM  = 3; 		// How empty is the FL
    int RESOURCE_THRESHOLD = 1;
    
    //-------------------- rating attacking nodes
    int VERTEX_DEGREE = 1;
    int ENEMY_NEIGHBOUR = 6;
    int ENEMY_POWER = 5;	// How is the difference between power of us and destination enemy
    int UNDER_ATTACK = 2;	//	destination is under attack by another monster
    int DISTANCE_TO_OUR_UNIT = 1; // nearness to our units
    int MAX_DISTANCE_TO_FRIEND = 7; // maximum distance to our friends
    int NOT_ATTACK_ENEMY = 20;
    double ENEMY_REMAIN = 0.02;	// point based on number of enemy that cannot flee
    double ENEMY_EXISTENCE = 0.02;
    double REMAIN_UNITS = 0.2;	// remain units
    
    // -------------------------------- GlOBAL VARIABLES HERE
    Warshall *warshall;
    NODE_LIST **NodeList;
    unsigned long int size; // or N = number of nodes
    // -------------------------------------------------------
    
    //calibration
    
    std::vector<Node*> supporters;
    //std::vector<Node*> transporters;
    std::vector<Node*> attackers;
    std::vector<Node*> neighbours;
    std::vector<Node*> myNodes;
    std::vector<Node*> totalNodes;
    
    // Graph
    int **graph;
    
public:
    void doTurn(World *world);
    
    bool BFS(World *myWorld, Node *start, Node *dst, std::vector<Node*> *path);
    void getNodesIndexbyRole(int role, std::vector<int> *nodesIndex);
    
    
    
    
    
    
    
    
    int measurePower(int armyCount);
    int changeTactics(std::vector<Node*> myNodes);
    int getTactics();
    void decRoles(std::vector<Node*> myNodes, std::vector<Node*> totalNodes);
    void decAttackerStatus(std::vector<Node*> myAttackers);
    void dijkstra(std::vector<Node*> myNodes, Node *src);
    void dijkstra2(std::vector<Node*> nodes, Node *src);
    void printDijkstra(std::vector<Node*> myNode, Node *dst);
    void predictDijkstra(Node *src, Node *dst);
    void toSupporters(World *myWorld, std::vector<Node*> myNodes, std::vector<Node*> supporters, std::vector<Node*> transporters);
    void toAttackers(World *myWorld, std::vector<Node*> supporters, std::vector<Node*> attackers);
    
    void sort(std::vector<Node*> nodes, int procedure);
    void measureTactics(World *myWorld);
    void attackStrategy(World *myWorld, std::vector<Node*> myNodes, std::vector<Node*> attackers);
    
    void getNextMove(World *myWorld, Node *src, Node *dst);
    void getAttTarget(Node *src);
    
    void updateGraph(World *myWorld, int **myGraph);


};

#endif // _AI_H
