#ifndef _BOX2D_PP_HPP_
#define _BOX2D_PP_HPP_ 1

#include <Box2D/Box2D.h>

typedef std::unique_ptr<b2Body, std::function<void (b2Body *)> > body_ptr;
typedef std::unique_ptr<b2Fixture,
                        std::function<void (b2Fixture *)> > fixture_ptr;

#endif // _BOX2D_PP_HPP_
