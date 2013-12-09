#include <cmath>
#include <random>
#include <sstream>

#include "Game.hpp"

using namespace cocos2d;
using namespace std;

Level::Level(CCScene &scene, CCDirector &dir) {
    auto &layer = *CCLayerColor::create(ccc4(0, 255, 255, 255));
    scene.addChild(&layer);

    world_.reset(new SpaceWorld(layer, dir));
}

Game::Game(CCDirector &dir)
    : dir_(dir) {
    auto &scene = *CCScene::create();
    level_.reset(new Level(scene, dir));

    dir_.runWithScene(&scene);
}
