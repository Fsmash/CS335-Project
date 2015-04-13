//cs335 Spring 2015
//
//program: project.cpp
//author:  Justin Ramirez
//date:    2015
//
//This program is a game starting point for 335
//
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
#include "ppm.h"
#include "log.h"
extern "C" {
	#include "fonts.h"
}

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((double)rand())/(double)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
							 (c)[1]=(a)[1]-(b)[1]; \
(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = 1.0f;
#define PI 3.141592653589793
#define ALPHA 1

//X Windows variables
Display *dpy;
Window win;
GLXContext glc;


//-----------------------------------------------------------------------------
//Setup timers
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

int xres=1250, yres=900;

struct Vect {
        float x, y, z;
};
struct Shape {
	float tipX;
	float tipY;
        float width, height;
        float radius;
	float color[3];
        Vect center;
};
struct Player {
	Vec dir;
	Vec pos;
	Vec vel;
	bool forward;
	float height;
	float width;
	float angle;
	float color[3];
	Player() {
		pos[0] = 100;
		pos[1] = 250;
	}
};
struct Ghoul {
	Vec pos;
	Vec vel;
	bool forward;
	float height;
	float width;
	float angle;
	float color[3];
	struct Ghoul *prev;
	struct Ghoul *next;
	Ghoul() {
		prev = NULL;
		next = NULL;
	}
};

struct Game {
	Ghoul *ghoulHead;
	int nGhouls;
	bool firstrun;
	bool firstrender;
	Player player;
	Ghoul ghoul[1];
	Shape floor;
	Shape whip;
	bool hit;
	bool col;
	Shape obstacle[4];
	Game() {
	}
};


int keys[65536];

//function prototypes
void initXWindows(void);
void init_opengl(void);
void cleanupXWindows(void);
void check_resize(XEvent *e);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void init(Game *g);
void init_sounds(void);
void physics(Game *game);
void render(Game *game);

int main(void)
{
	logOpen();
	initXWindows();
	init_opengl();
	Game game;
	init(&game);
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	int done=0;
	game.firstrun = true;
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
		game.firstrun = false;
		glXSwapBuffers(dpy, win);
	}
	cleanupXWindows();
	cleanup_fonts();
	logClose();
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
	initialize_fonts();
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
	g->col = false; //collision flag, fixes multiple jumps
	g->firstrun = true;
	g->firstrender =true;
	g->ghoulHead = NULL;
	g->nGhouls = 0;


	//create player
	g->player.width = 20.0f;
	g->player.height = 70.0f;
	g->player.forward=true;
	g->player.vel[0] = 0;
	g->player.vel[1] = 0;
	
	g->player.pos[0] = 100.0f;
	g->player.pos[1] = 350.0f;
	g->player.pos[2] = 0.0f;
	g->player.angle = 0.0;
	g->player.color[0] = 189.0/255.0;
	g->player.color[1] = 255.0/255.0;
	g->player.color[2] = 235.0/255.0;	
	//create whip
	g->hit = false;
	g->whip.width = 0.0f;
	g->whip.height = 5.0f;
	g->whip.center.x = g->player.pos[0];
	g->whip.center.y = g->player.pos[1] + 200;
	g->whip.color[0] = 80.0/255.0;
	g->whip.color[1] = 127.0/255.0;
	g->whip.color[2] = 40.0/255.0;
	g->whip.tipX = 0;
		
	//create floor
	g->floor.center.y = 0.0f;
	g->floor.center.x = 100.0f;
	g->floor.width = 1200.0f;
	g->floor.height = 100.0f; 
	g->floor.color[0] = 127.0/255.0;
	g->floor.color[1] = 60.0/255.0;
	g->floor.color[2] = 56.0/255.0;
//create ghouls
	Ghoul *gh = new Ghoul;
	gh->width = 20;
	gh->height = 70;
	gh->pos[0] = 700;
	gh->pos[1] = 150;
	gh->vel[0] = 2;
	gh->color[0] = 67.0/255.0;
	gh->color[1] = 200.0/255.0;
	gh->color[2] = 156.0/255.0;
	gh->next = g->ghoulHead;
	if (g->ghoulHead != NULL)
		g->ghoulHead->prev = gh;
	g->ghoulHead = gh;
	g->nGhouls++;
	
//ghoul2
	gh = new Ghoul;
	gh->width = 20;
	gh->height = 70;
	gh->pos[0] = 400;
	gh->pos[1] = 150;
	gh->vel[0] = 2;
	gh->color[0] = 67.0/255.0;
	gh->color[1] = 200.0/255.0;
	gh->color[2] = 156.0/255.0;
	gh->next = g->ghoulHead;
	if (g->ghoulHead != NULL)
		g->ghoulHead->prev = gh;
	g->ghoulHead = gh;
	g->nGhouls++;
//create obstacles
	g->obstacle[0].center.y = 120.0f;
	g->obstacle[0].center.x = 100.0f;
	g->obstacle[0].width = 40.0f;
	g->obstacle[0].height = 60.0f;
	g->obstacle[0].color[0] = 178.0/255.0;
	g->obstacle[0].color[1] = 100.0/255.0;
	g->obstacle[0].color[2] = 79.0/255.0;
		
	g->obstacle[1].center.y = 120.0f;
	g->obstacle[1].center.x = 600.0f;
	g->obstacle[1].width = 40.0f;
	g->obstacle[1].height = 45.0f;
	g->obstacle[1].color[0] = 255.0/255.0;
	g->obstacle[1].color[1] = 183.0/255.0;
	g->obstacle[1].color[2] = 164.0/255.0;
	
	g->obstacle[2].center.y = 130.0f;
	g->obstacle[2].center.x = 900.0f;
	g->obstacle[2].width = 40.0f;
	g->obstacle[2].height = 70.0f;
	g->obstacle[2].color[0] = 255.0/255.0;
	g->obstacle[2].color[1] = 183.0/255.0;
	g->obstacle[2].color[2] = 164.0/255.0;
		
	g->obstacle[3].center.y = 290.0f;
	g->obstacle[3].center.x = 1100.0f;
	g->obstacle[3].width = 80.0f;
	g->obstacle[3].height = 30.0f;
	g->obstacle[3].color[0] = 255.0/255.0;
	g->obstacle[3].color[1] = 183.0/255.0;
	g->obstacle[3].color[2] = 164.0/255.0;
}

void deleteGhoul(Game *g, Ghoul *node){
	if(node->prev == NULL){
		if (node->next == NULL){
			g->ghoulHead = NULL;
		} else {
			node->next->prev = NULL;
			g->ghoulHead = node->next;
		}
	} else {
		if (node->next == NULL){
			node->prev->next = NULL;
		} else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}

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
	if (shift){}
	switch(key) {
		case XK_Escape:
			return 1;
		case XK_f:
			break;
		case XK_s:
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
}


void physics(Game *g)
{	
/*=============player physics==============*/	
	std::cout<<"[PHYSICS BEG]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
	//player direction	
	if(g->player.vel[0] > 0){
		g->player.forward = true;
	}	
	else if(g->player.vel[0] < 0){
		g->player.forward = false;
	}
	if(g->firstrun){
		g->player.forward = true;
	}

	//is player jumping?
	bool jump = true;
	if (g->player.vel[1] == 0 && g->col){
		jump = false;
		g->col = false;
	}
	
	//apply gravity
	g->player.vel[1] -= gravity;

	//apply velocity in both directions
	if (g->player.vel[0] + g->player.pos[0] < 4000){
	std::cout<<"[PHYSICS 1]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
		g->player.pos[0] += g->player.vel[0];	
	std::cout<<"[PHYSICS 2]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
	}
	g->player.pos[1] += g->player.vel[1];
	
	//reset x velocity 
	g->player.vel[0] = 0;
	
	//update whip position/size with respect to player
	g->whip.center.x = g->player.pos[0];
	g->whip.center.y = g->player.pos[1];
	g->whip.width = 0;

	//floor collision
	if (g->player.pos[1] <= g->floor.center.y + g->floor.height +g->player.height -20){
		g->player.vel[1]=0;
		g->player.pos[1] = g->floor.center.y + g->floor.height + g->player.height -20;
		g->col = true;
	}

	//Check for collision with obstacles
	float playerW = g->player.width;
	float playerH = g->player.height;
	float playerPosY = g->player.pos[1];
	float playerPosX = g->player.pos[0];
	
	for (int i = 0; i < 5; i++){
		float obsY = g->obstacle[i].center.y;
		float obsX = g->obstacle[i].center.x;
		float obsH = g->obstacle[i].height;
		float obsW = g->obstacle[i].width;
		
		//top
		if ( (playerPosX > obsX-obsW-playerW+2 
				&& playerPosX < obsX+obsW+playerW-2) 
		    		&& (playerPosY <= obsY+obsH+playerH) 
				&& (playerPosY >= (obsY+obsH-1)) ){
			g->player.vel[1] = 0;
			g->player.pos[1] = obsY+obsH+playerH;
			g->col = true;
			break;
		}
		//left side
		if ( (playerPosX > (obsX - obsW - playerW) 
				&& playerPosX < obsX) 
				&& (playerPosY < obsY+obsH)){
			g->player.pos[0] = obsX-obsW-playerW;
			break;

		}
		//right side
		if ( playerPosX > obsX 
				&& playerPosX < obsX+obsW+playerW  
				&& (playerPosY < obsY + obsH )){ 
			g->player.pos[0] = obsX+obsW+playerW;
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
/*============ghoul physics ==============*/
	//ghoul->direction
	Ghoul *ghoul = g->ghoulHead;
	while(ghoul){	
		if( ghoul->vel[0] > 0){
			ghoul->forward = true;
		}	
		else if( ghoul->vel[0] < 0){
			ghoul->forward = false;
		}
		ghoul = ghoul->next;
	}
	
	ghoul = g->ghoulHead;
	while (ghoul){
		ghoul->vel[1] =- gravity;
		ghoul->pos[0] += ghoul->vel[0];	
		ghoul->pos[1] += ghoul->vel[1];
		//floor collision
		if (ghoul->pos[1] <= g->floor.center.y 
				+ g->floor.height + ghoul->height -20){
			//	std::cout<<g->player.vel[1]<<std::endl; //debug y-velocity
			ghoul->vel[1]=0;
			ghoul->pos[1] = g->floor.center.y 
				+ g->floor.height + ghoul->height -20;
	//	std::cout<<"floor collision = true"<<std::endl;	//debug floor collision
		}
		ghoul = ghoul->next;
	}
	//Check for ghoul->collision with obstacles
	ghoul = g->ghoulHead;
	while(ghoul){
		float ghoulW = ghoul->width;
		float ghoulH = ghoul->height;
		for (int i = 0; i < 5; i++){
			float ghoulPosY = ghoul->pos[1];
			float ghoulPosX = ghoul->pos[0];
			float obsY = g->obstacle[i].center.y;
			float obsX = g->obstacle[i].center.x;
			float obsH = g->obstacle[i].height;
			float obsW = g->obstacle[i].width;
			//std::cout<<"x=" <<g->ghoul->pos[0]<< "y= "<< g->ghoul->pos[1] <<std::endl;
			

			//top
			if ( (ghoulPosX > obsX-obsW-ghoulW && ghoulPosX < obsX+obsW+ghoulW) 
					&& (ghoulPosY <= obsY+obsH+ghoulH) 
					&& (ghoulPosY >= (obsY+obsH-1)) ){
				ghoul->vel[1] = 0;
				ghoul->pos[1] = obsY+obsH+ghoulH;
				std::cout<<"top"<<std::endl;
				g->col = true;
				break;
			}	 
			//left side
			if ( (ghoulPosX > (obsX - obsW - ghoulW) && ghoulPosX < obsX) 
					&& (ghoulPosY < obsY+obsH) ){
				//std::cout<<"left"<<std::endl;
				ghoul->vel[0] = -ghoul->vel[0];//obsX-obsW-ghoul->;
				break;
			}
			//right side
			if ( ghoulPosX > obsX && ghoulPosX < obsX+obsW+ghoulW && 
				ghoulPosY < obsY+obsH ){
				//std::cout<<"right"<<std::endl;
				ghoul->vel[0] = -ghoul->vel[0];//obsX-obsW-ghoul->;
				break;
			}
			//whip

			if ( (ghoulPosX-ghoulW <= g->whip.tipX) && (ghoulPosX+ghoulW >= g->whip.tipX ) && g->hit){
				std::cout<<"hitb"<<std::endl;
				std::cout<<"killb"<<std::endl;
				Ghoul *saveGhoul = ghoul;
				deleteGhoul(g,ghoul);
				ghoul = saveGhoul;
				g->nGhouls--;
				break;
			}	
		}
	ghoul = ghoul->next;
	}
	g->hit = false;	
	if (g->player.pos[0] < 0) {
		g->player.pos[0] += (float)xres;
	}
	else if (g->player.pos[0] > 0 +(float)xres) {
		g->player.pos[0] -= (float)xres;
	}
	//---------------------------------------------------
	//check keys pressed now`
	if (keys[XK_Left]) {
		g->player.vel[0] =- 4;
	}
	if (keys[XK_Right]) {
		g->player.vel[0] =+ 4;
	}
	if (keys[XK_Up]) {
		if (!jump){
			g->player.vel[1] =+ 15;
			jump = true;
			g->col = false;
			//std::cout<<"jump colision = false"<<std::endl;
		}
	}
	if (keys[XK_space]) {
		//whip moves to right
		if(g->player.forward){
			g->whip.center.x += 70;
			g->whip.width = 70;
			g->whip.tipX = g->whip.center.x + g->whip.width;
			std::cout <<g->whip.tipX<<std::endl;
			//std::cout<<g->whip.center.x<<std::endl;
			g->hit = true;
		}
		//whip moves to left
		if(!g->player.forward){
			g->whip.center.x -= 70;
			g->whip.width = 70;
			g->whip.tipX = g->whip.center.x - g->whip.width;
			std::cout <<g->whip.tipX<<std::endl;
			//std::cout<<g->whip.center.x<<std::endl;
			g->hit = true;
		}
	}

	std::cout<<"[PHYSICS END]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
}
void render(Game *g)
{
	glClear(GL_COLOR_BUFFER_BIT);
//whip
	if (g->hit){		
		glColor3fv(g->obstacle[0].color);
		glPushMatrix();
		glTranslatef(g->whip.center.x, g->whip.center.y+20, 0);
		glBegin(GL_QUADS);
		float w = g->whip.width;
		float h = g->whip.height;
		glVertex2f(-w, -h);
		glVertex2f(  -w, h);
		glVertex2f(  w, h);
		glVertex2f( w, -h);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}	

	glColor3fv(g->floor.color);
	glPushMatrix();
	glTranslatef(g->floor.center.x, g->floor.center.y, 0);
	//float angle = atan2(player.dir[1], player.dir[0]);
	glBegin(GL_QUADS);
	//glVertex2f(-10.0f, -10.0f);
	//glVertex2f(  0.0f, 20.0f);
	//glVertex2f( 10.0f, -10.0f);
	float w = g->floor.width;
	float h = g->floor.height;
	glVertex2f(-w, -h);
	glVertex2f(  -w, h);
	glVertex2f(  w, h);
	glVertex2f( w, -h);
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glEnd();
	glPopMatrix();

//obstacle
//	glClear(GL_COLOR_BUFFER_BIT);
	for(int i = 0; i < 5; i++){
		glColor3fv(g->obstacle[i].color);
		glPushMatrix();
		glTranslatef(g->obstacle[i].center.x, g->obstacle[i].center.y, 0.0f);
		//float angle = atan2(player.dir[1], player.dir[0]);
		glBegin(GL_QUADS);
		//glVertex2f(-10.0f, -10.0f);
		//glVertex2f(  0.0f, 20.0f);
		//glVertex2f( 10.0f, -10.0f);
		float w1 = g->obstacle[i].width;
		float h1 = g->obstacle[i].height;
		glVertex2f(-w1, -h1);
		glVertex2f(  -w1, h1);
		glVertex2f(  w1, h1);
		glVertex2f( w1, -h1);
		glEnd();
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(0.0f, 0.0f);
		glEnd();
		glPopMatrix();
	}

//Draw the player
	if (g->player.forward){
		glColor3fv(g->player.color);
		glPushMatrix();
		std::cout<<"g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;

		glTranslatef(g->player.pos[0], g->player.pos[1], 0.0f);
		//float angle = atan2(player.dir[1], player.dir[0]);
		//glRotatef(g->player.angle, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		//glVertex2f(-10.0f, -10.0f);
		//glVertex2f(  0.0f, 20.0f);
		//glVertex2f( 10.0f, -10.0f);
		w = g->player.width;
		h = g->player.height;
		glVertex2f(-w, -h);
		glVertex2f(-w,  h);
		glVertex2f( w,  h);
		glVertex2f( w, -h);
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
		glColor3fv(g->player.color);
		glPushMatrix();
		std::cout<<"[RENDER]g->player.pos[0]"<<g->player.pos[0]<<"g->player.pos[1]"<<g->player.pos[1]<<std::endl;
		glTranslatef(g->player.pos[0], g->player.pos[1], 0.0f);
		//float angle = atan2(player.dir[1], player.dir[0]);
//		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
		//glVertex2f(-10.0f, -10.0f);
		//glVertex2f(  0.0f, 20.0f);
		//glVertex2f( 10.0f, -10.0f);
		w = g->player.width;
		h = g->player.height;
		glVertex2f(-w, -h);
		glVertex2f(-w,  h);
		glVertex2f( w,  h);
		glVertex2f( w, -h);
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
	Ghoul *ghoul = g->ghoulHead;
	while(ghoul){
		if (ghoul->forward){
				glColor3fv(ghoul->color);
				glPushMatrix();
				glTranslatef(ghoul->pos[0], ghoul->pos[1], 0.0f);
				//float angle = atan2(player.dir[1], player.dir[0]);
//				glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
				glBegin(GL_QUADS);
				//glVertex2f(-10.0f, -10.0f);
				//glVertex2f(  0.0f, 20.0f);
				//glVertex2f( 10.0f, -10.0f);
				w = ghoul->width;
				h = ghoul->height;
				glVertex2f(-w, -h);
				glVertex2f(-w,  h);
				glVertex2f( w,  h);
				glVertex2f( w, -h);
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
				glColor3fv(ghoul->color);
				glPushMatrix();
				glTranslatef(ghoul->pos[0], ghoul->pos[1], 0.0f);
				//float angle = atan2(player.dir[1], player.dir[0]);
//				glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
				glBegin(GL_QUADS);
				//glVertex2f(-10.0f, -10.0f);
				//glVertex2f(  0.0f, 20.0f);
				//glVertex2f( 10.0f, -10.0f);
				w = ghoul->width;
				h = ghoul->height;
				glVertex2f(-w, -h);
				glVertex2f(-w,  h);
				glVertex2f( w,  h);
				glVertex2f( w, -h);
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



