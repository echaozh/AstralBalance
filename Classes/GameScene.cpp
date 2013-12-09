#include <sstream>
#include <stdexcept>

#include "GameScene.hpp"

using namespace cocos2d;
using namespace std;

bool GameScene::init() {
    if(!CCScene::init())
        return false;

    auto *stars = StarLayer::create();
    if(!stars)
        return false;
    addChild(stars);

    return true;
}

bool StarLayer::init() {
    if(!CCLayerColor::initWithColor(ccc4(0, 255, 255, 255)))
        return false;

    batch_ = Star::batchFor();
    addChild(batch_);

    auto size = CCDirector::sharedDirector()->getVisibleSize();
    star_ = new Star(*batch_, 10, 5);
    star_->setPosition(ccp(size.width / 2, size.height / 2));

//    scheduleUpdate();

    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);

    return true;
}

bool StarLayer::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    if(star_->isTouched(*touch))
        star_->switchFramed();
    return true;
}

