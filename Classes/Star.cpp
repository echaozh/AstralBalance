#include <functional>
#include <stdexcept>

#include "Star.hpp"

using namespace cocos2d;
using namespace std;
using namespace std::chrono;

Star::Star(cocos2d::CCNode &parent, b2World &world, CCPoint pos, int starZ,
           int frameZ)
    : parent_(parent), world_(world), starZ_(starZ), frameZ_(frameZ),
      star_(CCParticleSun::create()), frame_(0),
      body_(0, bind(&b2World::DestroyBody, ref(world), placeholders::_1)) {
    if(!star_)
        throw bad_alloc();
    star_->setBlendAdditive(false);
    star_->setStartColor(ccc4f(1, 1, 0, 1));
    star_->setEndColor(ccc4f(1, 0, 0, 1));
    star_->setStartSize(100);
    star_->setRadialAccel(-60);
    star_->setSpeed(90);
    star_->setSpeedVar(15);
    star_->setLife(0.3);
    star_->setLifeVar(0.1);
    star_->setPosition(pos);

    add();

    b2BodyDef def;
    def.type = b2_dynamicBody;
    def.position.Set(pos.x / star_radius, pos.y / star_radius);
    def.userData = this;
    body_.reset(world.CreateBody(&def));

    b2CircleShape circle;
    circle.m_radius = 1;
    b2FixtureDef star;
    star.shape = &circle;
    star.density = 1;
    star.friction = 0;
    star.restitution = 1;
    b2star_ = fixture_ptr(body_->CreateFixture(&star),
                          bind(&b2Body::DestroyFixture, ref(*body_),
                               placeholders::_1));
}

void Star::kick_start(float x, float y) {
    body_->ApplyLinearImpulse(b2Vec2(x, y), body_->GetPosition());
}

void Star::attract(float x, float y) {
    body_->ApplyForceToCenter(b2Vec2(x, y));
}

CCPoint Star::updatePosition() {
    auto p = body_->GetPosition();
    auto pos = ccp(p.x * star_radius, p.y * star_radius);
    star_->setPosition(pos);
    if(frame_)
        frame_->setPosition(pos);
    return pos;
}

void Star::add() {
    if(frame_)
        parent_.addChild(frame_, frameZ_);
    parent_.addChild(star_, starZ_);
}

bool Star::switchFramed() {
    if(!frame_) {
        frame_ = CCParticleSun::create();
        if(!frame_)
            throw bad_alloc();
        frame_->setBlendAdditive(false);
        frame_->setStartColor(ccc4f(0, 0, 0, 1));
        frame_->setEndColor(ccc4f(0, 0, 0, 1));
        frame_->setStartSize(120);
        frame_->setRadialAccel(-72);
        frame_->setSpeed(108);
        frame_->setSpeedVar(18);
        frame_->setLife(0.3);
        frame_->setLifeVar(0.1);
        frame_->setPosition(star_->getPosition());
        // in batch node, frame has to be added first to not cover star
        parent_.removeChild(star_, false);
        add();
    } else {
        frame_->retain();
        parent_.removeChild(frame_, false);
        frame_->release();
        frame_ = 0;
    }
    return frame_;
}

CCParticleBatchNode *Star::batchFor() {
    auto texture = CCParticleSun::create()->getTexture();
    return CCParticleBatchNode::createWithTexture(texture);
}

bool Star::isTouched(CCTouch &touch) {
    auto pos = star_->convertTouchToNodeSpace(&touch);
    CCLog("Star::isTouched(): %p => (%f, %f)\n", this, pos.x, pos.y);
    // fingers can grow too thick...
    return (pos.x >= -star_radius * 2 && pos.y >= -star_radius * 2
            && pos.x <= star_radius * 2 && pos.y <= star_radius * 2);
}

void Star::setFramed(bool on) {
    if(on) {
        frameTime_ = duration_cast<duration<float> >(
            steady_clock::now().time_since_epoch());
        auto pos = star_->getPosition();
        framePos_ = b2Vec2(pos.x, pos.y);
    }
    if((bool) frame_ != on)
        switchFramed();
}

void Star::pushBy(CCTouch &touch) {
    auto t = duration_cast<duration<float> >(
        steady_clock::now().time_since_epoch());
    auto p = star_->getParent()->convertTouchToNodeSpace(&touch);
    b2Vec2 pos(p.x, p.y);
    pos -= framePos_;
    pos *= 1 / (star_radius * (t - frameTime_).count());
    kick_start(pos.x, pos.y);
}
