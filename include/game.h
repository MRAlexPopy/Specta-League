#ifndef OOP_GAME_H
#define OOP_GAME_H

#include "heropool.h"
#include "party.h"
#include "combatsystem.h"
#include "item.h"
#include "exceptions.h"
#include <iostream>
#include <set>
#include <string>
#include <limits>


class Game {
    Party party_;
    int   difficulty_ = 0;
    bool  running_    = true;

    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

public:
    static Game& getInstance() {
        static Game instance;
        return instance;
    }

    void run() {
        printBanner();
        draftPhase();
        setupInventory();
        mainMenuLoop();
        printFinalStats();
        std::cout << "\nThanks for playing Specta League!\n";
    }

private:
    
    void printBanner() const {
        std::cout << "\n";
        std::cout << "  ██████╗ ██████╗ ███████╗ ██████╗████████╗ █████╗ \n";
        std::cout << "  ██╔════╝ ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗\n";
        std::cout << "  ███████╗ ██████╔╝█████╗  ██║        ██║   ███████║\n";
        std::cout << "  ╚════██║ ██╔═══╝ ██╔══╝  ██║        ██║   ██╔══██║\n";
        std::cout << "  ███████║ ██║     ███████╗╚██████╗   ██║   ██║  ██║\n";
        std::cout << "  ╚══════╝ ╚═╝     ╚══════╝ ╚═════╝   ╚═╝   ╚═╝  ╚═╝\n";
        std::cout << "  ██╗     ███████╗ █████╗  ██████╗ ██╗   ██╗███████╗\n";
        std::cout << "  ██║     ██╔════╝██╔══██╗██╔════╝ ██║   ██║██╔════╝\n";
        std::cout << "  ██║     █████╗  ███████║██║  ███╗██║   ██║█████╗  \n";
        std::cout << "  ██║     ██╔══╝  ██╔══██║██║   ██║██║   ██║██╔══╝  \n";
        std::cout << "  ███████╗███████╗██║  ██║╚██████╔╝╚██████╔╝███████╗\n";
        std::cout << "  ╚══════╝╚══════╝╚═╝  ╚═╝ ╚═════╝  ╚═════╝ ╚══════╝\n";
        std::cout << "\n  Welcome, Commander. Assemble your party!\n\n";
    }

    
    void draftPhase() {
        HeroPool pool;
        pool.display(std::cout);

        std::cout << "\nDraft 3 heroes (enter 3 numbers 1-"
                  << pool.size() << ", space separated): ";
        std::set<int> chosen;
        while (static_cast<int>(chosen.size()) < 3) {
            int pick{};
            if (!(std::cin >> pick)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid — try again: ";
                continue;
            }
            if (pick < 1 || pick > pool.size()) {
                std::cout << "Must be 1-" << pool.size() << ": ";
                continue;
            }
            if (chosen.count(pick)) {
                std::cout << "Already picked. Choose another: ";
                continue;
            }
            chosen.insert(pick);
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (int idx : chosen) {
            try {
                auto hero = pool.draft(idx);
                std::cout << "  Drafted: " << hero->getName()
                          << " (" << hero->getClass() << ")"
                          << " — default strategy: "
                          << hero->getStrategy()->getName() << "\n";
                party_.addMember(std::move(hero));
            } catch (const InvalidSelectionException& e) {
                std::cout << "  " << e.what() << "\n";
            }
        }
        std::cout << "\nParty assembled! Heroes arrive with default strategies.\n";
    }

    
    void setupInventory() {
        std::cout << "\n[Shop] Stocking starting equipment...\n";
        try {
            party_.getWeapons().addItem(Weapon{"Iron Sword",  50, 10});
            party_.getWeapons().addItem(Weapon{"Steel Axe",   80, 15});
            party_.getWeapons().addItem(Weapon{"Magic Staff", 70, 12});
            party_.getPotions().addItem(Potion{"Minor Heal",  20, 30});
            party_.getPotions().addItem(Potion{"Major Heal",  40, 60});
            party_.getPotions().addItem(Potion{"Elixir",      60, 100});
        } catch (const InventoryFullException& e) {
            std::cout << "  Shop: " << e.what() << "\n";
        }
        std::cout << "Done.\n";
    }

    
    void mainMenuLoop() {
        CombatSystem combat{party_, party_, difficulty_};

        while (running_) {
            std::cout << "\n╔════════════════════════════════╗\n";
            std::cout << "║        SPECTA LEAGUE MENU      ║\n";
            std::cout << "╠════════════════════════════════╣\n";
            std::cout << "║ [1] Fight (random enemy)       ║\n";
            std::cout << "║ [2] Fight Orc                  ║\n";
            std::cout << "║ [3] Fight Dragon               ║\n";
            std::cout << "║ [4] Fight Skeleton             ║\n";
            std::cout << "║ [5] Fight Troll                ║\n";
            std::cout << "║ [6] Auto-battle (tournament)   ║\n";
            std::cout << "║ [7] View party / stats         ║\n";
            std::cout << "║ [8] View inventory             ║\n";
            std::cout << "║ [9] Equip weapon               ║\n";
            std::cout << "║ [k] Change hero strategy       ║\n";
            std::cout << "║ [l] View combat log            ║\n";
            std::cout << "║ [d] Set difficulty             ║\n";
            std::cout << "║ [0] Exit                       ║\n";
            std::cout << "╚════════════════════════════════╝\n";
            std::cout << "> ";

            char choice{};
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (party_.isWiped() && choice != '0') {
                std::cout << "\nAll heroes are dead. Game over.\n";
                running_ = false;
                break;
            }

            try {
                switch (choice) {
                    case '1':
                        combat.runBattle(EnemyFactory::EnemyType::RANDOM);
                        break;
                    case '2':
                        combat.runBattle(EnemyFactory::EnemyType::ORC);
                        break;
                    case '3':
                        combat.runBattle(EnemyFactory::EnemyType::DRAGON);
                        break;
                    case '4':
                        combat.runBattle(EnemyFactory::EnemyType::SKELETON);
                        break;
                    case '5':
                        combat.runBattle(EnemyFactory::EnemyType::TROLL);
                        break;
                    case '6':
                        doTournament(combat);
                        break;
                    case '7':
                        party_.displayMembers(std::cout);
                        party_.displayStats(std::cout);
                        break;
                    case '8':
                        party_.displayInventory(std::cout);
                        break;
                    case '9':
                        doEquipWeapon();
                        break;
                    case 'k': case 'K':
                        doChangeStrategy();
                        break;
                    case 'l': case 'L':
                        party_.displayLog(std::cout);
                        break;
                    case 'd': case 'D':
                        doSetDifficulty(combat);
                        break;
                    case '0':
                        running_ = false;
                        break;
                    default:
                        std::cout << "Unknown command.\n";
                }
            } catch (const EmptyPartyException& e) {
                std::cout << "[Error] " << e.what() << "\n";
            } catch (const SpectaException& e) {
                std::cout << "[Error] " << e.what() << "\n";
            }
        }
    }

   
    void doTournament(CombatSystem& combat) {
        std::cout << "\n══ TOURNAMENT MODE — 3 battles, heroes fight automatically ══\n";
        std::cout << "Strategies in use:\n";
        for (int i = 0; i < party_.size(); ++i) {
            const Hero* h = party_.getMember(i);
            if (h)
                std::cout << "  " << h->getName() << " : "
                          << h->getStrategy()->getName() << "\n";
        }

        int wins = 0;
        for (int round = 1; round <= 3 && !party_.isWiped(); ++round) {
            std::cout << "\n  [Tournament Round " << round << "]\n";
            if (combat.runAutoBattle()) ++wins;
        }
        std::cout << "\nTournament over. Wins: " << wins << "/3\n";
    }

  
    void doEquipWeapon() {
        if (party_.getWeapons().isEmpty()) {
            std::cout << "No weapons available.\n";
            return;
        }
        party_.displayInventory(std::cout);
        std::cout << "Choose weapon number: ";
        int widx{};
        std::cin >> widx;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        party_.displayMembers(std::cout);
        std::cout << "Choose hero number to equip: ";
        int hidx{};
        std::cin >> hidx;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        try {
            Weapon w  = party_.getWeapons().removeItem(widx - 1);
            Hero*  h  = party_.getMember(hidx - 1);
            if (!h) throw InvalidSelectionException(hidx, party_.size());
            h->equipWeapon(w.getAttackBonus());
            std::cout << h->getName() << " equips " << w.getName()
                      << " (ATK +" << w.getAttackBonus() << ").\n";
        } catch (const SpectaException& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    
    void doChangeStrategy() {
        party_.displayMembers(std::cout);
        std::cout << "Choose hero number: ";
        int hidx{};
        std::cin >> hidx;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        Hero* h = party_.getMember(hidx - 1);
        if (!h) { std::cout << "Invalid selection.\n"; return; }

        std::cout << "\nStrategies:\n"
                  << "  [1] Aggressive (max DPS — attack or special every turn)\n"
                  << "  [2] Defensive  (heal at < 40 % HP, otherwise attack)\n"
                  << "  [3] Tactical   (special every 3 turns, heal at < 30 % HP)\n"
                  << "Choice: ";
        int s{};
        std::cin >> s;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::shared_ptr<CombatStrategy> strat;
        switch (s) {
            case 1: strat = std::make_shared<AggressiveStrategy>(); break;
            case 2: strat = std::make_shared<DefensiveStrategy>();  break;
            case 3: strat = std::make_shared<TacticalStrategy>();   break;
            default:
                std::cout << "Invalid — strategy unchanged.\n";
                return;
        }
        h->setStrategy(strat);
        std::cout << h->getName() << "'s strategy changed to: "
                  << strat->getName() << "\n";
    }

    
    void doSetDifficulty(CombatSystem& combat) {
        std::cout << "Enter difficulty (0 = easy, 1 = normal, 2 = hard): ";
        int d{};
        if (std::cin >> d) {
            difficulty_ = d;
            combat.setDifficulty(d);
            std::cout << "Difficulty set to " << d << ".\n";
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

   
    void printFinalStats() const {
        std::cout << "\n══════════ FINAL STATS ══════════\n";
        party_.displayStats(std::cout);
        party_.printFinalRanking(std::cout);
        std::cout << "\nHeroes ever created : " << Hero::getHeroCount()    << "\n";
        std::cout << "Enemies ever created: " << EnemyFactory::getEnemiesCreated() << "\n";
    }
};

#endif
