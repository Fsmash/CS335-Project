//#include "../Mechanics/Game.h"
#ifndef _UI_H_
#define _UI_H_

Ppmimage *uiImage=NULL;
GLuint uiTexture;

void renderScore(Game *g, int xr, int yr, int score);
void renderHealth(Game *g, int xr, int yr);
void renderItemSlot(Game *g, int xr, int yr);

void renderScore(Game *g, int xr, int yr, int score){
	int xres = xr;
	int yres = yr;
	float scoreW = 58;	
	float scoreH = 12;	
//score text ------------------------------------------------------
	glPushMatrix();
	glTranslatef(190, yres-83, 0.0f);
	glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
	glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(225,225,225,225);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.25f);glVertex2f(-scoreW, -scoreH);
	glTexCoord2f(0.0f,0.0f);glVertex2f(-scoreW, scoreH);
	glTexCoord2f(0.5f,0.0f);glVertex2f(scoreW, scoreH);
	glTexCoord2f(0.5f,0.25f);glVertex2f(scoreW, -scoreH);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
	glPopMatrix();
//calculate numbers in score --------------------------------------------------
	int tmp = g->player.getScore();
	int numbers[10];
	for (int i = 10; i > 0;i--) {
	    numbers[i-1] = tmp%10;	
		tmp = tmp/10;	
	}
	
//render numbers --------------------------------------------------------------
	int width = 12;
	int height = 14;
	int x = 274;
	int y = 83;
	int spacing = 23;

	for (int i = 0; i < 10; i++) {
		if (numbers[i] == 0){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.39f,1.0f); glVertex2f(-width, -height);
			glTexCoord2f(0.39f,0.75f); glVertex2f(-width, height);
			glTexCoord2f(0.5f,0.75f);glVertex2f(width, height);
			glTexCoord2f(0.5f,1.0f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 9){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.3f,1.0f); glVertex2f(-width, -height);
			glTexCoord2f(0.3f,0.75f); glVertex2f(-width, height);
			glTexCoord2f(0.39f,0.75f);glVertex2f(width, height);
			glTexCoord2f(0.39f,1.0f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 8){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.19f,1.0f); glVertex2f(-width, -height);
			glTexCoord2f(0.19f,0.75f); glVertex2f(-width, height);
			glTexCoord2f(0.29f,0.75f);glVertex2f(width, height);
			glTexCoord2f(0.29f,1.0f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 7){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.1f,1.0f); glVertex2f(-width, -height);
			glTexCoord2f(0.1f,0.75f); glVertex2f(-width, height);
			glTexCoord2f(0.19f,0.75f);glVertex2f(width, height);
			glTexCoord2f(0.19f,1.0f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 6){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f); glVertex2f(-width, -height);
			glTexCoord2f(0.0f,0.75f); glVertex2f(-width, height);
			glTexCoord2f(0.09f,0.75f);glVertex2f(width, height);
			glTexCoord2f(0.09f,1.0f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 5){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.39f,0.75f); glVertex2f(-width, -height);
			glTexCoord2f(0.39f,0.5f); glVertex2f(-width, height);
			glTexCoord2f(0.5f,0.5f);glVertex2f(width, height);
			glTexCoord2f(0.5f,0.75f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 4){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.29f,0.75f); glVertex2f(-width, -height);
			glTexCoord2f(0.29f,0.5f); glVertex2f(-width, height);
			glTexCoord2f(0.39f,0.5f);glVertex2f(width, height);
			glTexCoord2f(0.39f,0.75f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 3){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.19f,0.75f); glVertex2f(-width, -height);
			glTexCoord2f(0.19f,0.5f); glVertex2f(-width, height);
			glTexCoord2f(0.29f,0.5f);glVertex2f(width, height);
			glTexCoord2f(0.29f,0.75f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 2){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.1f,0.75f); glVertex2f(-width, -height);
			glTexCoord2f(0.1f,0.5f); glVertex2f(-width, height);
			glTexCoord2f(0.19f,0.5f);glVertex2f(width, height);
			glTexCoord2f(0.19f,0.75f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
		else if (numbers[i] == 1){
			glPushMatrix();
			glTranslatef(x+(i*spacing), yres-y, 0.0f);
			glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
			glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, 0.0f);
			glColor4ub(225,225,225,225);
			glBindTexture(GL_TEXTURE_2D, uiTexture);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.75f); glVertex2f(-width, -height);
			glTexCoord2f(0.0f,0.5f); glVertex2f(-width, height);
			glTexCoord2f(0.080f,0.5f);glVertex2f(width, height);
			glTexCoord2f(0.080f,0.75f);glVertex2f(width, -height);
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_ALPHA_TEST);
			glColor3f(1.0f, 0.0f, 0.0f);
			glPopMatrix();
		}	
	}
}


void renderHealth(Game *g, int xr, int yr)
{
//health text ------------------------------------------------------
	int xres = xr;
	int yres = yr;
	float healthW = 58;	
	float healthH = 12;	

	glPushMatrix();
	glTranslatef(190, yres-47, 0.0f);
	glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
	glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(225,225,225,225);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.5f);glVertex2f(-healthW, -healthH);
	glTexCoord2f(0.0f,0.25f);glVertex2f(-healthW, healthH);
	glTexCoord2f(0.5f,0.25f);glVertex2f(healthW, healthH);
	glTexCoord2f(0.5f,0.5f);glVertex2f(healthW, -healthH);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
	glPopMatrix();
//render health bars ----------------------------------------------------------
	int width = 5;
	int height = 12;
	int x = 270;
	int y = 45;
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


void renderItemSlot(Game *g, int xr, int yr){
//render item frame -----------------------------------------------------------
	int xres = xr;
	int yres = yr;
	float itemSlotW = 75;	
	float itemSlotH = 55;	

	glPushMatrix();
//	glColor3f(155.0/255.0,169.0/255.0,178.0/255.0);
	glTranslatef(55, yres-70, 0.0f);
	glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
	glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
	glColor4ub(225,225,225,225);
    glBindTexture(GL_TEXTURE_2D, uiTexture);
	glBegin(GL_QUADS);
	glTexCoord2f(0.5f,0.8f);glVertex2f(-itemSlotW, -itemSlotH);
	glTexCoord2f(0.5f,0.0f);glVertex2f(-itemSlotW, itemSlotH);
	glTexCoord2f(0.855f,0.0f);glVertex2f(itemSlotW, itemSlotH);
	glTexCoord2f(0.855f,0.8f);glVertex2f(itemSlotW, -itemSlotH);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
	glPopMatrix();



}
#endif //_UI_H_
