#ifndef OOP_ITEM_H
#define OOP_ITEM_H

#include <string>
#include <iostream>


class Item {
protected:
    std::string name_;
    int value_ = 0;

public:
    Item(const std::string& name, int value) : name_{name}, value_{value} {}
    Item(const Item&) = default;
    Item& operator=(const Item&) = default;
    virtual ~Item() = default;

    [[nodiscard]] const std::string& getName() const { return name_; }
    [[nodiscard]] int getValue() const { return value_; }

    
    [[nodiscard]] virtual std::string getType() const = 0;
    virtual void describe(std::ostream& os) const = 0;

    
    Item& operator+=(int bonus) {
        value_ += bonus;
        return *this;
    }

    
    friend std::ostream& operator<<(std::ostream& os, const Item& item);
    friend std::istream& operator>>(std::istream& is, Item& item);
};


inline bool operator<(const Item& a, const Item& b) {
    return a.getValue() < b.getValue();
}

inline std::ostream& operator<<(std::ostream& os, const Item& item) {
    item.describe(os);
    return os;
}

inline std::istream& operator>>(std::istream& is, Item& item) {
    is >> item.name_ >> item.value_;
    return is;
}


class Weapon : public Item {
    int attackBonus_ = 0;

public:
    Weapon(const std::string& name, int value, int atkBonus)
        : Item{name, value}, attackBonus_{atkBonus} {}

    explicit Weapon(const std::string& name)
        : Item{name, 10}, attackBonus_{5} {}

    Weapon(const Weapon& other) : Item{other}, attackBonus_{other.attackBonus_} {}

    Weapon& operator=(const Weapon& other) {
        if (this == &other) return *this;
        Item::operator=(other);
        attackBonus_ = other.attackBonus_;
        return *this;
    }

    ~Weapon() override = default;

    [[nodiscard]] std::string getType() const override { return "Weapon"; }
    [[nodiscard]] int getAttackBonus() const { return attackBonus_; }

    void describe(std::ostream& os) const override {
        os << "[Weapon] " << name_
           << "  ATK+" << attackBonus_
           << "  val=" << value_;
    }
};


class Potion : public Item {
    int healAmount_ = 0;

public:
    Potion(const std::string& name, int value, int heal)
        : Item{name, value}, healAmount_{heal} {}

    explicit Potion(const std::string& name)
        : Item{name, 8}, healAmount_{30} {}

    Potion(const Potion& other) : Item{other}, healAmount_{other.healAmount_} {}

    Potion& operator=(const Potion& other) {
        if (this == &other) return *this;
        Item::operator=(other);
        healAmount_ = other.healAmount_;
        return *this;
    }

    ~Potion() override = default;

    [[nodiscard]] std::string getType() const override { return "Potion"; }
    [[nodiscard]] int getHealAmount() const { return healAmount_; }

    void describe(std::ostream& os) const override {
        os << "[Potion] " << name_
           << "  HP+" << healAmount_
           << "  val=" << value_;
    }
};

#endif
