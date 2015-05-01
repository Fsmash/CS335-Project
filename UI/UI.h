//#include "../Mechanics/Game.h"
#ifndef _UI_H_
#define _UI_H_

Ppmimage *uiImage=NULL;
GLuint uiTexture;

void renderHealth(Game *g, int xr, int yr);

void renderHealth(Game *g, int xr, int yr)
{
	int xres = xr;
	int yres = yr;
	float healthW = 55;	
	float healthH = 7;	

	glPushMatrix();
//	glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
	glTranslatef(60, yres-20, 0.0f);
	glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
	glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(225,225,225,225);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,1.0f);glVertex2f(-healthW, -healthH);
	glTexCoord2f(0.0f,0.5f);glVertex2f(-healthW, healthH);
	glTexCoord2f(1.0f,0.5f);glVertex2f(healthW, healthH);
	glTexCoord2f(1.0f,1.0f);glVertex2f(healthW, -healthH);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
	glPopMatrix();

	int width = 5;
	int height = 10;
	int x = 130;
	int y = 20;
	int spacing = 15;

	for (int i = 0; i < 15; i++) {
        glPushMatrix();
		glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
        glTranslatef(x+(i*spacing), yres-y, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-width, -height);
        glVertex2f(-width, height);
        glVertex2f(width, height);
        glVertex2f(width, -height);
        glEnd();
        glColor3f(1.0f, 0.0f, 0.0f);
        glPopMatrix();
	}
	for (int i = 0; i < g->player.getHealth(); i++) {
        glPushMatrix();
        glColor3f(255/255.0,0.0,0.0);
        glTranslatef(x+(i*spacing), yres-y, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-width, -height);
        glVertex2f(-width, height);
        glVertex2f(width, height);
        glVertex2f(width, -height);
        glEnd();
        glPopMatrix();
	}
}


#endif //_UI_H_
