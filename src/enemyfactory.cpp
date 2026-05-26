#include "enemyfactory.h"
#include <random>

static std::mt19937& rng() {
    static std::mt19937 gen{std::random_device{}()};
    return gen;
}

static int randInt(int lo, int hi) {
    return std::uniform_int_distribution<int>{lo, hi}(rng());
}

int EnemyFactory::enemiesCreated_ = 0;

const std::vector<std::string> EnemyFactory::orcNames_ = {
    "Grog", "Brak", "Ugor", "Zark", "Morg", "Thrak", "Vorg"
};

const std::vector<std::string> EnemyFactory::dragonNames_ = {
    "Ignis", "Thalax", "Verath", "Scoria", "Emberclaw"
};

const std::vector<std::string> EnemyFactory::skeletonNames_ = {
    "Rattlebones", "Grim", "Dustwalker", "Bonechill", "Ashwraith"
};

const std::vector<std::string> EnemyFactory::trollNames_ = {
    "Grunk", "Mogash", "Boulderback", "Stonefoot", "Murkwater"
};

const std::string& EnemyFactory::randomName(const std::vector<std::string>& v) {
    return v[static_cast<std::size_t>(randInt(0, static_cast<int>(v.size()) - 1))];
}

std::shared_ptr<Enemy> EnemyFactory::makeOrc(int diff) {
    ++enemiesCreated_;
    return std::make_shared<Orc>(randomName(orcNames_),
                                 60 + diff * 15,
                                 12 + diff * 4,
                                 30 + diff * 10);
}

std::shared_ptr<Enemy> EnemyFactory::makeDragon(int diff) {
    ++enemiesCreated_;
    return std::make_shared<Dragon>(randomName(dragonNames_),
                                    120 + diff * 30,
                                    20 + diff * 6,
                                    80 + diff * 20);
}

std::shared_ptr<Enemy> EnemyFactory::makeSkeleton(int diff) {
    ++enemiesCreated_;
    return std::make_shared<Skeleton>(randomName(skeletonNames_),
                                      45 + diff * 10,
                                      10 + diff * 3,
                                      20 + diff * 8);
}

std::shared_ptr<Enemy> EnemyFactory::makeTroll(int diff) {
    ++enemiesCreated_;
    return std::make_shared<Troll>(randomName(trollNames_),
                                   90 + diff * 20,
                                   14 + diff * 5,
                                   50 + diff * 15);
}

std::shared_ptr<Enemy> EnemyFactory::create(EnemyType type, int difficultyMod) {
    if (type == EnemyType::RANDOM) {
        switch (randInt(0, 3)) {
            case 0: return makeOrc(difficultyMod);
            case 1: return makeDragon(difficultyMod);
            case 2: return makeSkeleton(difficultyMod);
            default: return makeTroll(difficultyMod);
        }
    }
    switch (type) {
        case EnemyType::ORC:      return makeOrc(difficultyMod);
        case EnemyType::DRAGON:   return makeDragon(difficultyMod);
        case EnemyType::SKELETON: return makeSkeleton(difficultyMod);
        case EnemyType::TROLL:    return makeTroll(difficultyMod);
        default:                  return makeOrc(difficultyMod);
    }
}
