#ifndef OOP_ENEMYFACTORY_H
#define OOP_ENEMYFACTORY_H

#include "enemy.h"
#include <memory>
#include <string>
#include <vector>
#include <cstdlib>


class EnemyFactory {
    static const std::vector<std::string> orcNames_;
    static const std::vector<std::string> dragonNames_;
    static const std::vector<std::string> skeletonNames_;
    static const std::vector<std::string> trollNames_;

    static int enemiesCreated_;

public:
    enum class EnemyType { ORC, DRAGON, SKELETON, TROLL, RANDOM };

    
    EnemyFactory() = delete;

    [[nodiscard]] static std::shared_ptr<Enemy> create(EnemyType type,
                                                       int difficultyMod = 0);

    [[nodiscard]] static int getEnemiesCreated() { return enemiesCreated_; }

private:
    static std::shared_ptr<Enemy> makeOrc(int diff);
    static std::shared_ptr<Enemy> makeDragon(int diff);
    static std::shared_ptr<Enemy> makeSkeleton(int diff);
    static std::shared_ptr<Enemy> makeTroll(int diff);

    static const std::string& randomName(const std::vector<std::string>& names);
};

#endif
