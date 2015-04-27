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

        bool adjRight;

        bool adjLeft;

        void setAdjRight(bool adj) { adjRight = adj; }

        bool getAdjRight() { return adjRight; }

        void setAdjLeft(bool adj) { adjLeft = adj; }

        bool getAdjLeft() { return adjLeft; }

        void setBreakable(bool iBreak) { breakable = iBreak; }

        bool getBreakable() { return breakable; }

        Block() {
            next = prev = NULL;
            width = height = 40;
        }
};


#endif //_OBSTACLE_H_
