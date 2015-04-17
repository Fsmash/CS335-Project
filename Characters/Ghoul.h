//#pragma once
#ifndef _GHOUL_H_
#define _GHOUL_H_

#include <stddef.h>
#include "Character.h"
//Here is where we will add more features to the ghouls. 
//Suggestion, this class can be generalized even more to an enemy class that other enemy types can inherit from.
//Try adding projectile particles, vomit projectile particles that does damage.    


class Ghouls : public Character {

        public: 

                Ghouls *next;        //Ghouls will be formed in a doubley linked list and will be created and deleted that way.
                Ghouls *prev;

                Ghouls() {           //initialze pointers to NULL.
                        next = prev = NULL;
                        width = 20;
                        height = 70;
                        vel[0] = 2;
                        color[0] = 67.0/255.0;
                        color[1] = 200.0/255.0;
                        color[2] = 156.0/255.0;
                }

};

#endif //_GAME_H_
