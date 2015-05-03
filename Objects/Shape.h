#ifndef _OBJECTS_H_
#define _OBJECTS_H_

struct Vect {
        float x, y, z;
};

class Shape {

        protected: 
                float width;
                float height;
                float radius;
                Vect center;

        public:

                float getHeight() { return height; }

                void setHeight(float iHeight) { height = iHeight; }

                float getWidth() { return width; }

                void setWidth(float iWidth) { width = iWidth; }
                
                void setCenter(float x, float y) { 
                    center.x = x; 
                    center.y = y; 
                }

                float getCenterX() { return center.x; }

                void setCenterX(float x) { center.x = x; }

                float getCenterY() { return center.y; }

                void setCenterY(float y) { center.y = y; }
                
};

#endif //_OBJECTS_H_
