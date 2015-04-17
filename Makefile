CFLAGS = -I ./include
LIB    = ./libggfonts.so
LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: project.cpp Images/ppm.c log.c
	g++ $(CFLAGS) project.cpp log.c -Wall -Wextra $(LFLAGS) -o project -w

clean:
	rm -f project
	rm -f *.o

