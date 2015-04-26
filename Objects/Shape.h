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
                float color[3];
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
                
                float * getColor() { return color; }
                
                void setColor(float R, float G, float B) { 
                        color[0] = R; 
                        color[1] = G; 
                        color[2] = B; 
                }
};

#endif //_OBJECTS_H_
