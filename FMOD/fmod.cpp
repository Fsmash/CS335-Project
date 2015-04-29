#include "fmod.h"

static int nsounds=0;
FMOD::System *fsys;
//local global variables are defined here
FMOD::Sound   *sound[MAX_SOUNDS];
FMOD::Channel *channel = 0;

bool fmod_ok(FMOD_RESULT res) 
{
  if(res != FMOD_OK) {
    printf("FMOD error (%d): %s\n",res, FMOD_ErrorString(res));
    return false;
  }
  return true;
}

bool fmod_init()
{
  FMOD_RESULT res;
  unsigned int version;
  //printf("fmod_init()...\n");
  res = FMOD::System_Create(&fsys);
  //if(!fsys) printf("Error: fsys not updated!\n");
  if (!fmod_ok(res)) {
    return false;
    //printf("[Debug] FMOD failed at FMOD::System_Create(&fsys)!\n");
  }
  res = fsys->getVersion(&version);
  //printf("[Debug] FMOD version is: %d\n",version);
  if (!fmod_ok(res)) {
    return false;
    //printf("[Debug] FMOD failed at getVersion(&version)!\n");
  }
  if (version < FMOD_VERSION) {
    printf("Error!  Old version of FMOD %08x.\n", version);
    printf("This program requires %08x\n", FMOD_VERSION);
    return false;
  }
  res = fsys->init(32, FMOD_INIT_NORMAL, NULL);
  if (!fmod_ok(res)) return false;
  //if(!fsys) printf("Error: fsys reset to null!\n");
  return true;
}

bool fmod_createsound(const char *fname, int i)
{
  FMOD_RESULT res;
  //printf("fmod_createsound(**%s**, %i)...\n",fname,i);
  if (nsounds >= MAX_SOUNDS)
    return true;
  //if(!fsys) printf("Error: fsys is null!\n");
  res = fsys->createSound(fname, FMOD_DEFAULT, NULL, &sound[i]);
  if (!fmod_ok(res)) {
    printf("Error: fmod_createsound()\n");
    return false;
  }
  ++nsounds;
  return true;
}

bool fmod_playsound(int i)
{
  FMOD_RESULT res;
  //printf("fmod_playsound(%i)...\n",i);
  res = fsys->playSound(sound[i], NULL, false, &channel);
  if (!fmod_ok(res)) {
    printf("Error: fmod_playsound()\n");
    return false;
  }
  return true;
}

bool fmod_setmode(int i, int mode)
{
  FMOD_RESULT res;
  //printf("fmod_setmode()...\n");
  //res = FMOD_Sound_SetMode(sound[i], FMOD_LOOP_OFF);
  res = sound[i]->setMode(mode);
  if (!fmod_ok(res)) {
    printf("Error: fmod_setmode()\n");
    return false;
  }
  return true;
}

bool fmod_getlength(int i, unsigned int *lenms)
{
  FMOD_RESULT res;
  res = sound[i]->getLength(lenms, FMOD_TIMEUNIT_MS);
  if (!fmod_ok(res)) {
    printf("Error: fmod_getlength()\n");
    return false;
  }
  return true;
}

bool fmod_systemupdate()
{
  fsys->update();
  return true;
}

bool fmod_getchannelsplaying(int *channelsplaying)
{
  fsys->getChannelsPlaying(channelsplaying);
  return true;
}

bool fmod_cleanup()
{
  int i;
  FMOD_RESULT res;
  for (i=0; i<nsounds; i++) {
    res = sound[i]->release();
    if (!fmod_ok(res))
      return false;
  }
  
  res = fsys->close();
  if (!fmod_ok(res)) return false;
  res = fsys->release();
  if (!fmod_ok(res)) return false;
  return true;
}
