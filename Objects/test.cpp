#include <iostream>
#include <fstream>

using namespace std;

int main()
{

        int rows = 12;
        int array[rows][20];
        ifstream inFile;
        inFile.open("lvl1.txt");

        for (int r = 0; r < rows; r++)
        {
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
        return 0;
}
