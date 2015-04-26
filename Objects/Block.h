#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include <fstream>
#include <iostream>
#include "Shape.h"

using namespace std;

class Block : public Shape {

    public:

        Block *next;

        Block *prev;

        bool breakable;

        void setBreakable(bool iBreak) { breakable = iBreak; }

        bool getBreakable() { return breakable; }

        Block() {
            next = prev = NULL;
            width = height = 40;
        }
};


#endif //_OBSTACLE_H_
