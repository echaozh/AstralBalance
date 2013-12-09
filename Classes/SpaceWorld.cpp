#include <chrono>
#include <random>

#include "SpaceWorld.hpp"

using namespace cocos2d;
using namespace std;
using namespace std::chrono;

BorderBox::BorderBox(b2World &world, CCSize size, int ratio)
    : body_(0, bind(&b2World::DestroyBody, ref(world), placeholders::_1)) {
    b2BodyDef borders;
    borders.position.Set(0, 0);
    body_.reset(world.CreateBody(&borders));

    b2EdgeShape edges;
    b2FixtureDef box;
    box.shape = &edges;
//    box.restitution = 1f;

    edges.Set(b2Vec2(0, 0), b2Vec2(size.width / ratio, 0));
    bottom_ = fixture_ptr(body_->CreateFixture(&box),
                          bind(&b2Body::DestroyFixture, ref(*body_),
                               placeholders::_1));

    edges.Set(b2Vec2(0, 0), b2Vec2(0, size.height / ratio));
    left_ = fixture_ptr(body_->CreateFixture(&box),
                        bind(&b2Body::DestroyFixture, ref(*body_),
                             placeholders::_1));

    edges.Set(b2Vec2(0, size.height / ratio), b2Vec2(size.width / ratio,
                                                     size.height / ratio));
    top_ = fixture_ptr(body_->CreateFixture(&box),
                       bind(&b2Body::DestroyFixture, ref(*body_),
                            placeholders::_1));

    edges.Set(b2Vec2(size.width / ratio, size.height / ratio),
              b2Vec2(size.width / ratio, 0));
    right_ = fixture_ptr(body_->CreateFixture(&box),
                         bind(&b2Body::DestroyFixture, ref(*body_),
                              placeholders::_1));
}

SpaceWorld::SpaceWorld(CCLayer &layer, CCDirector &dir)
    : ud_(*this, *dir.getScheduler()), td_(*this, *dir.getTouchDispatcher()),
      size_(dir.getVisibleSize()), world_(new b2World(b2Vec2(0, 0))),
      borders_(*world_, dir.getVisibleSize(), Star::star_radius), re_(rd_()),
      vdist_(0.f, 20.f) {
    auto &batch = *Star::batchFor();
    layer.addChild(&batch);

    uniform_int_distribution<unsigned> wdist(50, size_.width - 50);
    uniform_int_distribution<unsigned> hdist(50, size_.height - 50);
    uniform_real_distribution<float> xdist(-20.f, 20.f);
    uniform_real_distribution<float> ydist(-20.f, 20.f);
    for (size_t i = 0; i < 10; ++i) {
        stars_.emplace_back(batch, *world_, ccp(wdist(re_), hdist(re_)), 10, 5);
        stars_.back().kick_start(xdist(re_), ydist(re_));
    }
}

void SpaceWorld::update(float delta) {
    world_->Step(delta, 10, 10);
    for(auto &star : stars_) {
        star.updatePosition();
        // auto f = star.velocity().Skew();
        // f.Normalize();
        // f *= vdist_(re_);
        // star.attract(f.x, f.y);
        // auto dx = size_.width / 2 - pos.x, dy = size_.height / 2 - pos.y;
        // auto ratio = Star::star_radius;
        // float fx, fy;
        // if(dx < 0)
        //     fx = -sqrtf(-dx / ratio);
        // else
        //     fx = sqrtf(dx / ratio);
        // if(dy < 0)
        //     fy = -sqrtf(-dy / ratio);
        // else
        //     fy = sqrtf(dy / ratio);
        // star.attract(fx, fy);
    }
}

bool SpaceWorld::touchBegan(CCTouch &touch, CCEvent &event) {
    CCLOG("SpaceWorld::touchBegan() called\n");
    for(auto &star : stars_) {
        if(star.isTouched(touch)) {
            star.setFramed(true);
            return true;
        }
    }
    return false;
}

void SpaceWorld::touchEnded(CCTouch &touch, CCEvent &event) {
    CCLOG("SpaceWorld::touchEnded() called\n");
    for(auto &star : stars_) {
        if(star.framed()) {
            star.pushBy(touch);
            break;
        }
    }
    touchCancelled(touch, event);
}

void SpaceWorld::touchCancelled(CCTouch &touch, CCEvent &event) {
    CCLOG("SpaceWorld::touchCancalled() called\n");
    for(auto &star : stars_)
        star.setFramed(false);
}
