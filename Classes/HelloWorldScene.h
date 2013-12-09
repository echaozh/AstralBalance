#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class DisplayLayer;
class ControlLayer;
class HelloWorldScene: public cocos2d::CCScene {
public:
    HelloWorldScene(): display_(0), control_(0) {}
    ~HelloWorldScene();
    virtual bool init();
    DisplayLayer &display() {return *display_;}
    CREATE_FUNC(HelloWorldScene);
private:
    DisplayLayer * display_;
    ControlLayer *control_;
};

class DisplayLayer: public cocos2d::CCLayerColor {
public:
    DisplayLayer(): touch_pos_(0) {}
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void set_touch_pos(cocos2d::CCPoint pos);
    CREATE_FUNC(DisplayLayer);
private:
    cocos2d::CCLabelTTF *touch_pos_;
};

class ControlLayer: public cocos2d::CCLayerColor {
public:
    static ControlLayer *create(HelloWorldScene &scene) {
        auto *p = new ControlLayer(scene);
        if(p && p->init()) {
            p->autorelease();
            return p;
        } else {
            delete p;
            return 0;
        }
    }
    ControlLayer(HelloWorldScene &scene): scene_(scene) {}
    virtual bool init();
    virtual void draw();
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    void close_app(cocos2d::CCObject *p);
private:
    HelloWorldScene &scene_;
};

#endif // __HELLOWORLD_SCENE_H__
