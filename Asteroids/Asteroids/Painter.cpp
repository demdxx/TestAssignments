//
//  Painter.cpp
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/11/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#include "Painter.h"

#include <string>
#include <GLUT/GLUT.h>

void Painter::drawString(const char *s, float x, float y, float z)
{
    Painter::drawString(GLUT_BITMAP_HELVETICA_18, s, x, y, z);
}

void Painter::drawString(void* font, const char *s, float x, float y, float z)
{
    unsigned int i;
    glRasterPos3f(x, y, z);
    
    for (i = 0; i < strlen(s); i++)
        glutBitmapCharacter(font, s[i]);
}
