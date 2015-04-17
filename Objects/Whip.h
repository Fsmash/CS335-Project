#ifndef _WHIP_H_
#define _WHIP_H_
#include "Shape.h"

class Whip : public Shape {

        public: 

                float tipX;
                float tipY;

                Whip() {           //initialze pointers to NULL.
                        tipX = tipY = 0;
                }

                float getTipX() { return tipX; }

                void setTipX(float x) { tipX = x; }

                float getTipY() { return tipY; }

                void setTipY(float y) { tipY = y; }

};


#endif //_WHIP_H_
