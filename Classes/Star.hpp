#ifndef _STAR_HPP_
#define _STAR_HPP_ 1

#include <functional>
#include <chrono>
#include <memory>

#include <cocos2d.h>

#include "Box2Dpp.hpp"

class Star {
public:
    Star(cocos2d::CCNode &parent, b2World &world, cocos2d::CCPoint pos,
         int starZ, int frameZ);
    bool framed() const {return frame_;}
    void kick_start(float x, float y);
    void attract(float x, float y);
    void pushBy(cocos2d::CCTouch &touch);
    b2Vec2 velocity() const {return body_->GetLinearVelocity();}
    cocos2d::CCPoint updatePosition();
    bool switchFramed();
    void setFramed(bool on);
    bool isTouched(cocos2d::CCTouch &touch);
    static cocos2d::CCParticleBatchNode *batchFor();
public:
    static const int star_radius = 25;
private:
    void add();
private:
    cocos2d::CCNode &parent_;
    b2World &world_;
    int starZ_, frameZ_;
    cocos2d::CCParticleSystemQuad *star_;
    cocos2d::CCParticleSystemQuad *frame_;
    body_ptr body_;
    fixture_ptr b2star_;
private:
    std::chrono::duration<float> frameTime_;
    b2Vec2 framePos_;
};

#endif // _STAR_HPP_
