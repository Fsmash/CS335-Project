#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include <fstream>
#include <iostream>
#include "Shape.h"

using namespace std;

class Block : public Shape {

    private:

        bool breakable;
        
        bool climb;

        bool adjRight;

        bool adjLeft;

    public:

        Block *next;

        Block *prev;

        ~Block() {
	        if (next != NULL) delete next;
        }

        void setClimb(bool iClimb) { climb = iClimb; }

        bool getClimb() { return climb; }
       
        void setAdjRight(bool adj) { adjRight = adj; }

        //bool getAdjRight() { return adjRight; }

        //void setAdjLeft(bool adj) { adjLeft = adj; }

        //bool getAdjLeft() { return adjLeft; }

        //void setBreakable(bool iBreak) { breakable = iBreak; }

        bool getBreakable() { return breakable; }

        Block() {
            next = prev = NULL;
            width = height = 40;
            climb = false;
            adjRight = adjLeft = false;
        }
};


#endif //_OBSTACLE_H_
