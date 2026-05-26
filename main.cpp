#include "game.h"
#include "inventory.h"
#include "item.h"
#include "exceptions.h"
#include <iostream>

int main() {

    
    Inventory<Weapon> demoWeapons(3);
    Inventory<Potion> demoPotions(3);

    try {
        demoWeapons.addItem(Weapon{"Training Sword", 10, 3});
        demoWeapons.addItem(Weapon{"Rusty Dagger",    5, 2});
        demoPotions.addItem(Potion{"Small Herb",      5, 15});
        demoWeapons.sortByValue(); 
        demoPotions.sortByValue();
    } catch (const InventoryFullException& e) {
       
        std::cout << "[Demo] InventoryFullException: " << e.what() << "\n";
    } catch (const SpectaException& e) {
        std::cout << "[Demo] SpectaException: " << e.what() << "\n";
    } catch (const std::exception& e) {
        std::cout << "[Demo] std::exception: " << e.what() << "\n";
    }

    
    try {
        throw InvalidSelectionException(99, 5);
    } catch (const InvalidSelectionException& e) {
        
        std::cout << "[Exception demo] InvalidSelectionException: "
                  << e.what() << "\n";
    } catch (const SpectaException& e) {
        std::cout << "[Exception demo] SpectaException: " << e.what() << "\n";
    } catch (const std::runtime_error& e) {
        std::cout << "[Exception demo] runtime_error: " << e.what() << "\n";
    }

    
    Game::getInstance().run();

    return 0;
}
