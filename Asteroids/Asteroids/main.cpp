//
//  main.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/10/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <ctime>

#include "Game.h"

int main(int argc, const char * argv[])
{
    // Init random
    srand((unsigned)time(0));

    // Init glut
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    Game *game = new Game(800, 600);
    game->init();
    game->start();
    
    glutMainLoop();
    
    game->stop();
    
    return 0;
}

