//#include "../Mechanics/Game.h"
#ifndef _UI_H_
#define _UI_H_

void renderUI(Game *g);

void renderUI(Game *g)
{
	int width = 5;
	int height = 10;
	int x = 20;
	int xres = 1250;
	int y = 210;
	int yres = 900;
	int spacing = 15;

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
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
        glEnd();
        glPopMatrix();
	}
}


#endif //_UI_H_
