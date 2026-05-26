#ifndef OOP_COMBATSYSTEM_H
#define OOP_COMBATSYSTEM_H

#include "party.h"
#include "enemy.h"
#include "enemyfactory.h"
#include "exceptions.h"
#include <iostream>
#include <memory>
#include <limits>


class CombatSystem {
    Party&           party_;
    CombatObserver&  observer_;
    int              difficulty_ = 0;

public:
    explicit CombatSystem(Party& party, CombatObserver& observer,
                          int difficulty = 0)
        : party_{party}, observer_{observer}, difficulty_{difficulty} {}

    CombatSystem(const CombatSystem&) = delete;
    CombatSystem& operator=(const CombatSystem&) = delete;
    ~CombatSystem() = default;

    void setDifficulty(int d) { difficulty_ = d; }

    
    bool runBattle(EnemyFactory::EnemyType type = EnemyFactory::EnemyType::RANDOM) {
        if (party_.isWiped()) throw EmptyPartyException{};
        auto enemy = EnemyFactory::create(type, difficulty_);
        printBattleHeader(*enemy);

        int round = 0;
        while (enemy->isAlive() && !party_.isWiped()) {
            ++round;
            observer_.onRoundStart(round);
            std::cout << "\n[Round " << round << "]\n";

            for (int i = 0; i < party_.size(); ++i) {
                Hero* hero = party_.getMember(i);
                if (!hero || !hero->isAlive()) continue;

                printHeroTurnHeader(*hero, *enemy);
                doPlayerTurn(hero, *enemy);
                if (!enemy->isAlive()) break;
            }

            if (!enemy->isAlive()) break;
            doEnemyTurn(*enemy);
            enemy->onTurnEnd();
        }
        return resolveBattle(*enemy);
    }

  
    bool runAutoBattle(EnemyFactory::EnemyType type = EnemyFactory::EnemyType::RANDOM) {
        if (party_.isWiped()) throw EmptyPartyException{};
        auto enemy = EnemyFactory::create(type, difficulty_);
        printBattleHeader(*enemy);

        int round = 0;
        while (enemy->isAlive() && !party_.isWiped()) {
            ++round;
            observer_.onRoundStart(round);
            std::cout << "\n[Round " << round << "]\n";

            for (int i = 0; i < party_.size(); ++i) {
                Hero* hero = party_.getMember(i);
                if (!hero || !hero->isAlive()) continue;

                std::cout << "  " << hero->getName()
                          << " [" << hero->getStrategy()->getName() << "]: ";
                
                hero->executeTurn(*enemy, party_);

                if (!enemy->isAlive()) break;
            }

            if (!enemy->isAlive()) break;
            doEnemyTurn(*enemy);
            enemy->onTurnEnd();
        }
        return resolveBattle(*enemy);
    }

private:
    static void printBattleHeader(const Enemy& enemy) {
        std::cout << "\n╔══════════════════════════════════════╗\n";
        std::cout << "║          ⚔  BATTLE START  ⚔          ║\n";
        std::cout << "╚══════════════════════════════════════╝\n";
        std::cout << "Opponent: " << enemy << "\n\n";
    }

    static void printHeroTurnHeader(const Hero& hero, const Enemy& enemy) {
        std::cout << "\n--- " << hero.getName() << "'s turn ---\n";
        std::cout << "  Enemy: " << enemy << "\n";
    }

    void doPlayerTurn(Hero* hero, Enemy& enemy) {
        std::cout << "  [a] Attack  [s] Special  [p] Potion\n  > ";
        char choice{};
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 'a': case 'A':
                doAttack(hero, enemy);
                break;
            case 's': case 'S':
                doSpecial(hero, enemy);
                break;
            case 'p': case 'P':
                usePotion(hero);
                break;
            default:
                std::cout << "  Unknown — defaulting to Attack.\n";
                doAttack(hero, enemy);
        }
    }

   static void doAttack(const Hero* hero, Enemy& enemy) {
        
        auto* skeleton = dynamic_cast<Skeleton*>(&enemy);
        int dmg = hero->attack();
        if (skeleton) {
            skeleton->takeDamageBlocked(dmg);
        } else {
            enemy.takeDamage(dmg);
        }
        std::cout << "  " << hero->getName() << " attacks for " << dmg
                  << " damage!\n";
    }

    static void doSpecial(Hero* hero, Enemy& enemy) {
        hero->useSpecialAbility(enemy);

        
        auto* dragon = dynamic_cast<Dragon*>(&enemy);
        if (dragon && dragon->hasCombustionSynergy()) {
            int combustionDmg = 15;
            dragon->takeDamage(combustionDmg);
            std::cout << "  *** COMBUSTION SYNERGY! Dragon takes extra "
                      << combustionDmg << " damage! ***\n";
        }
    }

    void usePotion(Hero* hero) {
        Inventory<Potion>& potions = party_.getPotions();
        if (potions.isEmpty()) {
            std::cout << "  No potions available!\n";
            return;
        }
        potions.display(std::cout);
        std::cout << "  Choose potion [1-" << potions.size() << "]: ";
        int idx{};
        std::cin >> idx;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        try {
            Potion p = potions.removeItem(idx - 1);
            hero->heal(p.getHealAmount());
            std::cout << "  " << hero->getName() << " uses " << p.getName()
                      << " and restores " << p.getHealAmount() << " HP!\n";
        } catch (const InvalidSelectionException& e) {
            std::cout << "  " << e.what() << "\n";
        }
    }

    void doEnemyTurn(Enemy& enemy) {
        
        const auto* dragon = dynamic_cast<const Dragon*>(&enemy);
        if (dragon && dragon->isBreathCharged())
            std::cout << "\n  !! " << enemy.getName()
                      << " unleashes DRAGON BREATH!\n";

        int dmg = enemy.attack();
        for (int i = 0; i < party_.size(); ++i) {
            Hero* hero = party_.getMember(i);
            if (!hero || !hero->isAlive()) continue;
            hero->takeDamage(dmg);
            std::cout << "  " << enemy.getName() << " attacks "
                      << hero->getName() << " for " << dmg << " dmg! ("
                      << hero->getHp() << "/" << hero->getMaxHp() << " HP)\n";
            if (!hero->isAlive()) {
                std::cout << "  " << hero->getName() << " has been slain!\n";
                observer_.onHeroDied(hero->getName()); 
            }
            break; 
        }

        
        enemy.getState().atkDebuff = 0;
    }

       bool resolveBattle(const Enemy& enemy) {
        if (!enemy.isAlive()) {
            std::cout << "\n  *** VICTORY! " << enemy.getName()
                      << " defeated! ***\n";
            observer_.onEnemyDefeated(enemy.getName(), enemy.getXpReward());
            levelUpSurvivors();
            return true;
        }
        std::cout << "\n  *** DEFEAT! Your party has been wiped out. ***\n";
        return false;
    }

    void levelUpSurvivors() {
        for (int i = 0; i < party_.size(); ++i) {
            Hero* h = party_.getMember(i);
            if (h && h->isAlive()) {
                h->levelUp();
                std::cout << "  " << h->getName()
                          << " reached level " << h->getLevel() << "!\n";
            }
        }
    }
};

#endif
