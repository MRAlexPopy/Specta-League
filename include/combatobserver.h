#ifndef OOP_COMBATOBSERVER_H
#define OOP_COMBATOBSERVER_H

#include <string>


class CombatObserver {
public:
    virtual ~CombatObserver() = default;

   
    virtual void onHeroDied(const std::string& heroName) = 0;

   
    virtual void onEnemyDefeated(const std::string& enemyName, int xpGained) = 0;

   
    virtual void onRoundStart(int roundNumber) = 0;
};

#endif
