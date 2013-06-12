//
//  types.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/11/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef Asteroids_types_h
#define Asteroids_types_h

#include <bitset>

enum KeyState { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_FIRE };
typedef std::bitset<5> Keys;

#endif
