#ifndef _SPACE_WORLD_HPP_
#define _SPACE_WORLD_HPP_ 1

#include <functional>
#include <memory>
#include <random>
#include <vector>

#include <cocos2d.h>

#include "Box2Dpp.hpp"
#include "CCDelegates.hpp"
#include "Star.hpp"

class BorderBox {
public:
    BorderBox(b2World &world, cocos2d::CCSize size, int ratio);
private:
    body_ptr body_;
    fixture_ptr bottom_, left_, top_, right_;
};

class SpaceWorld {
public:
    SpaceWorld(cocos2d::CCLayer &layer, cocos2d::CCDirector &dir);
    inline void update(float delta);
    inline bool touchBegan(cocos2d::CCTouch &touch, cocos2d::CCEvent &event);
    inline void touchCancelled(cocos2d::CCTouch &touch, cocos2d::CCEvent &event);
    inline void touchEnded(cocos2d::CCTouch &touch, cocos2d::CCEvent &event);
private:
    UpdateDelegate<SpaceWorld> ud_;
    SingleTouchDelegate<SpaceWorld, &SpaceWorld::touchBegan, nullptr,
                        &SpaceWorld::touchEnded,
                        &SpaceWorld::touchCancelled> td_;
    cocos2d::CCSize size_;
    std::unique_ptr<b2World> world_;
    std::vector<Star> stars_;
    BorderBox borders_;
private:                        // randoms
    std::random_device rd_;
    std::default_random_engine re_;
    uniform_real_distribution<float> vdist_;
};

#endif // _SPACE_WORLD_HPP_
