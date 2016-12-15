/*
 * Node.h
 *
 *  Created on: Feb 6, 2016
 *      Author: armin
 */

#ifndef NODE_H_
#define NODE_H_

#include <vector>

class Node {
private:
	int index;
	int owner;
	int armyCount;
	std::vector<Node*> neighbours;
public:
	Node(int index);
	virtual ~Node();

	std::vector<Node*>& getNeighbours(); // get neighbours of this node.

	int getIndex(); // get index of the node. useful when you want to move and you need index of a node.

	/*
	 * get owner of the node. owner will be an integer. if you want to check if
     * this node is yours you can check owner is equal to your team ID. also if
     * you want to check if this node is for you opponent, you can check owner
     * is equal to (1 - your team ID). otherwise this node is empty.
	 */
	int getOwner();

	/**
	 * get number of armies in this node. note that if this node is for your
	 * opponent, then the result of the function is an approximation of the
	 * army count in this node. read contest manual for more details.
	 */
	int getArmyCount();

    void setNeighbours(const std::vector<Node*>& neighbours);

    void setOwner(int owner);

    void setArmyCount(int armyCount);

    void setIndex(int index);
    
    int role = 0; // 1 : attacker, 2: support, 3: transporter
    int status = 0; // for attackers - > 1 : conquare and attack, 2: attack, 3: conquare

};

#endif /* NODE_H_ */
