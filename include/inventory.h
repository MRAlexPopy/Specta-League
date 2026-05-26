#ifndef OOP_INVENTORY_H
#define OOP_INVENTORY_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "exceptions.h"


template <typename T>
class Inventory {
    std::vector<T> items_;
    int maxCapacity_ = 10;

public:
    explicit Inventory(int cap = 10) : maxCapacity_{cap} {}
    Inventory(const Inventory&) = default;
    Inventory& operator=(const Inventory&) = default;
    ~Inventory() = default;

    void addItem(const T& item) {
        if (static_cast<int>(items_.size()) >= maxCapacity_)
            throw InventoryFullException(item.getName());
        items_.push_back(item);
    }

    
    T removeItem(int index) {
        if (index < 0 || index >= static_cast<int>(items_.size()))
            throw InvalidSelectionException(index + 1,
                                            static_cast<int>(items_.size()));
        T item = items_[static_cast<std::size_t>(index)];
        items_.erase(items_.begin() + index);
        return item;
    }

    [[nodiscard]] int  size()    const { return static_cast<int>(items_.size()); }
    [[nodiscard]] bool isEmpty() const { return items_.empty(); }

    [[nodiscard]] const T& getItem(int index) const {
        if (index < 0 || index >= static_cast<int>(items_.size()))
            throw InvalidSelectionException(index + 1,
                                            static_cast<int>(items_.size()));
        return items_[static_cast<std::size_t>(index)];
    }

    void display(std::ostream& os) const {
        if (items_.empty()) { os << "  (empty)\n"; return; }
        for (int i = 0; i < static_cast<int>(items_.size()); ++i)
            os << "  [" << (i + 1) << "] "
               << items_[static_cast<std::size_t>(i)] << "\n";
    }

    
    void sortByValue() {
        std::sort(items_.begin(), items_.end(),
                  [](const T& a, const T& b) {
                      return a.getValue() > b.getValue();
                  });
    }
};

#endif
