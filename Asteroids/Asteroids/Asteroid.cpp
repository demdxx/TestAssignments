//
//  Asteroid.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <cmath>

#include "Asteroid.h"
#include "Game.h"

#define max(a, b) a>b ? a : b

Asteroid::Asteroid(int size, float speed)
    : _speed(speed)
    , _rotate(0.0)
    ,_size(size)
{
    do {
        _r = (float)rand()/(float)RAND_MAX;
        _g = (float)rand()/(float)RAND_MAX;
        _b = (float)rand()/(float)RAND_MAX;
    } while(_r<0.2 && _g<0.2 && _b<0.2);
    _pointsCount = rand(5, max(size/2.5f, 6));
}

void Asteroid::draw(void) {
    if (!live) {
        return;
    }

    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glScalef(_size, _size, 1);
    glRotatef(_rotate * 180 / M_PI - 90, 0, 0, 1);
    glColor3f(_r, _g, _b);
    glLineWidth(1);
    glBegin(GL_POLYGON);
        for (int i = 0; i < _pointsCount; ++i) {
            float y = sin(2 * M_PI / _pointsCount * i);
            float x = cos(2 * M_PI / _pointsCount * i);
            glVertex2f(
                y+(i%4 ? 0 : float(y>0?0.2:-0.2)),
                x+(i%3 ? 0 : float(x>0?0.2:-0.2)));
        }
    glEnd();
//    glColor3f(_r, _g+0.1, _b+0.1);
//    glBegin(GL_POLYGON);
//        for (int i = 0; i < _pointsCount; ++i) {
//            glVertex2f(
//                sin(2 * M_PI / _pointsCount * i),
//                cos(2 * M_PI / _pointsCount * i));
//        }
//    glEnd();
    glPopMatrix();
}

void Asteroid::tick(Keys& keys) {
    if (0==int(_speed*100)%2) {
        _rotate += _speed * Game::DELTA_T * 0.001;
    } else {
        _rotate -= _speed * Game::DELTA_T * 0.001;
    }
    position.moveForward(Game::DELTA_T, _speed);
}

bool Asteroid::ifFaced(Enemy* e)
{
    if (e->isLive()) {
        GMPosition *a = &position,
                   *b = e->getPosition();
        double d = sqrt((b->x - a->x) * (b->x - a->x) + (b->y - a->y) * (b->y - a->y));
        if (d<_size+1.5f) {
            return true;
        }
    }
    return false;
}

int Asteroid::getSize(void) const {
    return _size;
}
