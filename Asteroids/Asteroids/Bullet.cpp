//
//  Bullet.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <GLUT/GLUT.h>

#include "Bullet.h"
#include "Game.h"

Bullet::Bullet(float s) : Enemy(), _speed(s) {}

void Bullet::draw(void) {
    if (!live) {
        return;
    }

    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
    glRotatef(position.getRotate(), 0, 0, 0);
    glPointSize(3);
    glColor3f(
        float(rand())/float(RAND_MAX),
        float(rand())/float(RAND_MAX),
        float(rand())/float(RAND_MAX));
    glBegin(GL_POINTS);
    glVertex2f(0, 0);
    glEnd();
    glPointSize(1);
    glPopMatrix();
}

void Bullet::tick(Keys& keys) {
    position.moveForward(Game::DELTA_T, _speed);
}
