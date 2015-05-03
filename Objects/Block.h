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

    public:

        Block *next;

        Block *prev;

        ~Block() {
	        if (next != NULL) delete next;
        }

        void setClimb(bool iClimb) { climb = iClimb; }

        bool getClimb() { return climb; }
       
        void setBreakable(bool iBreak) { breakable = iBreak; }

        bool getBreakable() { return breakable; }

        Block() {
            next = prev = NULL;
            width = height = 40;
            climb = false;
            breakable = false;
        }
};


#endif //_OBSTACLE_H_
