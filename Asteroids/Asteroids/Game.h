//
//  Game.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Game__
#define __Asteroids__Game__

#include <set>
#include <deque>
#include <GLUT/GLUT.h>

#include "Window.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "types.h"

static int rand(int low, int high) {
   return rand() % (high - low + 1) + low;
}

class Game : public Window
{
    enum {
        ASTEROID_MIN_SIZE  = 10
       ,ASTEROID_MAX_SIZE  = 40
       
       ,ASTEROID_MIN_SPEED  = 1
       ,ASTEROID_MAX_SPEED  = 70
       
       ,FIRST_LEVEL_ASTEROIDS   = 10
       ,ASTEROIDS_LEVEL_STEP    = 10
       
       ,ASTEROIDS_AWARY_TICKS = 2030
       ,ASTEROIDS_MAC_COUNT = 999
       ,ASTEROIDS_COUNT_STEP = 10
       
       ,INVISIBLE_EDGE = 40
    };

    Keys                    _keys;
    Ship                    _ship;
    
    typedef std::deque<Bullet*> Bullets;
    typedef std::set<Asteroid*> Asterids;
    
    Asterids                _asteroids;
    Bullets                 _bullets;
    
    int                     _tick;
    int                     _killed;

public:
    enum { 
        DELTA_T = 1000 / 100,
    };

public:
    inline Game(int width, int height)
         : Window(width, height)
         , _tick(0)
         , _killed(0)
    {
    }

    virtual ~Game(void) {
        stop();
    }

    void init(void);
    void start(void);
    void stop(void);
    
private:
    bool isWin(void);
    void initLevel(void);
    void runLoop(void);
    int loop(void);
    void tick(void);
    
    void addAsteroids(int count=ASTEROIDS_COUNT_STEP);
    Asteroid* createAsteroid(int size, float speed, float x, float y);
    bool canDestroy(Enemy *e) const;

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

#endif /* defined(__Asteroids__Game__) */
