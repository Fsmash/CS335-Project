//cs335 Spring 2015
//
//program: project.cpp
//author:  Justin Ramirez
//date:    2015
//
//This program is a game starting point for 335
//

//libraries
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>    
//#include <GL/gl.h>    
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "Images/ppm.h"
//#include "log.h"
#include "Mechanics/Game.h"
#include <ctype.h>

/*extern "C" {
#include "fonts.h"
}*/
//-----------------------------------------------------------------------------

//macros, not used
/*
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
(c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
 */
//-----------------------------------------------------------------------------

//constants
//const float timeslice = 1.0f;
const float gravity = 1.0f;
#define PI 3.141592653589793
#define ALPHA 1
//-----------------------------------------------------------------------------

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;
//-----------------------------------------------------------------------------

//Setup timers,                             time not used

const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
struct timespec timeStart, timeCurrent;
struct timespec timePause;
double physicsCountdown=0.0;
double timeSpan=0.0;

//unsigned int upause=0;

double timeDiff(struct timespec *start, struct timespec *end) {
        return (double)(end->tv_sec - start->tv_sec ) +
                (double)(end->tv_nsec - start->tv_nsec) * oobillion;
}

void timeCopy(struct timespec *dest, struct timespec *source) {
        memcpy(dest, source, sizeof(struct timespec));
}

//-----------------------------------------------------------------------------

//Global variables
int xres=1250, yres=900;
int keys[65536];
//-----------------------------------------------------------------------------

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void init_sounds(void);

void init(Game *g);
void physics(Game *game);
void render(Game *game);
//-----------------------------------------------------------------------------

int main(void)
{
        //logOpen();
        initXWindows();
        init_opengl();
        Game game;
        init(&game);
        srand(time(NULL));
        clock_gettime(CLOCK_REALTIME, &timePause);
        clock_gettime(CLOCK_REALTIME, &timeStart);
        int done=0;
        game.setRun(true);
        while (!done) {
                while (XPending(dpy)) {
                        XEvent e;
                        XNextEvent(dpy, &e);
                        check_resize(&e);
                        check_mouse(&e);
                        done = check_keys(&e);
                }
                clock_gettime(CLOCK_REALTIME, &timeCurrent);
                timeSpan = timeDiff(&timeStart, &timeCurrent);
                timeCopy(&timeStart, &timeCurrent);
                physicsCountdown += timeSpan;
                while (physicsCountdown >= physicsRate) {
                        physics(&game);
                        physicsCountdown -= physicsRate;
                }
                render(&game);
                game.setRun(false);
                glXSwapBuffers(dpy, win);
        }
        cleanupXWindows();
        //cleanup_fonts();
        //logClose();
        return 0;
}

void cleanupXWindows(void)
{
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
}

void set_title(void)
{
        //Set the window title bar.
        XMapWindow(dpy, win);
        XStoreName(dpy, win, "CS335 - Project template");
}

void setup_screen_res(const int w, const int h)
{
        xres = w;
        yres = h;
}

void initXWindows(void)
{
        GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
        //GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
        XSetWindowAttributes swa;
        setup_screen_res(xres, yres);
        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
                std::cout << "\n\tcannot connect to X server" << std::endl;
                exit(EXIT_FAILURE);
        }
        Window root = DefaultRootWindow(dpy);
        XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
        if (vi == NULL) {
                std::cout << "\n\tno appropriate visual found\n" << std::endl;
                exit(EXIT_FAILURE);
        } 
        Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                StructureNotifyMask | SubstructureNotifyMask;
        win = XCreateWindow(dpy, root, 0, 0, xres, yres, 0,
                        vi->depth, InputOutput, vi->visual,
                        CWColormap | CWEventMask, &swa);
        set_title();
        glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);
}

void reshape_window(int width, int height)
{
        //window has been resized.
        setup_screen_res(width, height);
        //
        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        glOrtho(0, xres, 0, yres, -1, 1);
        set_title();
}

void init_opengl(void)
{
        //OpenGL initialization
        glViewport(0, 0, xres, yres);
        //Initialize matrices
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        //This sets 2D mode (no perspective)
        glOrtho(0, xres, 0, yres, -1, 1);
        //
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_FOG);
        glDisable(GL_CULL_FACE);
        //
        //Clear the screen to black
        glClearColor(0.0, 0.0, 0.0, 1.0);
        //Do this to allow fonts
        glEnable(GL_TEXTURE_2D);
        //initialize_fonts();
}

void check_resize(XEvent *e)
{
        //The ConfigureNotify is sent by the
        //server if the window is resized.
        if (e->type != ConfigureNotify)
                return;
        XConfigureEvent xce = e->xconfigure;
        if (xce.width != xres || xce.height != yres) {
                //Window size did change.
                reshape_window(xce.width, xce.height);
        }
}

void init(Game *g) {
        memset(keys, 0, 65536);
        //set some initial parameters
        g->player.setPos(100.0f, 350.0f);     

        g->initWhip();          
        g->whip.setCenterX(g->player.getPosX());         
        g->whip.setCenterY(g->player.getPosY() + 200);         

        g->initFloor();       

        //create ghouls
        g->createGhoul(); 
        //ghoul2
        g->createGhoul(); 

        //create obstacles
        g->initObstacles();
      
}

/*      not used
void normalize(Vec v) {
        Flt len = v[0]*v[0] + v[1]*v[1];
        if (len == 0.0f) {
                v[0] = 1.0;
                v[1] = 0.0;
                return;
        }
        len = 1.0f / sqrt(len);
        v[0] *= len;
        v[1] *= len;
}
*/

void check_mouse(XEvent *e)
{
        //Did the mouse move?
        //Was a mouse button clicked?
        static int savex = 0;
        static int savey = 0;
        //
        if (e->type == ButtonRelease) {
                return;
        }
        if (e->type == ButtonPress) {
                if (e->xbutton.button==1) {
                        //Left button is down
                }
                if (e->xbutton.button==3) {
                        //Right button is down
                }
        }
        if (savex != e->xbutton.x || savey != e->xbutton.y) {
                //Mouse moved
                savex = e->xbutton.x;
                savey = e->xbutton.y;
        }
}

int check_keys(XEvent *e)
{
        //keyboard input?
        static int shift=0;
        int key = XLookupKeysym(&e->xkey, 0);
        //Log("key: %i\n", key);
        if (e->type == KeyRelease) {
                keys[key]=0;
                if (key == XK_Shift_L || key == XK_Shift_R)
                        shift=0;
                return 0;
        }
        if (e->type == KeyPress) {
                keys[key]=1;
                if (key == XK_Shift_L || key == XK_Shift_R) {
                        shift=1;
                        return 0;
                }
        } else {
                return 0;
        }
        // if (shift){}                     not used
        switch(key) {
                case XK_Escape:
                        return 1;
                        /*  case XK_f:
                            break;
                            case XK_s:
                            break;
                            case XK_Down:
                            break;
                            case XK_equal:
                            break;
                            case XK_minus:
                            break;*/
        }
        return 0;
}


void physics(Game *g)
{	
        /*=============player physics==============*/	
        //std::cout<<"[PHYSICS BEG]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
        //player direction
        
           if(g->player.getVelX() > 0){
           g->player.setFwd(true);
           }	

           else if(g->player.getVelX() < 0){
           g->player.setFwd(false);
           }

           if(g->getRun()){
           g->player.setFwd(true);
           }

        //is player jumping?
        bool jump = true;
        if (g->player.getVelY() == 0 && g->getCol()){
        jump = false;
        g->setCol(false);
        }

        //apply gravity
        g->player.applyGrav(gravity);

        //apply velocity in both directions
        if (g->player.getVelX() + g->player.getPosX() < 4000){
        //std::cout<<"[PHYSICS 1]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
        g->player.setPosX(g->player.getPosX() + g->player.getVelX());	
        //std::cout<<"[PHYSICS 2]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
        }
        g->player.setPosY(g->player.getPosY() + g->player.getVelY());

        //reset x velocity 
        g->player.setVelX(0);

        //update whip position/size with respect to player
        g->updateWhip();
        //g->whip.setCenterX(g->player.getPosX());
        //g->whip.setCenterY(g->player.getPosY());
        g->whip.setWidth(0);

        //floor collision
        if (g->player.getPosY() <= g->floor.getCenterY() + g->floor.getHeight() + g->player.getHeight() - 20) {
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
void render(Game *g)
{	
	
	glClear(GL_COLOR_BUFFER_BIT);
	const int xSize = 20;
	const int ySize = 12;
	int blockWidth = 40;
	int blockHeight = 40;	
	char **map;
	char *lvl = "Levels/lvl1.txt";
	map = g->getLevel(lvl,12,20);


	
	for (int y = 0; y < ySize; y++){
		for(int x = 0; x < xSize; x++){
			if (!isspace(map[y][x])){
        		//render square
               	if (map[y][x] == '#')
                glColor3f(0.0f,1.0f,0.0f);
               	else {
                glColor3f(1.0f,1.0f,0.0f);
                }
                glPushMatrix();
                //glTranslatef(g->obstacle[i].getCenterX(), g->obstacle[i].getCenterY(), 0.0f);
                glTranslatef(((2*blockWidth*x)+blockWidth), (yres-(2*blockHeight*y)-blockHeight), 0.0f);
                glBegin(GL_QUADS);
                glVertex2f(-blockWidth, -blockHeight);
                glVertex2f(-blockWidth, blockHeight);
                glVertex2f(blockWidth, blockHeight);
                glVertex2f(blockWidth, -blockHeight);
                glEnd();
                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_POINTS);
                glVertex2f(0.0f, 0.0f);
                glEnd();
                glPopMatrix();
        	}
		}
	}
}

/*
        //whip
        if (g->getHit()){		
                glColor3fv(g->obstacle[0].getColor());
                glPushMatrix();
                glTranslatef(g->whip.getCenterX(), g->whip.getCenterY() + 20, 0);
                glBegin(GL_QUADS);
                float whipW = g->whip.getWidth();
                float whipH = g->whip.getHeight();
                glVertex2f(-whipW, -whipH);
                glVertex2f(-whipW, whipH);
                glVertex2f(whipW, whipH);
                glVertex2f(whipW, -whipH);
                glEnd();
                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_POINTS);
                glVertex2f(0.0f, 0.0f);
                glEnd();
                glPopMatrix();
        }	

        glColor3fv(g->floor.getColor());
        glPushMatrix();
        glTranslatef(g->floor.getCenterX(), g->floor.getCenterY(), 0);
        glBegin(GL_QUADS);
        float floorW = g->floor.getWidth();
        float floorH = g->floor.getHeight();
        glVertex2f(-floorW, -floorH);
        glVertex2f(-floorW, floorH);
        glVertex2f(floorW, floorH);
        glVertex2f(floorW, -floorH);
        glEnd();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(0.0f, 0.0f);
        glEnd();
        glPopMatrix();

        
        float playerW;
        float playerH;

        //Draw the player
        if (g->player.getFwd()) {
                glColor3fv(g->player.getColor());
                glPushMatrix();
                //std::cout<<"g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;

                glTranslatef(g->player.getPosX(), g->player.getPosY(), 0.0f);
                glBegin(GL_QUADS);
                
                playerW = g->player.getWidth();
                playerH = g->player.getHeight();
                
                glVertex2f(-playerW, -playerH);
                glVertex2f(-playerW, playerH);
                glVertex2f(playerW, playerH);
                glVertex2f(playerW, -playerH);
                glEnd();
                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_QUADS);
                glVertex2f(-10.0f, 40.0f);
                glVertex2f(-10.0f,65.00f);
                glVertex2f( 20.0f, 65.0f);
                glVertex2f( 20.0f, 40.0f);
                glEnd();
                glPopMatrix();

        }
        else{
                glColor3fv(g->player.getColor());
                glPushMatrix();
                //std::cout<<"[RENDER]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
                glTranslatef(g->player.getPosX(), g->player.getPosY(), 0.0f);
                glBegin(GL_QUADS);

                playerW = g->player.getWidth();
                playerH = g->player.getHeight();

                glVertex2f(-playerW, -playerH);
                glVertex2f(-playerW, playerH);
                glVertex2f(playerW, playerH);
                glVertex2f(playerW, -playerH);
                glEnd();
                glColor3f(1.0f, 0.0f, 0.0f);
                glBegin(GL_QUADS);
                glVertex2f(-20.0f, 40.0f);
                glVertex2f(-20.0f,65.00f);
                glVertex2f( 10.0f, 65.0f);
                glVertex2f( 10.0f, 40.0f);
                glEnd();
                glPopMatrix();

        }

        float ghoulW;
        float ghoulH;
        Ghouls *ghoul = g->ghoulHead;
        while(ghoul){
                if (ghoul->getFwd()){
                        glColor3fv(ghoul->getColor());
                        glPushMatrix();
                        glTranslatef(ghoul->getPosX(), ghoul->getPosY(), 0.0f);
                        glBegin(GL_QUADS);
                       
                        ghoulW = ghoul->getWidth();
                        ghoulH = ghoul->getHeight();
                        
                        glVertex2f(-ghoulW, -ghoulH);
                        glVertex2f(-ghoulW, ghoulH);
                        glVertex2f(ghoulW, ghoulH);
                        glVertex2f(ghoulW, -ghoulH);
                        glEnd();
                        glColor3f(1.0f, 0.0f, 0.0f);
                        glBegin(GL_QUADS);
                        glVertex2f(-10.0f, 40.0f);
                        glVertex2f(-10.0f,65.00f);
                        glVertex2f( 20.0f, 65.0f);
                        glVertex2f( 20.0f, 40.0f);
                        glEnd();
                        glPopMatrix();

                }
                else{
                        //std::cout<<"backwards";
                        glColor3fv(ghoul->getColor());
                        glPushMatrix();
                        glTranslatef(ghoul->getPosX(), ghoul->getPosY(), 0.0f);
                        glBegin(GL_QUADS);

                        ghoulW = ghoul->getWidth();
                        ghoulH = ghoul->getHeight();
                        
                        glVertex2f(-ghoulW, -ghoulH);
                        glVertex2f(-ghoulW, ghoulH);
                        glVertex2f(ghoulW, ghoulH);
                        glVertex2f(ghoulW, -ghoulH);
                        glEnd();
                        glColor3f(1.0f, 0.0f, 0.0f);
                        glBegin(GL_QUADS);
                        glVertex2f(-20.0f, 40.0f);
                        glVertex2f(-20.0f,65.00f);
                        glVertex2f( 10.0f, 65.0f);
                        glVertex2f( 10.0f, 40.0f);
                        glEnd();
                        glPopMatrix();

                }
                ghoul = ghoul->next;
        }
}

*/

