#ifndef _CC_DELEGATE_HPP_
#define _CC_DELEGATE_HPP_ 1

#include "cocos2d.h"

template <typename DelegateFor,
          void (DelegateFor::*UpdateFunc)(float) = &DelegateFor::update>
class UpdateDelegate : public cocos2d::CCObject {
public:
    UpdateDelegate(DelegateFor &f, cocos2d::CCScheduler &sched) : f_(f) {
        retain();
        sched.scheduleUpdateForTarget(this, 0, false);
    }
    void update(float delta) {
        static_assert(UpdateFunc, "Delegated class has to have an update() "
                      "method");
        (f_.*UpdateFunc)(delta);
    }
private:
    DelegateFor &f_;
};

template <typename DelegateFor,
          bool (DelegateFor::*TouchBegan)(cocos2d::CCTouch *, cocos2d::CCEvent *)
          = &DelegateFor::touchBegan,
          void (DelegateFor::*TouchMoved)(cocos2d::CCTouch *, cocos2d::CCEvent *)
          = nullptr,
          void (DelegateFor::*TouchEnded)(cocos2d::CCTouch *, cocos2d::CCEvent *)
          = nullptr,
          void (DelegateFor::*TouchCancelled)(cocos2d::CCTouch *,
                                              cocos2d::CCEvent *) = nullptr>
class SingleTouchDelegate : public cocos2d::CCTouchDelegate,
                            public cocos2d::CCObject {
public:
    SingleTouchDelegate(DelegateFor &f) : f_(f) {}
    bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        static_assert(TouchBegan, "Delegated class has to at least have a "
                      "touchBegan() method");
        return (f_.*TouchBegan)(*touch, *event);
    }
    void ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(TouchMoved)
            (f_.*TouchMoved)(*touch, *event);
    }
    void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(TouchEnded)
            (f_.*TouchEnded)(*touch, *event);
    }
    void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
        if(TouchCancelled)
            (f_.*TouchCancelled)(*touch, *event);
    }
private:
    DelegateFor &f_;
};

// class C {
// public:
//     void g(float delta);
//     bool h();
//     bool touchBegan();
// };

// int f() {
//     C c;
//     UpdateDelegate<C, &C::g> u(c);
//     SingleTouchDelegate<C> t(c);
//     u.update(1);
//     t.ccTouchBegan();
//     return 0;
// }

#endif // _CC_DELEGATE_HPP_
