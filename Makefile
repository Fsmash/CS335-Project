CFLAGS = -I ./include
#LIB    = ./libggfonts.so
#LFLAGS = $(LIB) -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

#all: project.cpp Images/ppm.c etc/log.c FMOD/fmod.cpp
#	g++ $(CFLAGS) project.cpp etc/log.c FMOD/fmod.cpp -Wall -Wextra $(LFLAGS) -o project -w
all: project.cpp Images/ppm.c etc/log.c
	g++ $(CFLAGS) project.cpp etc/log.c -Wall -Wextra $(LFLAGS) -o project -w

clean:
	rm -f project
	rm -f *.o

