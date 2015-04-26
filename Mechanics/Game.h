#ifndef _GAME_H_
#define _GAME_H_

#include <fstream>
#include <iostream>
#include "../Characters/Player.h"
#include "../Characters/Ghoul.h"
#include "../Objects/Whip.h"
#include "../Objects/Block.h"

using namespace std;

//typedef float Matrix[4][4];       not used for now

class Game {

    protected:
        bool firstrun;          //first startup
        bool firstrender;       //first time rendering

        bool hit;               //hit enemy or object, true or false
        bool col;               //collision with platform or floor, true of false

    public:

        //Shape obstacle[4];    //platforms, change to pointer and dynamically allocate memory later
        Shape floor;            //floor of the scene
        Whip whip;              //whip weapon, try to move to player class later

        Block *blockHead;       //pointer to begninning of Block linked list
        int nBlocks;            //the number of blocks in the list

        Ghouls *ghoulHead;      //pointer to beginning of Ghoul linked list
        int nGhouls;            //the number of ghouls in the list

        Players player;         //the player, could possibly be a pointer if we want to implement multiplayer

        Game() {
            col = false;        //collision flag, fixes multiple jumps
            firstrun = true;    //first time running game
            firstrender =true;  //first time rendering
            ghoulHead = NULL;   //initialize ghoul head of list to NULL
            blockHead = NULL;   //initialize block head of list to NULL
            nGhouls = 0;        //initialize number of ghouls to zero
        }

        bool getHit() { return hit; }

        void setHit(bool iHit) { hit = iHit; }

        bool getCol() { return col; }

        void setCol(bool iCol) { col = iCol; }

        bool getRun() { return firstrun; }

        void setRun(bool iRun) { firstrun = iRun; }

        /*
           void updateWhip() {
           float xPos = player.getPosX();
           float yPos = player.getPosY();
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
           floor.setWidth(1200.0f);    //why was this here? - https://github.com/SilentReaper/CS335-Project.git
           floor.setHeight(100.0f); 
           floor.setColor(127.0/255.0, 60.0/255.0, 56.0/255.0);
        }*/

        Ghouls *createGhoul() {
            Ghouls *gh = new Ghouls;
            gh->next = ghoulHead;
            if (ghoulHead != NULL)
                ghoulHead->prev = gh;
            ghoulHead = gh;
            nGhouls++;
            return gh;
        }

        void deleteGhoul(Ghouls *node) {     //pass this function a pointer to the ghoul node to delete

            if(node->prev == NULL){
                if (node->next == NULL){
                    ghoulHead = NULL;
                } 

                else {
                    node->next->prev = NULL;
                    ghoulHead = node->next;
                }
            } 

            else {
                if (node->next == NULL){
                    node->prev->next = NULL;
                } 

                else {
                    node->prev->next = node->next;
                    node->next->prev = node->prev;
                }
            }

            delete node;
            node = NULL;

        }

        Block *createBlock() {

            Block *bh = new Block;
            bh->next = blockHead;
            if (blockHead != NULL)
                blockHead->prev = bh;
            blockHead = bh;
            nBlocks++;
            return bh;
        }

        char **getLevel(char *lvl, int rows, int cols) {

            char **array;
            array = new char*[rows];
            ifstream inFile;
            inFile.open(lvl);
            for (int r = 0; r < rows; r++) {

                array[r] = new char[cols];
                int c = 0;
                char ch = inFile.get();
                while (ch != '\n') {
                    array[r][c] = ch;
                    //cout << (char)array[r][c];
                    c++;
                    ch = inFile.get();
                }
                //cout << endl;
            }

            return array;    
        }
};

#endif //_GAME_H_
