#include "AI.h"

#include <vector>
#include <cstdlib>
#include <iostream>


void AI::doTurn(World *world)
{
    
    srand(time(NULL));
    
    if(init)
    {
        myNodes = world -> getMyNodes();
        attackers = world ->getMyNodes();
        
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
        
        init = false;
    }
    
    myNodes = world -> getMyNodes();
    
    if(map == 1)
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
