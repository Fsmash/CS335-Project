//#include "../Images"
//#include "../Images/ppm.h"
//#include "../Mechanics/Game.h"
#ifndef _ANIMATION_H_
#define _ANIMATION_H_


//Loading sprite images
Ppmimage *simonImage=NULL;
Ppmimage *blockImage=NULL;
Ppmimage *backImage=NULL;
GLuint simonTexture;
GLuint silhouetteTexture;
GLuint blockTexture;
GLuint backTexture;
int show_simon = 0;
int silhouette = 1;
int xWid = 750;
float xRes = 1250;
float yRes = 900;
//Setup time functions
double frameCountdown= 0.0;
//-----------------------------------------------------------------------------
///Check each pixel and change red pixels to black
void setColorBlack(Ppmimage *img) 
{
    int a, b , c;
    int w = img->width;
    int h = img->height;
    unsigned char *p, *ptr = (unsigned char *)img->data;
    p = ptr;
    for (int i=0; i<h; i++) {
        for (int j=0; j<w; j++) {
            a = *(p+0);
            b = *(p+1);
            c = *(p+2);
            if(a == 128 && b == 0 && c == 0) {
                *(p+0) = 0;
                *(p+1) = 0;
                *(p+2) = 0;
            }
            p+=3;
        }
    }    
}
//-----------------------------------------------------------------------------
//Build data to display sprite through texture 
unsigned char *buildAlphaData(Ppmimage *img)
{
    //add 4th component to RGB stream...
    int a,b,c;
    unsigned char *newdata, *ptr;
    unsigned char *data = (unsigned char *)img->data;
    //newdata = (unsigned char *)malloc(img->width * img->height * 4);
    newdata = new unsigned char[img->width * img->height * 4];
    ptr = newdata;
    for (int i=0; i<img->width * img->height * 3; i+=3) {
        a = *(data+0);
        b = *(data+1);
        c = *(data+2);
        *(ptr+0) = a;
        *(ptr+1) = b;
        *(ptr+2) = c;
        //
        //get the alpha value
        //
        //original code
        //get largest color component...
        //*(ptr+3) = (unsigned char)((
        //      (int)*(ptr+0) +
        //      (int)*(ptr+1) +
        //      (int)*(ptr+2)) / 3);
        //d = a;
        //if (b >= a && b >= c) d = b;
        //if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //
        //new code, suggested by Chris Smith, Fall 2013
        *(ptr+3) = (a|b|c);
        //
        ptr += 4;
        data += 3;
    }
    return newdata;
}
//--------------------------------------------------------------------------
//Function to organize sprite mapping and animations
void blockSprite(Block *b) {
    
    glPushMatrix();
    float wid = b->getWidth();
   
    glTranslatef(b->getCenterX(), b->getCenterY(), 0.0f);
    
    glBindTexture(GL_TEXTURE_2D, blockTexture);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.0); glVertex2f(-wid, -wid);
    glTexCoord2f(0.0, 0.95); glVertex2f(-wid, wid);
    glTexCoord2f(1.0, 0.95); glVertex2f(wid, wid);
    glTexCoord2f(1.0, 0.0); glVertex2f(wid, -wid);
    
    glEnd();
    glPopMatrix();
}

void backGround() {
    
    glPushMatrix();
   
    glTranslatef((xRes / 2), (yRes / 2) - 200, 0.0f);
    
    glBindTexture(GL_TEXTURE_2D, backTexture);
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 0.0); glVertex2f(-xWid, -xWid);
    glTexCoord2f(0.0, 1.0); glVertex2f(-xWid, xWid);
    glTexCoord2f(1.0, 1.0); glVertex2f(xWid, xWid);
    glTexCoord2f(1.0, 0.0); glVertex2f(xWid, -xWid);
    
    glEnd();
    glPopMatrix();
}

void spriteAnimation(Game *g, int *keys, const double frameRate, double timeSpan) 
{
    //float playerW;
    //float playerH;
    float wid = 60.0f;

    //Draw the player
    //glColor3fv(g->player.getColor());
    glPushMatrix();
    //std::cout<<"g->player.pos[0]"<<g->player.getPosX()<<"g->player.pos[1]"<<g->player.getPosY()<<std::endl;

    glTranslatef(g->player.getPosX(), g->player.getPosY(), 0.0f);
    if (!silhouette) {
        glBindTexture(GL_TEXTURE_2D, simonTexture);
    } else {
        glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glColor4ub(255,255,255,255);
    }

    glBegin(GL_QUADS);
    //playerW = g->player.getWidth();
    //playerH = g->player.getHeight();
    //walk right
    if (keys[XK_Right] || keys['d']) {
        frameCountdown += timeSpan;
        if(frameCountdown < (10*frameRate)) {
            glTexCoord2f(0.180f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.180f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.218f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.218f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (10*frameRate) && frameCountdown < (20*frameRate)) {
            glTexCoord2f(0.216f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.216f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.257f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.257f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (20*frameRate) && frameCountdown < (30*frameRate)) {
            glTexCoord2f(0.257f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.257f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.294f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.294f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (30*frameRate) && frameCountdown < (40*frameRate)) {
            glTexCoord2f(0.294f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.294f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.329f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.329f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (40*frameRate) && frameCountdown < (50*frameRate)) {
            glTexCoord2f(0.329f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.329f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.359f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.359f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown >= (50*frameRate)) {            
            glTexCoord2f(0.359f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.359f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.393f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.393f, 0.055f); glVertex2f(wid, -wid);
            if(frameCountdown >= (60*frameRate)) {
                frameCountdown = 0.0;
            }
        }
    } else if(keys[XK_Left] || keys['a']) {
        frameCountdown += timeSpan;
        if(frameCountdown < (10*frameRate)) {
            glTexCoord2f(0.218f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.218f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.180f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.180f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (10*frameRate) && frameCountdown < (20*frameRate)) {
            glTexCoord2f(0.257f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.257f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.216f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.216f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (20*frameRate) && frameCountdown < (30*frameRate)) {
            glTexCoord2f(0.294f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.294f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.257f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.257f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (30*frameRate) && frameCountdown < (40*frameRate)) {
            glTexCoord2f(0.329f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.329f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.294f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.294f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown > (40*frameRate) && frameCountdown < (50*frameRate)) {
            glTexCoord2f(0.359f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.359f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.329f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.329f, 0.055f); glVertex2f(wid, -wid);
        } else if(frameCountdown >= (50*frameRate)) {            
            glTexCoord2f(0.393f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.393f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.359f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.359f, 0.055f); glVertex2f(wid, -wid);
            if(frameCountdown >= (60*frameRate)) {
                frameCountdown = 0.0;
            }
        }   
    } else if (keys[XK_Down] || keys['s']) {
        if(g->player.getFwd()) {
            glTexCoord2f(0.071f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.071f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.11f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.11f, 0.055f); glVertex2f(wid, -wid);
        } else {
            glTexCoord2f(0.11f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.11f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.071f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.071f, 0.055f); glVertex2f(wid, -wid);
        }
    } else {
        if(g->player.getFwd()) {
            glTexCoord2f(0.016f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.016f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.055f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.055f, 0.055f); glVertex2f(wid, -wid);
        } else {
            glTexCoord2f(0.055f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.055f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.016f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.016f, 0.055f); glVertex2f(wid, -wid);
        }
    }      
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

#endif //_Animation_H_
