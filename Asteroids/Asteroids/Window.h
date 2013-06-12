//
//  Window.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Window__
#define __Asteroids__Window__

#include <stdlib.h>
#include <GLUT/GLUT.h>

#include "Geometry.h"

class Window
{
protected:
    static Window* _self;

    int         _windowsID;
    GMSize      _size;
    
    // For calculating FPS
    int         _frameCount;
    int         _previousTime;
    float       _fps;

    bool        _fullScreenMode;

protected:
    Window(int width, int height);
    void init(const char* title);

    void calculateFPS(void);
    void drawFPS(float x, float y);

protected:
    // Events
    virtual void idle(void);
    virtual void renderScene(void);
    virtual void changeSize(int width, int height);
    
    // Control events
    virtual void processPressKey(unsigned char key, int x, int y);
    virtual void processReleaseKey(unsigned char key, int x, int y);
    virtual void pressKey(int key, int x, int y);
    virtual void releaseKey(int key, int x, int y);
    virtual void mouseButton(int button, int state, int x, int y);
    virtual void mouseMove(int x, int y);
};


#endif /* defined(__Asteroids__Window__) */
