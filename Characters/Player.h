//#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "Character.h"
//Here is where we will add more features to the player. Work on moving whip here and other weapons here.
//Rpg elements such as a level and stats will be worked on if time is allowed.    

class Players : public Character {

        public:

                Vec dir;        //direction of player character on key press. 
                bool jump;
                
                Players() {
                        width = 30.0f;
                        height = 50.0f;
                        forward=true;
                        setHealth(15);
						setScore(0);
						vel[0] = 0;
                        vel[1] = 0;
                }

                void setJump(bool iJump) { jump = iJump; }
                
                bool getJump() { return jump; }
                

};

#endif //_PLAYER_H_
