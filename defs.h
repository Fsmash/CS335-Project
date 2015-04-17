#ifndef _DEFS_H
#define _DEFS_H

typedef double Flt;
typedef double Vec[3];
typedef Flt	Matrix[4][4];

typedef struct t_rect {
	int left;
	int top;
	int right;
	int bot;
	int width;
	int height;
	int center;
	int centerx;
	int centery;
} Rect;

typedef struct t_mouse {
	int x,y;
	int lastx,lasty;
	int lbuttondown;
} Mouse;

typedef struct t_bsphere {
	Vec center;
	Flt radius;
	Flt radiusSqr;
} Bsphere;

typedef struct t_bbox {
	Flt min[3];
	Flt max[3];
} Bbox;

typedef struct t_texmap {
	int xres, yres;
	unsigned char *c;
	unsigned int *i;
} Texmap;

typedef struct t_screen {
	int x_res, y_res;
	int screen_x_res, screen_y_res;
	float gl_perspective_viewangle;
	Flt xcenter, ycenter;
	Flt fy_res, fx_res;
} Screen;

#define MAX_LIGHTS (4)
typedef struct t_lights {
	int onoff;
	int nlights;
	Vec light;
	float glAmb[4];
	float glDif[4];
	float glPos[4];
} Lights;

typedef struct t_keys {
	int autorepeatstate, shiftstate, ctrlstate, shiftlockstate;
} Keys;

typedef struct t_camera {
	Vec pos;
	Vec move;
	Vec dir;
	Vec rot;
	Flt ang;
	Flt tilt;
	Matrix m;
} Camera;

typedef struct t_mats {
	Matrix worldSpace;
	Matrix cameraSpace;
	Matrix playerSpace;
} Mats;

typedef struct t_frustum {
	Flt frontPlaneDistance;
	Vec frontPlaneNormal;
	Vec frontPlanePoint;
	Flt farPlaneDistance;
	Vec farPlanePoint;
	Flt farObjectPlaneDistance;
	Vec farObjectPlanePoint;
	Flt missile_view_distance2;
	//#1 is left   plane normal
	//#2 is right  plane normal
	//#3 is top    plane normal
	//#4 is bottom plane normal
	//#5 is front  plane normal
	//#6 is far    plane normal
	Vec planes[6];
	Flt glplanesd[6][4];
	float glplanesf[6][4];
	float glFarPlaneDistance;
	float glNearPlaneDistance;
	Vec save_planes[6];
} Frustum;

#endif

