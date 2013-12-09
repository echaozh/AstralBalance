#include <sstream>

#include <touch_dispatcher/CCTouch.h>

#include "HelloWorldScene.h"

using namespace std;

USING_NS_CC;

HelloWorldScene::~HelloWorldScene() {
    // if(display_)
    //     display_->release();
    // if(control_)
    //     control_->release();
}

bool HelloWorldScene::init () {
    if(!CCScene::init())
        return false;

    display_ = DisplayLayer::create();
    control_ = ControlLayer::create(*this);
    auto size = CCDirector::sharedDirector()->getVisibleSize();
//    control_->setPosition(ccp(size.width / 8, size.height / 8));

    addChild(display_);
    addChild(control_);

    return true;
}

bool DisplayLayer::init() {
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
        return false;

    // setTouchMode(kCCTouchesOneByOne);
    // setTouchEnabled(true);

    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    touch_pos_ = CCLabelTTF::create("(NaN,NaN)", "Artial", 32);
    touch_pos_->setColor(ccc3(0, 0, 0));
    touch_pos_->setPosition(ccp(origin.x + 100, origin.y + size.height - 100));
         // ccp(origin.x + size.width / 2,
                             //     origin.y + size.height / 2));
    addChild(touch_pos_);

    return true;
}

void DisplayLayer::set_touch_pos(cocos2d::CCPoint pos) {
    touch_pos_->setPosition(pos);
    ostringstream s;
    s << "(" << pos.x << "," << pos.y << ")";
    touch_pos_->setString(s.str().c_str());
}

bool DisplayLayer::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    // auto pos = convertTouchToNodeSpace(touch);
    // scene_.display().set_touch_pos(pos);
    return true;
}

bool ControlLayer::init() {
    auto size = CCDirector::sharedDirector()->getVisibleSize();
    if(!CCLayerColor::initWithColor(ccc4(0, 0, 255, 0), size.width * 3 / 4,
                                    size.height * 3 / 4))
        return false;

    setTouchMode(kCCTouchesOneByOne);
    setTouchEnabled(true);

    auto origin = CCDirector::sharedDirector()->getVisibleOrigin();
    auto close =
        CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this,
                                menu_selector(ControlLayer::close_app));
    auto csize = close->getContentSize();
	close->setPosition(ccp(origin.x + size.width - csize.width / 2 ,
                           origin.y + csize.height / 2));

    auto menu = CCMenu::create(close, 0);
    menu->setPosition(CCPointZero);
    addChild(menu, 1);

    return true;
}

void ControlLayer::draw() {
    auto box = boundingBox();
    auto pos = box.origin;
    auto size = box.size;
    ccDrawColor4B(0, 0, 0, 255);
    ccDrawRect(pos, ccp(pos.x + size.width, pos.y + size.height));
}

bool ControlLayer::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    auto pos = convertTouchToNodeSpace(touch);
    scene_.display().set_touch_pos(pos);
    return true;
}

void ControlLayer::close_app(CCObject *p)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WINRT     \
    || CC_TARGET_PLATFORM == CC_PLATFORM_WP8
	CCMessageBox("You pressed the close button. Windows Store Apps do not "
                 "implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    exit(0);
#endif
#endif
}
