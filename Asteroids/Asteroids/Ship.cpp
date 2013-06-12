//
//  Ship.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <cmath>

#include "Ship.h"
#include "Game.h"

#define SPEED_INC float(INC_SPEED) * float(Game::DELTA_T)/100.0

void Ship::draw(void) {
    if (!live) {
        return;
    }

    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glRotatef(position.getRotate(), 0, 0, 1);
    glColor3f(0, 1, 0);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(0, 20);
        glVertex2f(10, -10);
        glVertex2f(0, 0);
        glVertex2f(-10, -10);
    glEnd();
    glColor3f(1, 0, 0);
    if (_jet)
    {
        glLineWidth(1+(_speed>0?_speed:-_speed)*0.1);
        glBegin(GL_LINES);
            glVertex2f(0, 0);
            glVertex2f(0, -5*(_speed?_speed:-_speed));
            glVertex2f(10, -10);
            glVertex2f(10, -10-2*(_speed?_speed:-_speed));
            glVertex2f(-10, -10);
            glVertex2f(-10, -10-2*(_speed?_speed:-_speed));
        glEnd();
    }
    
    // Draw shield
    if (hasShield()) {
        glColor3f(1.0f, 1.0f, 0.0f);
        glLineWidth(1+(_speed>0?_speed:-_speed)*0.2);
        Vertex vs[4] = {
            {-14, -10, 0}
           ,{-6, 34, 0}
           ,{6, 34, 0}
           ,{14, -12, 0}
        };
        glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 4, (const float*)vs);
        glEnable(GL_MAP1_VERTEX_3);
        
        glBegin(GL_LINE_STRIP);
            for(int i=0; i<30; i++)
                glEvalCoord1f((float)i/30.0f);
        glEnd();
    }
    glPopMatrix();
}

void Ship::tick(Keys& keys) {
    // Turn left/right
    if (keys[KEY_LEFT]) {
        position.turnLeft(Game::DELTA_T, 3.0f);
    }
    if (keys[KEY_RIGHT]) {
        position.turnRight(Game::DELTA_T, 3.0f);
    }
    
    // Move forward or back
    if (keys[KEY_UP]) {
        if (_speed<MAX_SPEED) {
            _speed += SPEED_INC;
        } else {
            _speed += SPEED_INC * 0.01f;
        }
        _jet = true;
    }
    else if (keys[KEY_DOWN]) {
        if (_speed>-float(MAX_SPEED)/3.0f) {
            _speed -= SPEED_INC;
        } else {
            _speed -= SPEED_INC * 0.01f;
        }
        _jet = true;
    } else {
        if (_speed>0) {
            _speed -= SPEED_INC/1.7f;
            if (_speed<0) {
                _speed = 0;
            }
        } else if (_speed<0) {
            _speed += SPEED_INC/1.7f;
            if (_speed>0) {
                _speed = 0;
            }
        }
        _jet = false;
    }
    if (_speed!=0) {
        position.moveForward(Game::DELTA_T, _speed);
    }
    _fireTiksAgo++;
}

float Ship::getSpeed(void) const {
    return _speed;
}

bool Ship::hasShield(void) const {
    return _speed>7;
}

bool Ship::canFire(void) const {
    return _fireTiksAgo>FIRE_PAUSE_TIKS;
}

Bullet *Ship::fire(void) {
    if (canFire()) {
        _fireTiksAgo = 0;
        Bullet *b = new Bullet(5+_speed);
        GMPosition *p = b->getPosition();
        p->angle = position.angle;
        p->x = position.x + 20*cos(p->angle);
        p->y = position.y + 20*sin(p->angle);
        return b;
    }
    return NULL;
}

