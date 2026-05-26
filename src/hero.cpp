#include "hero.h"
#include "enemy.h"
#include <iostream>
#include <algorithm>

int Hero::heroCount_ = 0;


void Warrior::useSpecialAbility(Enemy& target) {
    int debuff = 5 + level_ * 2;
    target.getState().atkDebuff = std::max(target.getState().atkDebuff, debuff);
    std::cout << "  " << name_ << " uses Shield Bash! Enemy ATK -"
              << debuff << " next hit.\n";
}


void Mage::useSpecialAbility(Enemy& target) {
    int fireDmg = level_ * 15;
    target.takeDamage(fireDmg);
    target.getState().burning   = true;
    target.getState().burnTurns = 3;
    manaPoints_ -= 20;
    std::cout << "  " << name_ << " casts Fireball! Deals " << fireDmg
              << " dmg and sets " << target.getName()
              << " ablaze (3 turns).\n";
}


void Rogue::useSpecialAbility(Enemy& target) {
    int poisonPerTurn = 8 + level_ * 2;
    target.getState().poisonDmg   = std::max(target.getState().poisonDmg,
                                             poisonPerTurn);
    target.getState().poisonTurns = 3;
    std::cout << "  " << name_ << " uses Poison Blade! "
              << target.getName() << " takes " << poisonPerTurn
              << " poison dmg/turn for 3 turns.\n";
}
