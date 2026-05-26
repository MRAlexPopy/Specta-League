#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H

#include <stdexcept>
#include <string>


class SpectaException : public std::runtime_error {
public:
    explicit SpectaException(const std::string& msg)
        : std::runtime_error(msg) {}
    ~SpectaException() override = default;
};


class InventoryFullException : public SpectaException {
    std::string itemName_;
public:
    explicit InventoryFullException(const std::string& itemName)
        : SpectaException("Inventory full — cannot add: " + itemName),
          itemName_{itemName} {}
    [[nodiscard]] const std::string& getItemName() const { return itemName_; }
};


class InvalidSelectionException : public SpectaException {
    int selected_;
    int maxValid_;
public:
    InvalidSelectionException(int selected, int maxValid)
        : SpectaException("Selection " + std::to_string(selected) +
                          " out of range [1.." + std::to_string(maxValid) + "]"),
          selected_{selected}, maxValid_{maxValid} {}
    [[nodiscard]] int getSelected() const { return selected_; }
    [[nodiscard]] int getMaxValid() const { return maxValid_; }
};


class InvalidActionException : public SpectaException {
public:
    explicit InvalidActionException(const std::string& msg)
        : SpectaException("Invalid action: " + msg) {}
};


class EmptyPartyException : public SpectaException {
public:
    EmptyPartyException()
        : SpectaException("Cannot start combat with an empty party") {}
};

#endif
