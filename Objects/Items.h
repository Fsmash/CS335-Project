#ifndef _ITEMS_H_
#define _ITEMS_H_
#include <fstream>
#include <iostream>
#include "Shape.h"
#include "../Mechanics/Game.h"

using namespace std;

class Items : public Shape {

    public:

        Items *next;
        Items *prev;
		bool col;

		~Items() {
	        if (next != NULL) delete next;
		}
        Items() {
			col = false;
        }
};


#endif //_OBSTACLE_H_
