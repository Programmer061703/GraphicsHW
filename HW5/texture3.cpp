//---------------------------------------
// Program: texture3.cpp
// Purpose: Texture map brick photograph
//          onto a cube model.
// Author:  John Gauch
// Date:    April 2011
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"
#include <fstream>
#include <iostream>
#include <random>

// Global variables 
#define ROTATE 1
#define TRANSLATE 2
#define MOVE 3
int xangle = 10;
int yangle = 10;
int zangle = 10;
int xpos = 0;
int ypos = 0;
int zpos = 0;
int mode = ROTATE;
int maze[10][10];
int size1;
int size2;
int y_pos;
int x_pos;
int numGold;
int xdim, ydim;
unsigned char *texture1;
unsigned char *texture2;
unsigned char *texture3;
unsigned char *saul;
unsigned char *grass;
unsigned char *gold;
//int grass_xdim, grass_ydim; 
#define SIZE 10


void readFile(const string filename){
      ifstream din(filename);
      string line;
      din >> size1 >> size2;
      din >> y_pos >> x_pos;
   

      din.ignore(); 
      for(int i = 0; i < size1; i++){
         for(int j = 0; j < size2; j++){
           char ch;
           din.get(ch);
             maze[i][j] = ch; 
             
         }
         din.ignore();
      }
    din.close();

   
}


//---------------------------------------
// Initialize texture image
//---------------------------------------
void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim)
{
   // Read jpg image
   im_color image;
   image.ReadJpg(name);
   printf("Reading image %s\n", name);
   xdim = 1; while (xdim < image.R.Xdim) xdim*=2; xdim /=2;
   ydim = 1; while (ydim < image.R.Ydim) ydim*=2; ydim /=2;
   image.Interpolate(xdim, ydim);
   printf("Interpolating to %d by %d\n", xdim, ydim);

   // Copy image into texture array
   texture = (unsigned char *)malloc((unsigned int)(xdim*ydim*3));
   int index = 0;
   for (int y = 0; y < ydim; y++)
      for (int x = 0; x < xdim; x++)
      {
         texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
      }
}

//---------------------------------------
// Function to draw 3D block
//---------------------------------------
void block(float xmin, float ymin, float zmin,
           float xmax, float ymax, float zmax)
{
   // Define 8 vertices
   float ax = xmin, ay = ymin, az = zmax;
   float bx = xmax, by = ymin, bz = zmax;
   float cx = xmax, cy = ymax, cz = zmax;
   float dx = xmin, dy = ymax, dz = zmax;

   float ex = xmin, ey = ymin, ez = zmin;
   float fx = xmax, fy = ymin, fz = zmin;
   float gx = xmax, gy = ymax, gz = zmin;
   float hx = xmin, hy = ymax, hz = zmin;

   // Draw 6 faces
   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(gx, gy, gz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(cx, cy, cz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 3.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(2.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(2.0, 2.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(0.0, 2.0);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 3.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(3.0, 0.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(3.0, 3.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 3.0);
   glVertex3f(hx, hy, hz);
   glEnd();
}

void treasurePlace(){

   srand(time(NULL));
   int random;
   for(int i = 0; i < size1; i++){
      for(int j = 0; j < size2; j++){
         random = rand() % 2;
         cout << random<<endl;
         if(random == 1 && maze[i][j] == ' '){
            maze[i][j] = 'g';
            numGold++;
            
         }
         }
      }
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   // Init view
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
   glEnable(GL_DEPTH_TEST);

   // Init texture
   
   glEnable(GL_TEXTURE_2D);
   init_texture((char *)"textures/brick.jpg", texture1, xdim, ydim);
   init_texture((char *)"textures/rock.jpg", texture2, xdim, ydim);
   init_texture((char *)"textures/wood.jpg", texture3, xdim, ydim);
   init_texture((char *)"textures/saul.jpg", saul, xdim, ydim);
   init_texture((char *)"textures/grass.jpg", grass, xdim, ydim);
   init_texture((char *)"textures/gold.jpg", gold, xdim, ydim);
   treasurePlace();
   
   //init_texture((char *)"textures/grass.jpg", grass, grass_xdim, grass_ydim);
   
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}




   







//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{

  
   
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);
   
   
  
   // Draw objects
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, grass);
   block(0, 0, 0, 5, 5, 0);
   
   
   

   for (int i = 0; i < size1; i++){
      for (int j = 0; j < size2; j++){
         float size = 0.5; // Size of each block
         float offset = size * 0.25; // Offset to keep the smaller player block centered
         float xbase = j * size;
         float ybase = i * size;

        
         
         //player
         if(i == y_pos && j == x_pos){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, saul);
            block(xbase+offset, ybase+offset, offset, xbase + offset / size, ybase + offset / size, offset / size);
         }
         //brick
         if(maze[i][j]== 'b'){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1);
            block(xbase, ybase, 0, xbase + size, ybase + size, size);
         }
         //rock
         if(maze[i][j]== 'r'){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture2);
            block(xbase, ybase, 0, xbase + size, ybase + size, size);
               
         }
         //wood
         if(maze[i][j]== 'w'){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture3);
            block(xbase, ybase, 0, xbase + size, ybase + size, size);

         }
         //gold 
         if(maze[i][j]== 'g'){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, gold);
            block(xbase+offset, ybase+offset, offset, xbase + offset / size, ybase + offset / size, offset / size);
      }

         if(maze[y_pos][x_pos]=='g'){
            maze[y_pos][x_pos] = ' ';
            numGold--;
         }

         
         
   }
  
    glFlush();
}
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Determine if we are in ROTATE or TRANSLATE mode
   if ((key == 'r') || (key == 'R'))
   {
      printf("Type x y z to decrease or X Y Z to increase ROTATION angles.\n");
      mode = ROTATE;
   }
   else if ((key == 't') || (key == 'T'))
   {
      printf("Type x y z to decrease or X Y Z to increase TRANSLATION distance.\n");
      mode = TRANSLATE;
   }
   else if((key == 'm') || (key == 'M')){
      printf("Type w a s d to move player\n");
      mode = MOVE;
   }

   // Handle ROTATE
   if (mode == ROTATE)
   {
      if (key == 'x')
         xangle -= 5;
      else if (key == 'y')
         yangle -= 5;
      else if (key == 'z')
         zangle -= 5;
      else if (key == 'X')
         xangle += 5;
      else if (key == 'Y')
         yangle += 5;
      else if (key == 'Z')
         zangle += 5;
      glutPostRedisplay();
   }

   // Handle TRANSLATE
   if (mode == TRANSLATE)
   {
      if (key == 'x')
         xpos -= 15;
      else if (key == 'y')
         ypos -= 15;
      else if (key == 'z')
         zpos -= 15;
      else if (key == 'X')
         xpos += 15;
      else if (key == 'Y')
         ypos += 15;
      else if (key == 'Z')
         zpos += 15;
      glutPostRedisplay();
   }
   
   // Handle MOVE
   if (mode == MOVE){
      int new_x_pos = x_pos;
      int new_y_pos = y_pos;
      if (key == 'w')
         new_y_pos = y_pos + 1;
      else if (key == 'a')
         new_x_pos = x_pos - 1;
      else if (key == 's')
         new_y_pos = y_pos - 1;
      else if (key == 'd')
         new_x_pos = x_pos + 1;

      // Check for collisions with obstacles
      if (new_x_pos >= 0 && new_x_pos < size2 && new_y_pos >= 0 && new_y_pos < size1) {
          char tile = maze[new_y_pos][new_x_pos];
          if (tile != 'b' && tile != 'r' && tile != 'w') {
              x_pos = new_x_pos;
              y_pos = new_y_pos;
          }
      }
      if(numGold == 0){
            cout << "YOU COLLECTED ALL THE GOLD, YOU WIN"<<endl; 
         }

      glutPostRedisplay();
   }
}


//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
   // Handle mouse down
   static int xdown, ydown;
   if (state == GLUT_DOWN)
   {
      xdown = x;
      ydown = y;
   }

   // Handle ROTATE
   if ((mode == ROTATE) && (state == GLUT_UP))
   {
      xangle += (y - ydown);
      yangle -= (x - xdown);
      zangle = 0;
      glutPostRedisplay();
   }

   // Handle TRANSLATE
   if ((mode == TRANSLATE) && (state == GLUT_UP))
   {
      xpos += (x - xdown);
      ypos -= (y - ydown);
      glutPostRedisplay();
   }
}




//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   readFile("maze.txt");
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("Texture");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
    init();
   
    printf("Keyboard commands:\n");
    printf("   't' or 'T' - go to translate mode\n");
    printf("   'r' or 'R' - go to rotate mode\n");
    printf("   'x' - rotate or translate on x-axis -5\n");
    printf("   'X' - rotate or translate on x-axis +5\n");
    printf("   'y' - rotate or translate on y-axis -5\n");
    printf("   'Y' - rotate or translate on y-axis +5\n");
    printf("Mouse operations:\n");
    printf("   'mouse down' - start selecting rotation direction\n");
    printf("   'mouse up' - finish selecting rotation direction\n");
   glutMainLoop();
   return 0;
}
