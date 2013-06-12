//
//  Display.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <string>

#include "Window.h"
#include "Painter.h"

Window* Window::_self = NULL;

Window::Window(int width, int height)
       : _size({width, height})
       , _fullScreenMode(false)
       , _frameCount(0)
       , _previousTime(0)
       , _fps(0.0f)
{
    _self = this;
}

void Window::init(const char* title) {
	glutInitWindowPosition(100,100);
	glutInitWindowSize(_size.width, _size.height);
	_windowsID = glutCreateWindow(title);
    
    glutIdleFunc([](){
        _self->idle();
    });

	// callbacks for window
	glutDisplayFunc([](){
        _self->renderScene();
    });
	glutReshapeFunc([](int w, int h){
        _self->changeSize(w, h);
    });
//	glutIdleFunc(renderSceneAll);

	// register callbacks
//	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc([](unsigned char key, int x, int y){
        _self->processPressKey(key, x, y);
    });
	glutKeyboardUpFunc([](unsigned char key, int x, int y){
        _self->processReleaseKey(key, x, y);
    });
	glutSpecialFunc([](int key, int x, int y){
        _self->pressKey(key, x, y);
    });
	glutSpecialUpFunc([](int key, int x, int y){
        _self->releaseKey(key, x, y);
    });
	glutMouseFunc([](int button, int state, int x, int y){
        _self->mouseButton(button, state, x, y);
    });
	glutMotionFunc([](int x, int y){
        _self->mouseMove(x, y);
    });

    if (_fullScreenMode) {
        glutFullScreen();
    }
}

 /**
 * Calculates the frames per second
 */
void Window::calculateFPS()
{
    //  Increase frame count
    _frameCount++;
 
    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
 
    //  Calculate time passed
    int timeInterval = currentTime - _previousTime;
 
    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        _fps = float(_frameCount) / (float(timeInterval) / 1000.0f);
 
        //  Set time
        _previousTime = currentTime;
 
        //  Reset frame count
        _frameCount = 0;
    }
}

void Window::drawFPS(float x, float y) {
    char s[100];
    sprintf(s, "%.2f", _fps);
    std::string fps("FPS: ");
    fps += s;
    Painter::drawString(fps.c_str(), x, y, 1);
}

///////////////////////////////////////////////////////////////////
// EVENTS
///////////////////////////////////////////////////////////////////


void Window::idle(void) {
    calculateFPS();
}

void Window::renderScene(void) {
    // DO SOMETHING
}

void Window::changeSize(int width, int height) {
    _size = {width, height};
}
    
// Control events
void Window::processPressKey(unsigned char key, int x, int y) {
    // DO SOMETHING
}

void Window::processReleaseKey(unsigned char key, int x, int y) {
    // DO SOMETHING
}

void Window::pressKey(int key, int x, int y) {
    // DO SOMETHING
}

void Window::releaseKey(int key, int x, int y) {
    // DO SOMETHING
}

void Window::mouseButton(int button, int state, int x, int y) {
    // DO SOMETHING
}

void Window::mouseMove(int x, int y) {
    // DO SOMETHING
}

