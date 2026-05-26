#include "enemy.h"
#include <iostream>
#include <algorithm>


void Enemy::onTurnEnd() {
    if (state_.burning && state_.burnTurns > 0) {
        const int burnDmg = 5;
        hp_ = std::max(0, hp_ - burnDmg);
        --state_.burnTurns;
        std::cout << "  >> " << name_ << " takes " << burnDmg
                  << " burn damage (" << state_.burnTurns << " turns left).\n";
        if (state_.burnTurns == 0) state_.burning = false;
    }

    
    if (state_.poisonTurns > 0) {
        hp_ = std::max(0, hp_ - state_.poisonDmg);
        --state_.poisonTurns;
        std::cout << "  >> " << name_ << " takes " << state_.poisonDmg
                  << " poison damage (" << state_.poisonTurns
                  << " turns left).\n";
        if (state_.poisonTurns == 0) state_.poisonDmg = 0;
    }
}
