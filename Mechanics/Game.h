#ifndef _GAME_H_
#define _GAME_H_

#include "../Characters/Player.h"
#include "../Characters/Ghoul.h"
#include "../Objects/Whip.h"

//typedef float Matrix[4][4];       not used for now

class Game {

        protected:
                bool firstrun;          //first startup
                bool firstrender;       //first time rendering

                bool hit;               //hit enemy or object, true or false
                bool col;               //collision with platform or floor, true of false

        public:
        
                Shape obstacle[4];      //platforms, change to pointer and dynamically allocate memory later
                Shape floor;            //floor of the scene
                Whip whip;             //whip weapon, try to move to player class later

                Ghouls *ghoulHead;       //pointer to beginning of Ghoul linked list
                int nGhouls;            //the number of ghouls in the list

                Players player;          //the player, could possibly be a pointer if we want to implement multiplayer
                
                Game() {
                        col = false;        //collision flag, fixes multiple jumps
                        firstrun = true;    //first time running game
                        firstrender =true;  //first time rendering
                        ghoulHead = NULL;   //initialize head of list to NULL
                        nGhouls = 0;        //initialize number of ghouls to zero
                }

                bool getHit() { return hit; }

                void setHit(bool iHit) { hit = iHit; }

                bool getCol() { return col; }

                void setCol(bool iCol) { col = iCol; }

                bool getRun() { return firstrun; }

                void setRun(bool iRun) { firstrun = iRun; }

                void updateWhip() {
                        float xPos = player.getPosY();
                        float yPos = player.getPosX();
                        whip.setCenterX(xPos);
                        whip.setCenterY(yPos);
                }

                void initWhip() {     //inititailize whip object, will try to replace, move, or both later
                        hit = false;                        //hit detection false
                        whip.setWidth(0.0f);                  //width zero because it has yet to extend
                        whip.setHeight(5.0f);                 //height about 5, could be thinner
                        whip.setColor(80.0/255.0, 127.0/255.0, 40.0/255.0);
                        whip.setTipX(0);                      //tip that causes hit
                        updateWhip();
                }

                void initFloor() {      //initialize floor
                        floor.setCenterY(0.0f);
                        floor.setCenterX(100.0f);
                        floor.setWidth(1200.0f);
                        floor.setHeight(100.0f); 
                        floor.setColor(127.0/255.0, 60.0/255.0, 56.0/255.0);
                }

                void initObstacles() {
                        //initialize obstacles, change obstacle to a pointer and dynamically allocate memory for each new platform
                        //obstacle might need to be its own class
                        obstacle[0].setCenterY(120.0f);
                        obstacle[0].setCenterX(100.0f);
                        obstacle[0].setWidth(40.0f);
                        obstacle[0].setHeight(60.0f);
                        obstacle[0].setColor(178.0/255.0, 100.0/255.0, 79.0/255.0);

                        obstacle[1].setCenterY(120.0f);
                        obstacle[1].setCenterX(600.0f);
                        obstacle[1].setWidth(40.0f);
                        obstacle[1].setHeight(45.0f);
                        obstacle[1].setColor(255.0/255.0, 183.0/255.0, 164.0/255.0);

                        obstacle[2].setCenterY(130.0f);
                        obstacle[2].setCenterX(900.0f);
                        obstacle[2].setWidth(40.0f);
                        obstacle[2].setHeight(70.0f);
                        obstacle[2].setColor(255.0/255.0, 183.0/255.0, 164.0/255.0);

                        obstacle[3].setCenterY(290.0f);
                        obstacle[3].setCenterX(1100.0f);
                        obstacle[3].setWidth(80.0f);
                        obstacle[3].setHeight(30.0f);
                        obstacle[3].setColor(255.0/255.0, 183.0/255.0, 164.0/255.0);
                }

                void createGhoul() {
                        Ghouls *gh = new Ghouls;
                        gh->next = ghoulHead;
                        if (ghoulHead != NULL)
                                ghoulHead->prev = gh;
                        ghoulHead = gh;
                        nGhouls++;
                }

                void deleteGhoul(Ghouls *node) {     //pass this function a pointer to the ghoul node to delete

                        if(node->prev == NULL){
                                if (node->next == NULL){
                                        ghoulHead = NULL;
                                } else {
                                        node->next->prev = NULL;
                                        ghoulHead = node->next;
                                }
                        } else {
                                if (node->next == NULL){
                                        node->prev->next = NULL;
                                } else {
                                        node->prev->next = node->next;
                                        node->next->prev = node->prev;
                                }
                        }

                        delete node;
                        node = NULL;
                }


};

#endif //_GAME_H_