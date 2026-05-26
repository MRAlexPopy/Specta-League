#include "strategy.h"
#include "hero.h"
#include "enemy.h"
#include "party.h"
#include <iostream>
#include <algorithm>


bool AggressiveStrategy::executeTurn(Hero& hero, Enemy& enemy,
                                     Party& /*party*/) {
    ++turnsSinceSpecial_;
    if (turnsSinceSpecial_ >= 3) {
        turnsSinceSpecial_ = 0;
        hero.useSpecialAbility(enemy);
        return true;
    }
    int dmg = hero.attack();
    
    auto* skeleton = dynamic_cast<Skeleton*>(&enemy);
    if (skeleton) {
        skeleton->takeDamageBlocked(dmg);
    } else {
        enemy.takeDamage(dmg);
    }
    std::cout << "attacks for " << dmg << " dmg. ";
    std::cout << "[Enemy HP: " << enemy.getHp() << "]\n";
    return true;
}


bool DefensiveStrategy::executeTurn(Hero& hero, Enemy& enemy, Party& party) {
    ++turnsSinceSpecial_;

    
    float hpRatio = static_cast<float>(hero.getHp()) /
                    static_cast<float>(hero.getMaxHp());
    if (hpRatio < 0.40f && !party.getPotions().isEmpty()) {
        try {
            Potion p = party.getPotions().removeItem(0);
            hero.heal(p.getHealAmount());
            std::cout << "heals with " << p.getName() << " (+"
                      << p.getHealAmount() << " HP). ";
            std::cout << "[Hero HP: " << hero.getHp() << "]\n";
            return false;
        } catch (const InvalidSelectionException&) {
            
        }
    }

    if (turnsSinceSpecial_ >= 5) {
        turnsSinceSpecial_ = 0;
        hero.useSpecialAbility(enemy);
        return true;
    }

    int dmg = hero.attack();
    auto* skeleton = dynamic_cast<Skeleton*>(&enemy);
    if (skeleton) {
        skeleton->takeDamageBlocked(dmg);
    } else {
        enemy.takeDamage(dmg);
    }
    std::cout << "attacks for " << dmg << " dmg. ";
    std::cout << "[Enemy HP: " << enemy.getHp() << "]\n";
    return true;
}


bool TacticalStrategy::executeTurn(Hero& hero, Enemy& enemy, Party& party) {
    ++turnCounter_;

    float hpRatio = static_cast<float>(hero.getHp()) /
                    static_cast<float>(hero.getMaxHp());
    if (hpRatio < 0.30f && !party.getPotions().isEmpty()) {
        try {
            Potion p = party.getPotions().removeItem(0);
            hero.heal(p.getHealAmount());
            std::cout << "heals with " << p.getName() << " (+"
                      << p.getHealAmount() << " HP). ";
            std::cout << "[Hero HP: " << hero.getHp() << "]\n";
            return false;
        } catch (const InvalidSelectionException&) {}
    }

    if (turnCounter_ % 3 == 0) {
        hero.useSpecialAbility(enemy);
        auto* dragon = dynamic_cast<Dragon*>(&enemy);
        if (dragon && dragon->hasCombustionSynergy()) {
            const int combustionDmg = 15;
            dragon->takeDamage(combustionDmg);
            std::cout << "  *** COMBUSTION SYNERGY! +"
                      << combustionDmg << " extra dmg! ***\n";
        }
        return true;
    }

    int dmg = hero.attack();
    auto* skeleton = dynamic_cast<Skeleton*>(&enemy);
    if (skeleton) {
        skeleton->takeDamageBlocked(dmg);
    } else {
        enemy.takeDamage(dmg);
    }
    std::cout << "attacks for " << dmg << " dmg. ";
    std::cout << "[Enemy HP: " << enemy.getHp() << "]\n";
    return true;
}
