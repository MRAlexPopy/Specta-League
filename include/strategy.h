#ifndef OOP_STRATEGY_H
#define OOP_STRATEGY_H

#include <string>
#include <iostream>
#include <memory>


class Hero;
class Enemy;
class Party;


class CombatStrategy {
public:
    virtual ~CombatStrategy() = default;

    virtual bool executeTurn(Hero& hero, Enemy& enemy, Party& party) = 0;

    [[nodiscard]] virtual std::string getName() const = 0;
};


class AggressiveStrategy : public CombatStrategy {
    int turnsSinceSpecial_ = 99; 

public:
    bool executeTurn(Hero& hero, Enemy& enemy, Party& party) override;
    [[nodiscard]] std::string getName() const override { return "Aggressive"; }
};


class DefensiveStrategy : public CombatStrategy {
    int turnsSinceSpecial_ = 99;

public:
    bool executeTurn(Hero& hero, Enemy& enemy, Party& party) override;
    [[nodiscard]] std::string getName() const override { return "Defensive"; }
};


class TacticalStrategy : public CombatStrategy {
    int turnCounter_ = 0;

public:
    bool executeTurn(Hero& hero, Enemy& enemy, Party& party) override;
    [[nodiscard]] std::string getName() const override { return "Tactical"; }
};

#endif
