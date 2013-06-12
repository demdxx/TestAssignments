//
//  Geometry.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Geometry__
#define __Asteroids__Geometry__

struct GMSize {
    int width;
    int height;
};

struct GMPoint {
    float x;
    float y;
};

struct Vertex {
    float x, y, z;
};

struct GMPosition {
    float   x;
    float   y;
    float   vx;
    float   vy;
    float   angle;
    
    void moveForward(float dt, float speed);
    void moveMoveBack(float dt, float speed);
    void turnLeft(float dt, float speed);
    void turnRight(float dt, float speed);
    float getRotate(void);
};

#endif /* defined(__Asteroids__Geometry__) */
