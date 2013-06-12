//
//  Bullet.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Bullet__
#define __Asteroids__Bullet__

#include "Enemy.h"

class Bullet : public Enemy
{
    float _speed;
public:
    Bullet(float s);

    void draw(void);
    void tick(Keys& keys);
};

#endif /* defined(__Asteroids__Bullet__) */
