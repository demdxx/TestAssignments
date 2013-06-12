//
//  Asteroid.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Asteroid__
#define __Asteroids__Asteroid__

#include <ctime>
#include <stdlib.h>

#include "Enemy.h"
#include "Geometry.h"

class Asteroid : public Enemy
{
    enum {
        MIN_FACETS = 6
       ,MAX_FACETS = 30
    };
    
    float   _r, _g, _b;

    float   _speed;
    float   _rotate;
    int     _size;

    int     _pointsCount;
    
    void generateBody(void);
public:
    Asteroid(int size, float speed);

    void draw(void);
    void tick(Keys& keys);
    
    bool ifFaced(Enemy* e);
    int getSize(void) const;
};

#endif /* defined(__Asteroids__Asteroid__) */
