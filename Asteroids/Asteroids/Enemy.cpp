//
//  Enemy.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include "Enemy.h"

Enemy::Enemy(void)
        : position({0, 0, 0, 0, 0})
        , live(true) {}

void Enemy::draw(void) {
    // Draw object
}

void Enemy::tick(Keys& keys) {
    // Move object
}

GMPosition* Enemy::getPosition(void) {
    return &position;
}

bool Enemy::isLive(void) const {
    return live;
}

void Enemy::dead(void) {
    live = false;
}
