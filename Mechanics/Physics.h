#include "Game.h"

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

void playerUpdateWhip() {
        //update whip position/size with respect to player
        g->updateWhip();
        g->whip.setWidth(0);
}

void playerCollision() {
        //floor collision
        if (g->player.getPosY() <= g->block.getCenterY() + g->floor.getHeight() + g->player.getHeight() - 20) {
        g->player.setVelY(0);
        g->player.setPosY(g->floor.getCenterY() + g->floor.getHeight() + g->player.getHeight() - 20);
        g->setCol(true);
        }

        //Check for collision with obstacles
        float playerW = g->player.getWidth();
        float playerH = g->player.getHeight();
        float playerPosY = g->player.getPosY();
        float playerPosX = g->player.getPosX();

        for (int i = 0; i < 5; i++){
        float obsY = g->obstacle[i].getCenterY();
        float obsX = g->obstacle[i].getCenterX();
        float obsH = g->obstacle[i].getHeight();
        float obsW = g->obstacle[i].getWidth();

        //top
        if ( (playerPosX > obsX - obsW - playerW + 2 
        && playerPosX < obsX + obsW + playerW - 2) 
        && (playerPosY <= obsY + obsH + playerH) 
        && (playerPosY >= (obsY + obsH - 1)) ) {
        g->player.setVelY(0);
        g->player.setPosY(obsY + obsH + playerH);
        g->setCol(true);
        break;
}
//left side
if ( (playerPosX > (obsX - obsW - playerW) 
             && playerPosX < obsX) 
                && (playerPosY < obsY+obsH)){
        g->player.setPosX(obsX - obsW - playerW);
        break;

}
//right side
if ( playerPosX > obsX 
                && playerPosX < obsX + obsW + playerW  
                && (playerPosY < obsY + obsH )){ 
        g->player.setPosX(obsX + obsW + playerW);
        break;
}
////bottom
//if ( playerPosY + playerH >= obsY-obsH && 
//		(playerPosX > obsX-obsW-playerW && playerPosX < obsX+obsW+playerW)  ){
//	//g->player.vel[1] = -g->player.vel[1];
//	g->player.pos[1] = obsY-obsH -playerH;
//	std::cout<<"pPY+pH"<<playerPosY+playerH<<std::endl;
//	std::cout<<"obsy-obsh"<<obsY-obsH<<std::endl;
//	std::cout<<"bottom"<<std::endl;
//	break;
//}	 
}

//============ghoul physics ==============
//ghoul->direction
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
}
//---------------------------------------------------
//check keys pressed now`
if (keys[XK_Left]) {
        g->player.setVelX(g->player.getVelX() - 4);
}
if (keys[XK_Right]) {
        g->player.setVelX(g->player.getVelX() + 4);
}
if (keys[XK_Up]) {
        if (!jump){
                g->player.setVelY(g->player.getVelY() + 15);
                jump = true;
                g->setCol(false);
                //std::cout<<"jump colision = false"<<std::endl;
        }
}
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
}

//std::cout<<"[PHYSICS END]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;

}

