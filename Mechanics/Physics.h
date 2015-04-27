#include "Game.h"
#include <X11/keysym.h>
int x=1250, y=900;

const float gravity = 0.5f;

void playerFwd(Game *g) {	
    //player determines if player is moving forward
    if(g->player.getVelX() > 0) g->player.setFwd(true);

    else if(g->player.getVelX() < 0) g->player.setFwd(false);

    if(g->getRun()) g->player.setFwd(true);
}

void playerJump(Game *g) {

    //check if plalyer is jumping
    if (g->player.getVelY() == 0 && g->getCol()) {
        g->player.setJump(false);
        g->setCol(false);
    }

    else { g->player.setJump(true); }
}

void playerForces(Game *g) {

    //apply forces
    //apply gravity
    g->player.applyGrav(gravity);

    //apply velocity in both directions
    if (g->player.getVelX() + g->player.getPosX() < 4000){
        g->player.setPosX(g->player.getPosX() + g->player.getVelX());	
    }

    g->player.setPosY(g->player.getPosY() + g->player.getVelY());

    //reset x velocity 
    g->player.setVelX(0);
}

void playerCollision(Game *g) {
    //Check for collision with obstacles
    float playerW = g->player.getWidth();
    float playerH = g->player.getHeight();
    float playerY = g->player.getPosY();
    float playerX = g->player.getPosX();

    float blockY;
    float blockX;
    float blockH;
    float blockW;

    Block *block = g->blockHead;

    for (int i = 0; i < g->nBlocks; i++) {

        //cout << "peanut butter ice cream diahrrea" << endl;
        blockY = block->getCenterY();
        blockX = block->getCenterX();
        blockH = block->getHeight();
        blockW = block->getWidth();

        //top
        if ( (playerX > blockX - blockW - playerW 
                    && playerX < blockX + blockW + playerW) 
                && (playerY <= blockY + blockH + playerH) 
                && (playerY >= (blockY + blockH)) ) {
            g->player.setVelY(0);
            g->player.setPosY(blockY + blockH + playerH);
            g->setCol(true);
        }
        
        //bottom
        if ( (playerX > blockX - blockW - playerW 
                    && playerX < blockX + blockW + playerW) 
                && (playerY >= blockY - blockH - playerH) 
                && (playerY <= (blockY - blockH)) ) {
            g->player.setVelY(0);
            g->player.setPosY(blockY - blockH - playerH);
            g->setCol(true);
        }
        
        //left side
        if ( (playerX > (blockX - blockW - playerW) 
                    && (playerX < blockX)) 
                && (playerY < (blockY + blockH))
                && (playerY > (blockY - blockH))
                && !(block->getAdjLeft())) {
            g->player.setPosX(blockX - blockW - playerW);
        }

        //right side
        if ( playerX < (blockX + blockW + playerW) 
                && (playerX > blockX) 
                && (playerY < (blockY + blockH ))
                && (playerY > (blockY - blockH ))
                && !(block->getAdjRight())) {
            cout << "block x " << blockX << " y " << blockY << "pushing your shit" << endl;
            g->player.setPosX(blockX + blockW + playerW);
        }

        block = block->next;
    }
}

void applyKey(Game *g, int *keys) {

    //    if (g->player.getPosX() < 0) {
    //        g->player.setPosX(g->player.getPosX() + (float)x);
    //    }
    //    else if (g->player.getPosX() > 0 + (float)x) {
    //        g->player.setPosX(g->player.getPosX() - (float)x);
    //    }


    //check keys pressed now`
    if (keys[XK_Left]) {
        g->player.setVelX(g->player.getVelX() - 4);
    }
    if (keys[XK_Right]) {
        g->player.setVelX(g->player.getVelX() + 4);
    }

    if (keys[XK_Up]) {
        if (!g->player.getJump()){
            g->player.setVelY(g->player.getVelY() + 10);
            g->player.setJump(true);
            g->setCol(false);
            //std::cout<<"jump colision = false"<<std::endl;
        }
    }
    /*
       if (keys[XK_space]) {
//whip moves to right
if(g->player.getFwd()){
g->whip.setCenterX(g->whip.getCenterX() + 70);
g->whip.setWidth(70);
g->whip.setTipX(g->whip.getCenterX() + g->whip.getWidth());
//std::cout <<g->whip.tipX<<std::endl;
//std::cout<<g->whip.center.x<<std::endl;
g->setHit(true);
}
//whip moves to left
if(!g->player.getFwd()){
g->whip.setCenterX(g->whip.getCenterX() - 70);
g->whip.setWidth(70);
g->whip.setTipX(g->whip.getCenterX() - g->whip.getWidth());
//std::cout <<g->whip.tipX<<std::endl;
//std::cout<<g->whip.center.x<<std::endl;
g->setHit(true);
}
}*/
}

/*
   void playerUpdateWhip() {
//update whip position/size with respect to player
g->updateWhip();
g->whip.setWidth(0);
}*/


//============ghoul physics ==============
//ghoul->direction
/*
   Ghouls *ghoul = g->ghoulHead;
   while(ghoul){	
   if( ghoul->getVelX() > 0){
   ghoul->setFwd(true);
   }	
   else if( ghoul->getVelX() < 0){
   ghoul->setFwd(false);
   }
   ghoul = ghoul->next;
   }

   ghoul = g->ghoulHead;
   while (ghoul){
   ghoul->applyGrav(gravity);
   ghoul->setPosX(ghoul->getPosX() + ghoul->getVelX());	
   ghoul->setPosY(ghoul->getPosY() + ghoul->getVelY());
//floor collision
if (ghoul->getPosY() <= g->floor.getCenterY() + g->floor.getHeight() + ghoul->getHeight() - 20){
//	std::cout<<g->player.vel[1]<<std::endl; //debug y-velocity
ghoul->setVelY(0);
ghoul->setPosY(g->floor.getCenterY() + g->floor.getHeight() + ghoul->getHeight() - 20);
//	std::cout<<"floor collision = true"<<std::endl;	//debug floor collision
}
ghoul = ghoul->next;
}
//Check for ghoul->collision with obstacles
ghoul = g->ghoulHead;
while(ghoul){
float ghoulW = ghoul->getWidth();
float ghoulH = ghoul->getHeight();
for (int i = 0; i < 5; i++){
float ghoulPosY = ghoul->getPosY();
float ghoulPosX = ghoul->getPosX();
float obsY = g->obstacle[i].getCenterY();
float obsX = g->obstacle[i].getCenterX();
float obsH = g->obstacle[i].getHeight();
float obsW = g->obstacle[i].getWidth();
//std::cout<<"x=" <<g->ghoul->pos[0]<< "y= "<< g->ghoul->pos[1] <<std::endl;


//top
if ( (ghoulPosX > obsX - obsW - ghoulW && ghoulPosX < obsX + obsW + ghoulW) 
&& (ghoulPosY <= obsY + obsH + ghoulH) 
&& (ghoulPosY >= (obsY + obsH - 1)) ){
ghoul->setVelY(0);
ghoul->setPosY(obsY + obsH + ghoulH);
//std::cout<<"top"<<std::endl;
g->setCol(true);
break;
}	 
//left side
if ( (ghoulPosX > (obsX - obsW - ghoulW) && ghoulPosX < obsX) 
&& (ghoulPosY < obsY + obsH) ){
//std::cout<<"left"<<std::endl;
ghoul->setVelX(ghoul->getVelX() * -1);//obsX-obsW-ghoul->;
break;
}
//right side
if ( ghoulPosX > obsX && ghoulPosX < obsX + obsW + ghoulW && 
ghoulPosY < obsY + obsH ){
//std::cout<<"right"<<std::endl;
ghoul->setVelX(ghoul->getVelX() * -1);//obsX-obsW-ghoul->;
break;
}
//whip

if ( (ghoulPosX - ghoulW <= g->whip.getTipX()) && (ghoulPosX + ghoulW >= g->whip.getTipX()) && g->getHit()) {
//std::cout<<"hitb"<<std::endl;
//std::cout<<"killb"<<std::endl;
Ghouls *saveGhoul = ghoul;
g->deleteGhoul(ghoul);
ghoul = saveGhoul;
g->nGhouls--;
break;
}	
}
ghoul = ghoul->next;
}
g->setHit(false);	
if (g->player.getPosX() < 0) {
    g->player.setPosX(g->player.getPosX() + (float)xres);
}
else if (g->player.getPosX() > 0 + (float)xres) {
    g->player.setPosX(g->player.getPosX() - (float)xres);
}*/
//---------------------------------------------------

