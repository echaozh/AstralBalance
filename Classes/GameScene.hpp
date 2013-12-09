#ifndef _GAME_SCENE_HPP_
#define _GAME_SCENE_HPP_ 1

#include "cocos2d.h"

class GameScene : public cocos2d::CCScene {
public:
    virtual bool init();
    CREATE_FUNC(GameScene);
};

class Star;
class StarLayer : public cocos2d::CCLayerColor {
public:
    StarLayer() : batch_(0), star_(0) {}
    virtual bool init();
    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    CREATE_FUNC(StarLayer);
private:
    cocos2d::CCParticleBatchNode *batch_;
    Star *star_;
};

class Star {
public:
    Star(cocos2d::CCNode &parent, int starZ, int frameZ);
    void setPosition(cocos2d::CCPoint pos);
    void switchFramed();
    bool isTouched(cocos2d::CCTouch &touch);
    static cocos2d::CCParticleBatchNode *batchFor();
private:
    void add();
private:
    cocos2d::CCNode &parent_;
    int starZ_, frameZ_;
    cocos2d::CCParticleSystemQuad *star_;
    cocos2d::CCParticleSystemQuad *frame_;
    cocos2d::CCLabelTTF *touched_;
};

#endif // _GAME_SCENE_HPP_

