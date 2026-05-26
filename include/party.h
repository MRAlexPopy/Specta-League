#ifndef OOP_PARTY_H
#define OOP_PARTY_H

#include "hero.h"
#include "item.h"
#include "inventory.h"
#include "combatobserver.h"
#include "exceptions.h"
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string>


class Party : public CombatObserver {
    std::vector<std::unique_ptr<Hero>> members_; 
    Inventory<Weapon> weaponInventory_;
    Inventory<Potion> potionInventory_;
    std::list<std::string>     combatLog_;       
    std::map<std::string, int> killCount_;         

    int totalXp_    = 0;
    int battlesWon_ = 0;

    static const int MAX_LOG = 30;

public:
    Party() : weaponInventory_{5}, potionInventory_{5} {}
    Party(const Party&) = delete;
    Party& operator=(const Party&) = delete;
    ~Party() override = default;

    void addMember(std::unique_ptr<Hero> hero) {
        if (!hero) return;
        members_.push_back(std::move(hero));
    }

    [[nodiscard]] int size() const {
        return static_cast<int>(members_.size());
    }

    [[nodiscard]] Hero* getMember(int idx) {
        if (idx < 0 || idx >= size()) return nullptr;
        return members_[static_cast<std::size_t>(idx)].get();
    }

    [[nodiscard]] const Hero* getMember(int idx) const {
        if (idx < 0 || idx >= size()) return nullptr;
        return members_[static_cast<std::size_t>(idx)].get();
    }

    [[nodiscard]] bool isWiped() const {
        return std::all_of(members_.begin(), members_.end(),
                           [](const std::unique_ptr<Hero>& h) {
                               return !h->isAlive();
                           });
    }

   
    void onHeroDied(const std::string& heroName) override {
        addLog(heroName + " has fallen. Surviving allies are enraged!");
        for (int i = 0; i < size(); ++i) {
            Hero* h = getMember(i);
            if (h && h->isAlive())
                h->applyAvengingBuff(6);
        }
    }

   
    void onEnemyDefeated(const std::string& enemyName, int xpGained) override {
        totalXp_    += xpGained;
        ++battlesWon_;
        ++killCount_[enemyName];
        addLog("Defeated " + enemyName + " (+" + std::to_string(xpGained) + " XP)");
    }

    
    void onRoundStart(int roundNumber) override {
        if (roundNumber > 1) {
            for (int i = 0; i < size(); ++i) {
                Hero* h = getMember(i);
                if (h) h->clearAvengingBuff();
            }
        }
    }

   
    Inventory<Weapon>& getWeapons() { return weaponInventory_; }
    Inventory<Potion>& getPotions() { return potionInventory_; }

    
    void displayMembers(std::ostream& os) const {
        os << "\n--- Party Status ---\n";
        for (int i = 0; i < size(); ++i) {
            os << "  [" << (i + 1) << "] ";
            getMember(i)->displayStatus(os);
        }
    }

    void displayInventory(std::ostream& os) const {
        os << "\nWeapons:\n";
        weaponInventory_.display(os);
        os << "Potions:\n";
        potionInventory_.display(os);
    }

    void displayLog(std::ostream& os) const {
        os << "\n--- Combat Log ---\n";
        for (const auto& entry : combatLog_)
            os << "  " << entry << "\n";
    }

    void displayStats(std::ostream& os) const {
        os << "\n--- Party Stats ---\n";
        os << "  Battles won : " << battlesWon_ << "\n";
        os << "  Total XP    : " << totalXp_ << "\n";
        if (!killCount_.empty()) {
            os << "  Kill count  :\n";
            for (const auto& kv : killCount_)
                os << "    " << kv.first << " : " << kv.second << "\n";
        }
    }

    
    void printFinalRanking(std::ostream& os) const {
        std::vector<const Hero*> sorted;
        sorted.reserve(static_cast<std::size_t>(size()));
        for (int i = 0; i < size(); ++i)
            sorted.push_back(getMember(i));

        std::sort(sorted.begin(), sorted.end(),
                  [](const Hero* a, const Hero* b) {
                      return a->getHp() > b->getHp();
                  });

        os << "\n--- Final Ranking (by remaining HP) ---\n";
        int rank = 1;
        for (const Hero* h : sorted) {
            os << "  #" << rank++ << "  ";
            h->displayStatus(os);
        }
    }

    [[nodiscard]] int getTotalXp()    const { return totalXp_; }
    [[nodiscard]] int getBattlesWon() const { return battlesWon_; }

private:
    void addLog(const std::string& entry) {
        if (static_cast<int>(combatLog_.size()) >= MAX_LOG)
            combatLog_.pop_front();
        combatLog_.push_back(entry);
    }
};

#endif
