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
public:
	void doTurn(World *world);

private:
    World *myWorld;
    const int myTeamId = myWorld->getMyId();
    const int escapeConst = myWorld -> getEscapeConstant();
    const int nodeBounsConst = myWorld -> getNodeBonusConstant();
    const int edgeBounsConst = myWorld -> getEdgeBonusConstant();
    const int lArmyMaxConst = myWorld -> getLowArmyBound();
    const int mArmyMaxConst = myWorld -> getMediumArmyBound();
    const int mCasConst = myWorld -> getMediumCasualtyCoefficient();
    const int lCasConst = myWorld -> getLowCasualtyCoefficient();
};

#endif // _AI_H
