//
//  Geometry.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <cmath>

#include "Geometry.h"

void GMPosition::moveForward(float dt, float speed) {
    x += 0.1 * cos(angle) * dt * speed;
    y += 0.1 * sin(angle) * dt * speed;
}

void GMPosition::moveMoveBack(float dt, float speed) {
}

void GMPosition::turnLeft(float dt, float speed) {
    angle += speed * dt / 1000.0f;
}

void GMPosition::turnRight(float dt, float speed) {
    angle -= speed * dt / 1000.0f;
}

float GMPosition::getRotate(void) {
    return angle * 180 / M_PI - 90;
}
