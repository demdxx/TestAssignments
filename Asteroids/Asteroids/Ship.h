//
//  Ship.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Ship__
#define __Asteroids__Ship__

#include "Enemy.h"

class Bullet;

class Ship : public Enemy
{
    enum {
        MAX_SPEED   = 5
       ,INC_SPEED   = 1
       ,FIRE_PAUSE_TIKS = 20
    };

    bool    _jet;
    float   _speed;
    int     _fireTiksAgo;
public:
    inline Ship(void)
            : Enemy()
            , _jet(false)
            , _speed(0.0)
            , _fireTiksAgo(FIRE_PAUSE_TIKS) {}

    void draw(void);
    void tick(Keys& keys);
    
    float getSpeed(void) const;
    bool hasShield(void) const;
    bool canFire(void) const;
    
    Bullet *fire(void);
};

#endif /* defined(__Asteroids__Ship__) */
