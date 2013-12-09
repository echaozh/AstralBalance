#ifndef _GAME_HPP_
#define _GAME_HPP_ 1

#include <memory>
#include <vector>

#include "cocos2d.h"

#include "SpaceWorld.hpp"
#include "Star.hpp"

class SpaceWorld;
class Level {
public:
    Level(cocos2d::CCScene &scene, cocos2d::CCDirector &dir);
private:
    std::unique_ptr<SpaceWorld> world_; // so worlds can be polymorphic
};

class Game {
public:
    Game(cocos2d::CCDirector &dir);
private:
    cocos2d::CCDirector &dir_;
    std::unique_ptr<Level> level_; // so levels can be polymorphic
};

#endif // _GAME_HPP_
