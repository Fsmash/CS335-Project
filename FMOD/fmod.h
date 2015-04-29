#ifndef THIS_FMOD
#define THIS_FMOD

#include "api/inc/fmod.hpp"
#include "api/inc/fmod_errors.h"
#include <stdio.h>

#define MAX_SOUNDS 32
#define BGM 0


extern bool fmod_ok(FMOD_RESULT res);

extern bool fmod_init();

extern bool fmod_createsound(const char *fname, int i);

extern bool fmod_playsound(int i);

extern bool fmod_setmode(int i, int mode);

extern bool fmod_getlength(int i, unsigned int *lenms);

extern bool fmod_systemupdate();

extern bool fmod_getchannelsplaying(int *channelsplaying);

extern bool fmod_cleanup();

#endif
