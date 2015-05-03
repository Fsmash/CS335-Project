#include "Game.h"
#include <X11/keysym.h>
int x=1250, y=900;

// this allows for dashing, if you couldnt figure it out
extern int dash = 0;

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
    //if (g->player.getVelX() + g->player.getPosX() < 70*80){ //columns * blockwidth
    g->player.setPosX(g->player.getPosX() + g->player.getVelX());	
    //}

    g->player.setPosY(g->player.getPosY() + g->player.getVelY());

    //reset x velocity 
    g->player.setVelX(0);
}

void characterCollision(Game *g) {
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
    //for (int i = 0; i < g->nBlocks-10; i++) {
	while(block){
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
            if (block->getClimb())
                g->setCol(true);
        }
        
        //left side
        if ( (playerX > (blockX - blockW - playerW) 
                    && (playerX < blockX)) 
                && (playerY < (blockY + blockH))
                && (playerY > (blockY - blockH))
                && !(block->getAdjLeft())) {
            g->player.setPosX(blockX - blockW - playerW);
			cout<<"here"<<endl;
        }

        //right side
        if ( playerX < (blockX + blockW + playerW) 
                && (playerX > blockX) 
                && (playerY < (blockY + blockH ))
                && (playerY > (blockY - blockH ))
                && !(block->getAdjRight())) {
            //cout << "block x " << blockX << " y " << blockY << "pushing your shit" << endl;
            g->player.setPosX(blockX + blockW + playerW);
        }
        block = block->next;
    }
}

void applyKey(Game *g, int *keys) {

    //check keys pressed now`
    if (keys[XK_Left] || keys['a']) {
        if(dash)
            g->player.setVelX(g->player.getVelX() - 12);
        else
            g->player.setVelX(g->player.getVelX() - 4);
    }
    if (keys[XK_Right] || keys['d']) {
        if(dash)
            g->player.setVelX(g->player.getVelX() + 12);
        else
            g->player.setVelX(g->player.getVelX() + 4);
    }

    if (keys[XK_space] || keys['w'] || keys[XK_Up]) {
        if (!g->player.getJump()) {
            if(dash)
                g->player.setVelY(g->player.getVelY() + 18);
            else
                g->player.setVelY(g->player.getVelY() + 10);
            g->player.setJump(true);
            g->setCol(false);
        }
    }
}


