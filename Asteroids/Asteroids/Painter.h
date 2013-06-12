//
//  Painter.h
//  Asteroids
//
//  Created by Dmitry Ponomarev on 6/11/13.
//  Copyright (c) 2013 demdxx. All rights reserved.
//

#ifndef __Asteroids__Painter__
#define __Asteroids__Painter__

class Painter {
public:
    static void drawString(const char *s, float x, float y, float z);
    static void drawString(void *font, const char *s, float x, float y, float z);
};

#endif /* defined(__Asteroids__Painter__) */
