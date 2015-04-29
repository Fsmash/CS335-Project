//#include "../Images"
//#include "../Images/ppm.h"
//#include "../Mechanics/Game.h"
#ifndef _ANIMATION_H_
#define _ANIMATION_H_


//Loading sprite images
Ppmimage *simonImage=NULL;
GLuint simonTexture;
GLuint silhouetteTexture;
int show_simon = 0;
int silhouette = 1;
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

void spriteAnimation(Game *g, int *keys) {
//float playerW;
    //float playerH;
    float wid = 60.0f;

    //Draw the player
    //glColor3fv(g->player.getColor());
    glPushMatrix();
    //std::cout<<"g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g-    >player.pos[1]<<std::endl;

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
    if (keys[XK_Right]) {
        glTexCoord2f(0.0f, 0.055f); glVertex2f(-wid, -wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
        glTexCoord2f(0.055f, 0.0f); glVertex2f(wid, wid);
        glTexCoord2f(0.055f, 0.055f); glVertex2f(wid, -wid);
    } else if(keys[XK_Left]) {
        glTexCoord2f(0.055f, 0.055f); glVertex2f(-wid, -wid);
        glTexCoord2f(0.055f, 0.0f); glVertex2f(-wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(wid, wid);
        glTexCoord2f(0.0f, 0.055f); glVertex2f(wid, -wid);
    } else if (keys[XK_Down]) {
        glTexCoord2f(0.055f, 0.055f); glVertex2f(-wid, -wid);
        glTexCoord2f(0.055f, 0.0f); glVertex2f(-wid, wid);
        glTexCoord2f(0.11f, 0.0f); glVertex2f(wid, wid);
        glTexCoord2f(0.11f, 0.055f); glVertex2f(wid, -wid);
        g->player.setVel(0,0);
    } else {
        if(g->player.getFwd()) {
            glTexCoord2f(0.0f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.055f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.055f, 0.055f); glVertex2f(wid, -wid);
        } else {
            glTexCoord2f(0.055f, 0.055f); glVertex2f(-wid, -wid);
            glTexCoord2f(0.055f, 0.0f); glVertex2f(-wid, wid);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(wid, wid);
            glTexCoord2f(0.0f, 0.055f); glVertex2f(wid, -wid);
        }
    }
    glEnd();
    glPopMatrix();
}

#endif //_Animation_H_
