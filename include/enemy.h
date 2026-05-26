#ifndef OOP_ENEMY_H
#define OOP_ENEMY_H

#include <string>
#include <iostream>
#include <algorithm>


struct EnemyState {
    int atkDebuff   = 0;   
    bool burning    = false; 
    int burnTurns   = 0;
    int poisonDmg   = 0;  
    int poisonTurns = 0;
};


class Enemy {
protected:
    std::string name_;
    int hp_        = 0;
    int maxHp_     = 0;
    int attackDmg_ = 0;
    int xpReward_  = 0;
    EnemyState state_;

public:
    Enemy(const std::string& name, int hp, int atk, int xp)
        : name_{name}, hp_{hp}, maxHp_{hp}, attackDmg_{atk}, xpReward_{xp} {}

    Enemy(const Enemy&) = default;
    Enemy& operator=(const Enemy&) = default;
    virtual ~Enemy() = default;

    [[nodiscard]] virtual std::string getType() const = 0;
    virtual void displayStatus(std::ostream& os) const = 0;

    
    virtual void onTurnEnd();

   
    [[nodiscard]] virtual int attack() const {
        return std::max(1, attackDmg_ - state_.atkDebuff);
    }

    [[nodiscard]] const std::string& getName()      const { return name_; }
    [[nodiscard]] int  getHp()                      const { return hp_; }
    [[nodiscard]] int  getMaxHp()                   const { return maxHp_; }
    [[nodiscard]] bool isAlive()                    const { return hp_ > 0; }
    [[nodiscard]] int  getXpReward()                const { return xpReward_; }
    [[nodiscard]] EnemyState&       getState()            { return state_; }
    [[nodiscard]] const EnemyState& getState()      const { return state_; }

    void takeDamage(int dmg) { hp_ = std::max(0, hp_ - dmg); }

    friend std::ostream& operator<<(std::ostream& os, const Enemy& e) {
        e.displayStatus(os);
        return os;
    }
};

class Orc : public Enemy {
    bool raged_    = false;
    int rageBonus_ = 8;

public:
    Orc(const std::string& name, int hp, int atk, int xp)
        : Enemy{name, hp, atk, xp} {}

    [[nodiscard]] std::string getType() const override { return "Orc"; }

    void displayStatus(std::ostream& os) const override {
        os << "[Orc] " << name_ << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << attackDmg_;
        if (raged_) os << " (ENRAGED)";
    }

    void onTurnEnd() override {
        Enemy::onTurnEnd();
        if (!raged_ && hp_ < maxHp_ / 2) {
            raged_      = true;
            attackDmg_ += rageBonus_;
            std::cout << "  >> " << name_ << " RAGES! ATK +" << rageBonus_ << "!\n";
        }
    }
};


class Dragon : public Enemy {
    int chargeCounter_  = 0;
    bool breathCharged_ = false;

public:
    Dragon(const std::string& name, int hp, int atk, int xp)
        : Enemy{name, hp, atk, xp} {}

    [[nodiscard]] std::string getType() const override { return "Dragon"; }

    [[nodiscard]] int attack() const override {
        int base = Enemy::attack();
        return breathCharged_ ? base * 2 : base;
    }

    [[nodiscard]] bool isBreathCharged() const { return breathCharged_; }

   
    [[nodiscard]] bool hasCombustionSynergy() const {
        return breathCharged_ && state_.burning;
    }

    void displayStatus(std::ostream& os) const override {
        os << "[Dragon] " << name_ << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << attackDmg_;
        if (breathCharged_) os << " [BREATH READY]";
        if (state_.burning)  os << " [BURNING]";
    }

    void onTurnEnd() override {
        Enemy::onTurnEnd();
        ++chargeCounter_;
        breathCharged_ = (chargeCounter_ % 3 == 0);
        if (breathCharged_)
            std::cout << "  >> " << name_ << " charges its breath weapon!\n";
    }
};


class Skeleton : public Enemy {
    int blockAmount_ = 5;

public:
    Skeleton(const std::string& name, int hp, int atk, int xp)
        : Enemy{name, hp, atk, xp} {}

    [[nodiscard]] std::string getType() const override { return "Skeleton"; }

    void takeDamageBlocked(int rawDmg) {
        int absorbed = std::min(rawDmg, blockAmount_);
        int actual   = rawDmg - absorbed;
        hp_ = std::max(0, hp_ - actual);
        if (absorbed > 0)
            std::cout << "  >> " << name_ << " blocks " << absorbed << " damage!\n";
    }

    [[nodiscard]] int getBlockAmount() const { return blockAmount_; }

    void displayStatus(std::ostream& os) const override {
        os << "[Skeleton] " << name_ << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << attackDmg_ << "  BLOCK:" << blockAmount_;
    }
};


class Troll : public Enemy {
    int regenPerTurn_ = 5;

public:
    Troll(const std::string& name, int hp, int atk, int xp)
        : Enemy{name, hp, atk, xp} {}

    [[nodiscard]] std::string getType() const override { return "Troll"; }

    void displayStatus(std::ostream& os) const override {
        os << "[Troll] " << name_ << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << attackDmg_ << "  REGEN:" << regenPerTurn_ << "/turn";
    }

    void onTurnEnd() override {
        Enemy::onTurnEnd(); 
        if (isAlive() && hp_ < maxHp_) {
            int regen = std::min(regenPerTurn_, maxHp_ - hp_);
            hp_ += regen;
            std::cout << "  >> " << name_ << " regenerates " << regen << " HP.\n";
        }
    }
};

#endif
