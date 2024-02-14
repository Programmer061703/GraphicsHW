# define OpenGL libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lglut -lGL -lGLU
endif

# define compiler options
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3

fireworks: fireworks.cpp
	$(CC) -o fireworks fireworks.cpp $(LIB)