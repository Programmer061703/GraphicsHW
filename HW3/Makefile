# define OpenGL libraries
ifeq ($(shell uname),Darwin)
LIB = -DMAC -framework OPENGL -framework GLUT
else
LIB = -lglut -lGL -lGLU
endif

# define compiler options
CC = g++ -std=c++11 -Wall -Wno-deprecated-declarations -O3

draw: penny.cpp
		$(CC) -o penny penny.cpp $(LIB)

# draw: pennyblake.cpp
# 		$(CC) -o pennyblake pennyblake.cpp $(LIB)
