//#pragma once
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#define MAX_HEALTH 15
typedef float Vec[2];
//Base class for all characters in game. From player to NPCs to enemies.

class Character 
{
        protected:

                Vec pos;            //position of character, x and y components.
                Vec vel;            //velocity of character, x and y components.
                bool forward;       //true if moving forward, x velocity left or right. false if no velocity left or right.
                float height;       //height of character.
                float width;        //width of character.
                int health;
				int score;
	
        public:
				void setHealth(int h) { health = h; }             
				int getHealth() { return health; }  
				void decHealth() { 
					if (health > 0) {
						health--;
					} 
				}             
				void incHealth() {
					if (health < MAX_HEALTH){ 
						health++;
					}
				}             
                void setPos(float x, float y) { 
                        pos[0] = x; 
                        pos[1] = y; 
                }
                void setScore(int iScore) { score = iScore; }
                
				void addToScore(int iAdd) { score = score + iAdd; }
                
				int getScore() { return score; }

                void setPosX(float x) { pos[0] = x; }

                void setPosY(float y) { pos[1] = y; }

                float getPosX() { return pos[0]; }
                
                float getPosY() { return pos[1]; }

                void setVel(float x, float y) {
                        vel[0] = x;
                        vel[1] = y;
                }

                void setVelX(float x) { vel[0] = x; }               

                void setVelY(float y) { vel[1] = y; }
                
                float getVelX() { return vel[0]; }               
                
                float getVelY() { return vel[1]; }               
                
                void applyGrav(float grav) { vel[1] -= grav; }
    
                bool getFwd() { return forward; }
                
                void setFwd(bool iFwd) { forward = iFwd; } 
                
                void changeSize(float iHeight, float iWidth) {
                        height = iHeight;
                        width = iWidth;            
                }
                
                float getHeight() { return height; }
                
                float getWidth() { return width; }

};

#endif //_CHARACTER_H_
