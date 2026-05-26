#ifndef OOP_HERO_H
#define OOP_HERO_H

#include <string>
#include <iostream>
#include <memory>
#include <algorithm>
#include <random>
#include "exceptions.h"
#include "strategy.h"


class Enemy;
class Party;


class Hero {
protected:
    std::string name_;
    int hp_          = 0;
    int maxHp_       = 0;
    int attackDmg_   = 0;
    int level_       = 1;
    int weaponBonus_ = 0;
    int avengingBuff_ = 0; 

    std::shared_ptr<CombatStrategy> strategy_;

    static int heroCount_;

public:
    Hero(const std::string& name, int hp, int atk, int lvl = 1)
        : name_{name}, hp_{hp}, maxHp_{hp}, attackDmg_{atk}, level_{lvl} {
        ++heroCount_;
    }

    
    Hero(const Hero& other)
        : name_{other.name_}, hp_{other.hp_}, maxHp_{other.maxHp_},
          attackDmg_{other.attackDmg_}, level_{other.level_},
          weaponBonus_{other.weaponBonus_}, avengingBuff_{other.avengingBuff_},
          strategy_{other.strategy_} {
        ++heroCount_;
    }

    
    Hero& operator=(const Hero& other) {
        if (this == &other) return *this;
        name_         = other.name_;
        hp_           = other.hp_;
        maxHp_        = other.maxHp_;
        attackDmg_    = other.attackDmg_;
        level_        = other.level_;
        weaponBonus_  = other.weaponBonus_;
        avengingBuff_ = other.avengingBuff_;
        strategy_     = other.strategy_;
        return *this;
    }

    virtual ~Hero() { --heroCount_; }

    
    [[nodiscard]] virtual std::string getClass() const = 0;

    
    virtual void useSpecialAbility(Enemy& target) = 0;

    virtual void displayStatus(std::ostream& os) const = 0;
    [[nodiscard]] virtual std::unique_ptr<Hero> clone() const = 0;

    
    [[nodiscard]] virtual int attack() const {
        return attackDmg_ + weaponBonus_ + avengingBuff_;
    }

    
    void setStrategy(std::shared_ptr<CombatStrategy> s) {
        strategy_ = std::move(s);
    }

    
    bool executeTurn(Enemy& enemy, Party& party) {
        if (!strategy_)
            throw InvalidActionException(name_ + " has no combat strategy set");
        return strategy_->executeTurn(*this, enemy, party);
    }

    [[nodiscard]] std::shared_ptr<CombatStrategy> getStrategy() const {
        return strategy_;
    }

    
    [[nodiscard]] const std::string& getName()   const { return name_; }
    [[nodiscard]] int  getHp()                   const { return hp_; }
    [[nodiscard]] int  getMaxHp()                const { return maxHp_; }
    [[nodiscard]] int  getAttackDmg()            const { return attackDmg_; }
    [[nodiscard]] int  getLevel()                const { return level_; }
    [[nodiscard]] bool isAlive()                 const { return hp_ > 0; }
    [[nodiscard]] int  getWeaponBonus()          const { return weaponBonus_; }

    
    void takeDamage(int dmg) { hp_ = std::max(0, hp_ - dmg); }
    void heal(int amount)    { hp_ = std::min(maxHp_, hp_ + amount); }
    void equipWeapon(int bonus) { weaponBonus_ = bonus; }

    void applyAvengingBuff(int bonus) { avengingBuff_ += bonus; }
    void clearAvengingBuff()          { avengingBuff_ = 0; }

    void levelUp() {
        ++level_;
        maxHp_     += 10;
        hp_         = maxHp_;
        attackDmg_ += 3;
    }

   
    [[nodiscard]] static int getHeroCount() { return heroCount_; }
    static void resetCount()               { heroCount_ = 0; }
};


class Warrior : public Hero {
public:
    Warrior(const std::string& name, int hp, int atk, int lvl = 1)
        : Hero{name, hp, atk, lvl} {}

    Warrior(const Warrior& other) : Hero{other} {}

    Warrior& operator=(const Warrior& other) {
        if (this == &other) return *this;
        Hero::operator=(other);
        return *this;
    }

    ~Warrior() override = default;

    [[nodiscard]] std::string getClass() const override { return "Warrior"; }

    
    void useSpecialAbility(Enemy& target) override;

    void displayStatus(std::ostream& os) const override {
        os << "[Warrior] " << name_
           << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << (attackDmg_ + weaponBonus_ + avengingBuff_)
           << "  LVL:" << level_;
        if (avengingBuff_ > 0) os << "  [AVENGING +" << avengingBuff_ << "]";
        os << "\n";
    }

    [[nodiscard]] std::unique_ptr<Hero> clone() const override {
        return std::make_unique<Warrior>(*this);
    }
};


class Mage : public Hero {
    int manaPoints_ = 100;

public:
    Mage(const std::string& name, int hp, int atk, int lvl = 1)
        : Hero{name, hp, atk, lvl}, manaPoints_{100} {}

    Mage(const Mage& other) : Hero{other}, manaPoints_{other.manaPoints_} {}

    Mage& operator=(const Mage& other) {
        if (this == &other) return *this;
        Hero::operator=(other);
        manaPoints_ = other.manaPoints_;
        return *this;
    }

    ~Mage() override = default;

    [[nodiscard]] std::string getClass() const override { return "Mage"; }

   
    void useSpecialAbility(Enemy& target) override;

    void displayStatus(std::ostream& os) const override {
        os << "[Mage]    " << name_
           << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << (attackDmg_ + weaponBonus_ + avengingBuff_)
           << "  MP:" << manaPoints_
           << "  LVL:" << level_;
        if (avengingBuff_ > 0) os << "  [AVENGING +" << avengingBuff_ << "]";
        os << "\n";
    }

    [[nodiscard]] std::unique_ptr<Hero> clone() const override {
        return std::make_unique<Mage>(*this);
    }

    [[nodiscard]] int getMana() const { return manaPoints_; }
};


class Rogue : public Hero {
    int critChance_ = 30; 

public:
    Rogue(const std::string& name, int hp, int atk, int lvl = 1)
        : Hero{name, hp, atk, lvl}, critChance_{30} {}

    Rogue(const Rogue& other) : Hero{other}, critChance_{other.critChance_} {}

    Rogue& operator=(const Rogue& other) {
        if (this == &other) return *this;
        Hero::operator=(other);
        critChance_ = other.critChance_;
        return *this;
    }

    ~Rogue() override = default;

    [[nodiscard]] std::string getClass() const override { return "Rogue"; }

   
    [[nodiscard]] int attack() const override {
        int base = attackDmg_ + weaponBonus_ + avengingBuff_;
        static thread_local std::mt19937 gen{std::random_device{}()};
        if (std::uniform_int_distribution<int>{0, 99}(gen) < critChance_) {
            std::cout << "  ** CRITICAL HIT! **\n";
            return base * 2;
        }
        return base;
    }

  
    void useSpecialAbility(Enemy& target) override;

    void displayStatus(std::ostream& os) const override {
        os << "[Rogue]   " << name_
           << "  HP:" << hp_ << "/" << maxHp_
           << "  ATK:" << (attackDmg_ + weaponBonus_ + avengingBuff_)
           << "  CRIT:" << critChance_ << "%"
           << "  LVL:" << level_;
        if (avengingBuff_ > 0) os << "  [AVENGING +" << avengingBuff_ << "]";
        os << "\n";
    }

    [[nodiscard]] std::unique_ptr<Hero> clone() const override {
        return std::make_unique<Rogue>(*this);
    }
};

#endif
