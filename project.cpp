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
#include <GL/glu.h>
#include "Images/ppm.h"
//#include "log.h"
//#include "Mechanics/Game.h"
#include "Mechanics/Physics.h"
#include "Animation/Animation.h"
#include "UI/UI.h"
#include <ctype.h>
#include "FMOD/fmod.h"

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
    if(!fmod_init()) return -1;
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

        // must update fmod often
        fmod_systemupdate();
    }
    cleanupXWindows();
    fmod_cleanup();
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
    simonImage = ppm6GetImage("./Images/simon-belmont-2.ppm");
    //Set pixel red colors to black 
    setColorBlack(simonImage);
    //creating openGl texture elements
    glGenTextures(1, &simonTexture);
    glGenTextures(1, &silhouetteTexture);
    //-------------------------------------------------------------------
    //Simon
    //
    int w = simonImage->width;
    int h = simonImage->height;
    //
    glBindTexture(GL_TEXTURE_2D, simonTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
            GL_RGB, GL_UNSIGNED_BYTE, simonImage->data);
    //-------------------------------------------------------------------
    //
    //silhouette
    //displays sprite to screen 
    //
    glBindTexture(GL_TEXTURE_2D, silhouetteTexture);
    //
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    unsigned char *silhouetteData = buildAlphaData(simonImage);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, silhouetteData);
    delete [] silhouetteData;
    //-------------------------------------------------------------------
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
    //g->player.setPos(100.0f, 350.0f);     
    //g->initWhip();          
    //g->whip.setCenterX(g->player.getPosX());         
    //g->whip.setCenterY(g->player.getPosY() + 200);         
    //g->initFloor();       
    //create ghouls
    //g->createGhoul(); 
    //ghoul2
    //g->createGhoul(); 
    //create obstacles
    //g->initObstacles();

    int row = 20;
    int col = 60;
    char **map;
    char *lvl = "Levels/lvl1.txt";
    map = g->getLevel(lvl,row,col);
    Block *b;

    for (int r = 0; r < row; r++){
        for(int c = 0; c < col; c++){
            if (!isspace(map[r][c])){
                if (map[r][c] == 'S') 
                    g->player.setPos(((2*40*c)+40), (900-(2*40*r-35))); 
                
                b = g->createBlock();

                if (map[r][c] == 'C') 
                    b->setClimb(true); 

                if (c != (col - 1) && !isspace(map[r][c+1])) 
                    b->setAdjRight(true);
                
                if (c != 0 && !isspace(map[r][c-1])) 
                    b->setAdjLeft(true);
               
                b->setCenter(((2*40*c)+40), (900-(2*40*r)-40));  
            }
        }
    }

    // change this if you want, this was here for testing
    fmod_createsound("./Sounds/Shape Shifters_Coldly Calculated Design 8-bit.flac",BGM);
    // setting to loop for now (it is bgm atm)
    fmod_setmode(BGM,FMOD_LOOP_NORMAL);
    // play sound; bool does nothing atm
    fmod_playsound(BGM);
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
        if (key == XK_Shift_L || key == XK_Shift_R) {
            // release shift (unused?) and dash
            shift=0;
            dash=0;
        }
        return 0;
    }
    else if (e->type == KeyPress) {
        keys[key]=1;
        if (key == XK_Shift_L || key == XK_Shift_R) {
            // set shift (unused?) and dash
            shift=1;
            dash=1;
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

    playerFwd(g);	
    playerJump(g);
    playerForces(g);
    playerCollision(g);
    applyKey(g, keys);

}

void render(Game *g)
{	
    glClear(GL_COLOR_BUFFER_BIT);


    Block *bh = g->blockHead;
    int blockWidth = bh->getWidth();
    int blockHeight = bh->getHeight();
    float blockX;
    float blockY;

    glPushMatrix();
    float dist = -g->player.getPosX()+xres/2;
    if (dist < 0 || dist > 3960) 
        glTranslatef(-g->player.getPosX()+xres/2,-g->player.getPosY()+200, 0.0f);

    else {
        glTranslatef(0.0,-g->player.getPosY()+200, 0.0f);
    }

    for (int i = 0; i < g->nBlocks; i++) {

        glPushMatrix();
        blockX = bh->getCenterX();
        blockY = bh->getCenterY();
        glColor3f(83.0/255.0,52.0/255.0,178.0/255.0);
        glTranslatef(blockX, blockY, 0.0f);
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
        bh = bh->next;
    }
    spriteAnimation(g, keys);
    /* float playerW;
       float playerH;

    //Draw the player
    glColor3fv(g->player.getColor());
    glPushMatrix();
    playerW = g->player.getWidth();
    playerH = g->player.getHeight();

    glTranslatef(g->player.getPosX(), g->player.getPosY(), 0.0f);

    glBegin(GL_QUADS);
    glVertex2f(-playerW, -playerH);
    glVertex2f(-playerW, playerH);
    glVertex2f(playerW, playerH);
    glVertex2f(playerW, -playerH);
    glEnd();
    glColor3f(1.0f, 0.0f, 0.0f);
    glPopMatrix();
*/
    glPopMatrix();
	renderUI(g);
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
    */


    /*else{
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
}
