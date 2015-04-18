#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_
#include <fstream>
#include <iostream>
#include "Shape.h"

using namespace std;

class Obstacle : public Shape {

        public: 

                char **getLevel(char lvl, int rows, int cols) {

                        char **array;
                        array = new char*[rows];
                        ifstream inFile;
                        inFile.open(&lvl);

                        for (int r = 0; r < rows; r++)
                        {
                                array[r] = new char[cols];
                                int c = 0;
                                char ch = inFile.get();
                                while (ch != '\n')
                                {
                                        array[r][c] = ch;
                                        cout << (char)array[r][c];
                                        c++;
                                        ch = inFile.get();
                                }
                                cout << endl;
                        }
                        return array;    
                }

};


#endif //_OBSTACLE_H_
