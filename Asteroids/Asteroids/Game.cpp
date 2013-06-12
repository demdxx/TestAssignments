//
//  Game.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <cstdlib>
#include <string>
#include <cmath>

#include "Game.h"
#include "Painter.h"

std::string floatToString(float v) {
    char out[10] = {0};
    sprintf(out, "%.2f", v);
    return out;
}

std::string intToString(int v) {
    char out[10] = {0};
    sprintf(out, "%d", v);
    return out;
}

void Game::init(void) {
    Window::init("Asteroids");

    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_POLYGON_SMOOTH);

    glLoadIdentity();
    glOrtho(-_size.width / 2, _size.width / 2, -_size.height / 2, _size.height / 2, -1.0, 1.0);
}

void Game::start(void) {
    initLevel();
    runLoop();
}

void Game::stop(void) {
    // Stop game
}

bool Game::isWin(void) {
    return false;
}

void Game::initLevel(void) {
    // DO SOMETHING
}

void Game::runLoop(void) {
    int wait = loop();
    glutTimerFunc(wait, [](int){
        ((Game *)_self)->runLoop();
    }, 0);
}

int Game::loop(void) {
    static int t = 0;
    int d;
    do {
        int ct = glutGet(GLUT_ELAPSED_TIME);
        if (t == 0) {
            t = ct;
        }
        tick();

        t += Game::DELTA_T;
        d = t - ct;

        if (d > 0) {
            calculateFPS();
            renderScene();
        }
    } while (d <= 0);
    return d;
}

void Game::tick(void) {
    if (_tick%ASTEROIDS_AWARY_TICKS==0) {
        addAsteroids();
    }

    if (_ship.isLive()) {
        _ship.tick(_keys);
        
        // Correct ship position
        GMPosition *pos = _ship.getPosition();
        if (pos->x-INVISIBLE_EDGE>_size.width/2.0f) {
            pos->x -= _size.width+INVISIBLE_EDGE;
        }
        else if (pos->x+INVISIBLE_EDGE<-_size.width/2.0f) {
            pos->x += _size.width+INVISIBLE_EDGE;
        }
        if (pos->y-INVISIBLE_EDGE>_size.height/2.0f) {
            pos->y -= _size.height+INVISIBLE_EDGE;
        }
        else if (pos->y+INVISIBLE_EDGE<-_size.height/2.0f) {
            pos->y += _size.height+INVISIBLE_EDGE;
        }
        
        // Fire
        if (_keys[KEY_FIRE]) {
            Bullet *b = _ship.fire();
            if (b) {
                _bullets.push_back(b);
            }
        }
    }
    
    // Destroy objects
    while(_bullets.size()>0) {
        Bullet* b = _bullets[0];
        if (!canDestroy(b)) {
            break;
        }
        delete b;
        _bullets.pop_front();
    }
    
    // Tick bullets
    for (Bullets::const_iterator it=_bullets.begin();it!=_bullets.end();it++) {
        (*it)->tick(_keys);
    }
    
    // Process collision
    for (Asterids::const_iterator it=_asteroids.begin();it!=_asteroids.end();it++) {
        Enemy *e = NULL;
        Asteroid *a = *it;
        
        if (a->isLive()) {
            if (!a->ifFaced(&_ship)) {
                for (Bullets::const_iterator it2=_bullets.begin();it2!=_bullets.end();it2++)
                {
                    if (a->ifFaced(*it2)) {
                        e = *it2;
                        break;
                    }
                }
            } else {
                e = &_ship;
            }
            
            if (NULL!=e) {
                // Destroy bullet
                if (&_ship==e && _ship.hasShield()) {
                    // We are protected
                } else {
                    e->dead();
                }
                a->dead();
                _killed++;
                
                // Generate new asteroids
                if (a->getSize()/ASTEROID_MIN_SIZE>1) {
                    GMPosition *p = a->getPosition();
                    for (int i=0;i<a->getSize()/ASTEROID_MIN_SIZE;i++) {
                        _asteroids.insert(createAsteroid(
                            ASTEROID_MIN_SIZE,
                            float(rand(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED))/70,
                            p->x, p->y)
                        );
                    }
                }
            }
        }
    }
    
    // Tick asteroids
    for (Asterids::const_iterator it=_asteroids.begin();it!=_asteroids.end();it++) {
        (*it)->tick(_keys);

        // Correct asteroid position
        GMPosition *pos = (*it)->getPosition();
        if (pos->x-INVISIBLE_EDGE>_size.width/2.0f) {
            pos->x -= _size.width+INVISIBLE_EDGE;
        }
        else if (pos->x+INVISIBLE_EDGE<-_size.width/2.0f) {
            pos->x += _size.width+INVISIBLE_EDGE;
        }
        if (pos->y-INVISIBLE_EDGE>_size.height/2.0f) {
            pos->y -= _size.height+INVISIBLE_EDGE;
        }
        else if (pos->y+INVISIBLE_EDGE<-_size.height/2.0f) {
            pos->y += _size.height+INVISIBLE_EDGE;
        }
    }
    
    _tick++;
}

void Game::addAsteroids(int count) {
    int size;
    float speed;
    Asteroid *a;
    GMPosition *p;
    
    if (ASTEROIDS_MAC_COUNT<=_asteroids.size()) {
        return;
    }

    for (int i=0;i<count;i++) {
        size = rand(ASTEROID_MIN_SIZE, ASTEROID_MAX_SIZE);
        speed = float(rand(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED))/100;
        a = new Asteroid(size, speed);
        p = a->getPosition();
        p->angle = tanf(float(rand())/float(rand()));
        
        // Position
        switch (rand(0, 7)) {
            case 0: case 4: // TOP
                p->x = rand(-(_size.width/2+INVISIBLE_EDGE-size), _size.width/2+INVISIBLE_EDGE-size);
                p->y = rand(_size.height/2+size, _size.height/2+INVISIBLE_EDGE-size);
                break;
            case 1: case 5: // BOTTOM
                p->x = rand(-(_size.width/2+INVISIBLE_EDGE-size), _size.width/2+INVISIBLE_EDGE-size);
                p->y = rand(-(_size.height/2+INVISIBLE_EDGE-size), -(_size.height/2+size));
                break;
            case 2: case 6: // LEFT
                p->x = rand(-(_size.width/2+INVISIBLE_EDGE-size), -(_size.width/2-size));
                p->y = rand(-(_size.height/2+INVISIBLE_EDGE-size), _size.height/2+INVISIBLE_EDGE-size);
                break;
            case 3: case 7: // RIGHT
                p->x = rand(_size.width/2+size, _size.width/2+INVISIBLE_EDGE-size);
                p->y = rand(-(_size.height/2+INVISIBLE_EDGE-size), _size.height/2+INVISIBLE_EDGE-size);
                break;
        }
        _asteroids.insert(a);
    }
}

Asteroid* Game::createAsteroid(int size, float speed, float x, float y) {
    Asteroid *a = new Asteroid(size, speed);
    GMPosition *p = a->getPosition();
    p->angle = tanf(float(rand())/float(rand()));
    p->x = x;
    p->y = y;
    return a;
}

bool Game::canDestroy(Enemy *e) const {
    GMPosition *pos = e->getPosition();
    if(
      ! e->isLive()
     || pos->x-INVISIBLE_EDGE>_size.width/2.0f
     || pos->x+INVISIBLE_EDGE<-_size.width/2.0f
     || pos->y-INVISIBLE_EDGE>_size.height/2.0f
     || pos->y+INVISIBLE_EDGE<-_size.height/2.0f
    ) {
        return true;
    }
    return false;
}

// Events
void Game::idle(void) {
//    // Super
//    Window::idle();
//
//    // Process tick
//    tick();
//
//    //  Call display function (draw the current frame)
//    glutPostRedisplay();
}

void Game::renderScene(void) {
    // Clean scene
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Draw ship
    _ship.draw();
    
    // Draw bullets
    for (Bullets::const_iterator it=_bullets.begin();it!=_bullets.end();it++) {
        (*it)->draw();
    }
    
    // Draw asteroids
    for (Asterids::const_iterator it=_asteroids.begin();it!=_asteroids.end();it++) {
        (*it)->draw();
    }
    
    // Draw info
    glColor3f(1.0f, 1.0f, 1.0f);
    std::string speed("Speed: ");
    speed += floatToString(_ship.getSpeed());
    std::string asteroids("Asteroids: ");
    asteroids += intToString((int)_asteroids.size()-_killed);
    std::string killed("Destroyed: ");
    killed += intToString(_killed);
    
    Painter::drawString(speed.c_str(), _size.width/2.0-180, _size.height/2.0-20, 0);
    Painter::drawString(asteroids.c_str(), _size.width/2.0-180, _size.height/2.0-40, 0);
    Painter::drawString(killed.c_str(), _size.width/2.0-180, _size.height/2.0-60, 0);

    if (!_ship.isLive()) {
        glColor3f(1.0f, 0, 0);
        Painter::drawString("You lose! Game over", _size.width/2.0-200, _size.height/2.0-80, 0);
    }
    
    drawFPS(-_size.width/2+20, -_size.height/2+20);

    /* swap the back and front buffers so we can see what we just drew */
    glutSwapBuffers();
}

void Game::changeSize(int width, int height) {
    Window::changeSize(width, height);
    
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(0, width, 0, height, -1.0, 1.0);
    glOrtho(-_size.width / 2, _size.width / 2, -_size.height / 2, _size.height / 2, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Control events
void Game::processPressKey(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // MOVE UP
            _keys.set(KEY_UP);
            break;
        case 's': // MOVE DOWN
            _keys.set(KEY_DOWN);
            break;
        case 'a': // MOVE LEFT
            _keys.set(KEY_LEFT);
            break;
        case 'd': // MOVE RIGHT
            _keys.set(KEY_RIGHT);
            break;
        case ' ': // FIRE
            _keys.set(KEY_FIRE);
            break;
        default:
            break;
    }
}

void Game::processReleaseKey(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // MOVE UP
            _keys.reset(KEY_UP);
            break;
        case 's': // MOVE DOWN
            _keys.reset(KEY_DOWN);
            break;
        case 'a': // MOVE LEFT
            _keys.reset(KEY_LEFT);
            break;
        case 'd': // MOVE RIGHT
            _keys.reset(KEY_RIGHT);
            break;
        case ' ': // FIRE
            _keys.reset(KEY_FIRE);
            break;
        default:
            break;
    }
}

void Game::pressKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            _keys.set(KEY_LEFT);
            break;
        case GLUT_KEY_RIGHT:
            _keys.set(KEY_RIGHT);
            break;
        case GLUT_KEY_DOWN:
            _keys.set(KEY_DOWN);
            break;
        case GLUT_KEY_UP:
            _keys.set(KEY_UP);
            break;
    }
}

void Game::releaseKey(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            _keys.reset(KEY_LEFT);
            break;
        case GLUT_KEY_RIGHT:
            _keys.reset(KEY_RIGHT);
            break;
        case GLUT_KEY_DOWN:
            _keys.reset(KEY_DOWN);
            break;
        case GLUT_KEY_UP:
            _keys.reset(KEY_UP);
            break;
    }
}

void Game::mouseButton(int button, int state, int x, int y) {
}

void Game::mouseMove(int x, int y) {
}

