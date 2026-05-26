#ifndef OOP_HEROPOOL_H
#define OOP_HEROPOOL_H

#include "hero.h"
#include "strategy.h"
#include <vector>
#include <memory>
#include <iostream>
#include <set>


class HeroPool {
    std::vector<std::unique_ptr<Hero>> pool_;

public:
    HeroPool() { populate(); }
    HeroPool(const HeroPool&) = delete;
    HeroPool& operator=(const HeroPool&) = delete;
    ~HeroPool() = default;

    void display(std::ostream& os) const {
        os << "\n╔══════════════════════════════════════════════════════╗\n";
        os << "║                  TAVERN OF HEROES                   ║\n";
        os << "╠══════════════════════════════════════════════════════╣\n";
        for (int i = 0; i < static_cast<int>(pool_.size()); ++i) {
            os << "  [" << (i + 1) << "] ";
            pool_[static_cast<std::size_t>(i)]->displayStatus(os);
        }
        os << "╚══════════════════════════════════════════════════════╝\n";
    }

    [[nodiscard]] std::unique_ptr<Hero> draft(int oneBased) {
        int idx = oneBased - 1;
        if (idx < 0 || idx >= static_cast<int>(pool_.size())
                    || !pool_[static_cast<std::size_t>(idx)])
            throw InvalidSelectionException(oneBased,
                                            static_cast<int>(pool_.size()));
        return std::move(pool_[static_cast<std::size_t>(idx)]);
    }

    [[nodiscard]] int size() const { return static_cast<int>(pool_.size()); }

private:
    void populate() {
        auto aggressive = std::make_shared<AggressiveStrategy>();
        auto defensive  = std::make_shared<DefensiveStrategy>();
        auto tactical   = std::make_shared<TacticalStrategy>();

        
        auto makeWarrior = [&](const std::string& n, int hp, int atk) {
            auto w = std::make_unique<Warrior>(n, hp, atk);
            w->setStrategy(defensive);
            return w;
        };
        pool_.push_back(makeWarrior("Aldric",  120, 18));
        pool_.push_back(makeWarrior("Brynn",   110, 20));
        pool_.push_back(makeWarrior("Caelum",  130, 16));
        pool_.push_back(makeWarrior("Doric",   100, 22));
        pool_.push_back(makeWarrior("Eolan",   115, 17));
        pool_.push_back(makeWarrior("Fenwick", 125, 15));
        pool_.push_back(makeWarrior("Gorlen",  105, 21));

        
        auto makeMage = [&](const std::string& n, int hp, int atk) {
            auto m = std::make_unique<Mage>(n, hp, atk);
            m->setStrategy(tactical);
            return m;
        };
        pool_.push_back(makeMage("Hex",   80, 12));
        pool_.push_back(makeMage("Ilara", 75, 14));
        pool_.push_back(makeMage("Jinx",  70, 16));
        pool_.push_back(makeMage("Kira",  85, 10));
        pool_.push_back(makeMage("Lyris", 78, 13));
        pool_.push_back(makeMage("Mora",  72, 15));
        pool_.push_back(makeMage("Nyla",  82, 11));

        
        auto makeRogue = [&](const std::string& n, int hp, int atk) {
            auto r = std::make_unique<Rogue>(n, hp, atk);
            r->setStrategy(aggressive);
            return r;
        };
        pool_.push_back(makeRogue("Oryn",  90, 15));
        pool_.push_back(makeRogue("Pix",   95, 14));
        pool_.push_back(makeRogue("Quinn", 88, 16));
        pool_.push_back(makeRogue("Reya",  92, 13));
        pool_.push_back(makeRogue("Sable", 86, 17));
        pool_.push_back(makeRogue("Thyra", 94, 12));
    }
};

#endif
