//
//  Enemy.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Enemy__
#define __Asteroids__Enemy__

#include "Geometry.h"
#include "types.h"

class Enemy
{
protected:
    GMPosition  position;
    bool live;

public:
    Enemy(void);

    // Events
    void draw(void);
    void tick(Keys& keys);
    GMPosition* getPosition(void);
    
    bool isLive(void) const;
    void dead(void);
};

#endif /* defined(__Asteroids__Enemy__) */
