#include <math.h> 

#include <string> 
#include <stdio.h> 
#include <stdlib.h> 
#ifdef MAC 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 
#include "shading.cpp" 
#include <string> 
#include <iostream> 
#include <fstream> 
#include <vector>
#include <sstream>
using namespace std; 


// Transformation variables  
#define ROTATE 1 
#define TRANSLATE 2 
int xangle = 0; 
int yangle = 0; 
int zangle = 0; 
int xpos = 0; 
int ypos = 0; 
int zpos = 0; 
int mode = ROTATE; 
int phong = 0; 
#define SIZE 500 
#define width 500
#define height 500
float depthData [width][height]; 
float red[width][height];
float green[width][height];
float blue[width][height];
float Px[SIZE + 1][SIZE + 1]; 
float Py[SIZE + 1][SIZE + 1]; 
float Pz[SIZE + 1][SIZE + 1]; 
float Nx[SIZE + 1][SIZE + 1]; 
float Ny[SIZE + 1][SIZE + 1]; 
float Nz[SIZE + 1][SIZE + 1]; 
#define STEP 0.1 



void readDepthFile(const string filename) 
{ 
   ifstream din(filename); 
   if (!din.is_open()) 
   { 
      cout << "Error. Can't read file"; 
      return; 
   } 
   for (int i = 0; i < 500; i++) 
   { 
      for (int j = 0; j < 500; j++) 
      { 
         din >> depthData[j][i]; 
      } 
   } 
 
   for (int i = 0; i < 500; i++) 
   { 
      for (int j = 0; j < 250; j++) 
      { 
         
         float temp = depthData[i][j]; 
         depthData[i][j] = depthData[i][499 - j]; 
         depthData[i][499 - j] = temp; 
      } 
   } 
} 

void depthDisplay()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);
   glDisable(GL_LIGHTING);


// Draw the surface 
   for (int i = 0; i < SIZE; i+=3) 
   { 
      for (int j = 0; j < SIZE; j+=3) 
      { 
         float pennyHeight = depthData[i][j] / 15;
         glBegin(GL_LINE_LOOP); 
         glVertex3f(i, j, pennyHeight); 
         glVertex3f(i+1, j, pennyHeight); 
         glVertex3f(i+1, j+1, pennyHeight); 
         glVertex3f(i, j+1, pennyHeight); 
         glEnd(); 
      } 
   } 
   glFlush(); 
}
void readColorFile(const string filename) 
{ 
   ifstream din(filename); 
   if (!din.is_open()) 
   { 
      cout << "Error. Can't read file"; 
      return; 
   } 
   for (int i = 0; i < 500; i++) 
   { 
      for (int j = 0; j < 500; j++) 
      { 
         din >> red[j][i]; 
         din >> green[j][i]; 
         din >> blue[j][i]; 
      } 
   } 
 
   
   for (int i = 0; i < 500; i++) 
   { 
      for (int j = 0; j < 250; j++) 
      { 
         // Rotates the color display upright
         float temp; 
         temp = red[i][j]; 
         red[i][j] = red[i][499 - j]; 
         red[i][499 - j] = temp; 
 
         temp = green[i][j]; 
         green[i][j] = green[i][499 - j]; 
         green[i][499 - j] = temp; 
 
         temp = blue[i][j]; 
         blue[i][j] = blue[i][499 - j]; 
         blue[i][499 - j] = temp; 
      } 
   } 
} 
void init() 
{ 
   // Initialize OpenGL 
   glClearColor(0.0, 0.0, 0.0, 1.0); 
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity(); 
   float radius = 500; 
   glOrtho(0, radius, 0, radius, -radius, radius); 
   glEnable(GL_DEPTH_TEST); 
 
   
   glShadeModel(GL_SMOOTH); 
} 

void init_surface() 
{ 
   // Initialize (x,y,z) coordinates of surface 
   for (int i = 0; i < SIZE; i++) 
   { 
      for (int j = 0; j < SIZE; j++) 
      { 
         Px[i][j] = i; 
         Py[i][j] = j; 
         Pz[i][j] = depthData[i][j] / 10.0; 
      } 
   } 
} 
//Used in surface4.cpp
void init_normals() 
{ 
   // Initialize surface normals 
   for (int i=0; i<SIZE-1; i++) 
   { 
      for (int j=0; j<SIZE-1; j++) 
      { 
         // Get tangents S and T 
         float Sx = Px[i+1][j] - Px[i][j]; 
         float Sy = Py[i+1][j] - Py[i][j]; 
         float Sz = Pz[i+1][j] - Pz[i][j]; 
         float Tx = Px[i][j+1] - Px[i][j]; 
         float Ty = Py[i][j+1] - Py[i][j]; 
         float Tz = Pz[i][j+1] - Pz[i][j]; 
          
         // Calculate N = S cross T 
         Nx[i][j] = Sy*Tz - Sz*Ty; 
         Ny[i][j] = Sz*Tx - Sx*Tz; 
         Nz[i][j] = Sx*Ty - Sy*Tx; 
 
         // Make unit vector 
         float length = sqrt(Nx[i][j]*Nx[i][j] 
                     + Ny[i][j]*Ny[i][j] 
                     + Nz[i][j]*Nz[i][j]); 
         if (length > 0) 
         { 
            Nx[i][j] /= length; 
            Ny[i][j] /= length; 
            Nz[i][j] /= length; 
         } 
      } 
   }
}

//--------------------------------------- 
// Color Display callback for OpenGL 
//--------------------------------------- 
void colorDisplay() 
{ 
   // Incrementally rotate objects 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   glMatrixMode(GL_MODELVIEW); 
   glLoadIdentity(); 
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0); 
   glRotatef(xangle, 1.0, 0.0, 0.0); 
   glRotatef(yangle, 0.0, 1.0, 0.0); 
   glRotatef(zangle, 0.0, 0.0, 1.0); 
   glDisable(GL_LIGHTING); 
 
   // Draw the surface 
   for (int i = 0; i < SIZE; i++) 
   { 
      for (int j = 0; j < SIZE; j++) 
      { 
         glBegin(GL_POLYGON); 
         float pennyHeight = depthData[i][j] / 20; 
         glColor3f(red[i][j] / 255.0, green[i][j] / 255.0, blue[i][j] / 255.0); 
         glVertex3f(i, j, pennyHeight); 
         glVertex3f(i+1, j, pennyHeight); 
         glVertex3f(i+1, j+1, pennyHeight); 
         glVertex3f(i, j+1, pennyHeight); 
         glEnd(); 
      } 
   } 
   glFlush(); 
} 

void phongDisplay()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(xpos / 500.0, ypos / 500.0, zpos / 500.0);
   glRotatef(xangle, 1.0, 0.0, 0.0);
   glRotatef(yangle, 0.0, 1.0, 0.0);
   glRotatef(zangle, 0.0, 0.0, 1.0);

   // Initialize material properties
   init_material(Ka, Kd, Ks, 100 * Kp, 0.4, 0.2, 0.1);
   init_surface();
   init_normals();

   init_light(GL_LIGHT0, 1, 1, 1, 1, 1, 1); 

   // Draw the surface
   for (int i = 0; i < SIZE-1; i++)
      for (int j = 0; j < SIZE-1; j++)
      {
	 // glBegin(GL_LINE_LOOP);
	 glBegin(GL_POLYGON);
	 glNormal3f(Nx[i][j], Ny[i][j], Nz[i][j]);
	 glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
	 glNormal3f(Nx[i + 1][j], Ny[i + 1][j], Nz[i + 1][j]);
	 glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
	 glNormal3f(Nx[i + 1][j + 1], Ny[i + 1][j + 1], Nz[i + 1][j + 1]);
	 glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
	 glNormal3f(Nx[i][j + 1], Ny[i][j + 1], Nz[i][j + 1]);
	 glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
	 glEnd();
      }
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
  

   
   

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
   }

   if (key == 'd')
      glutDisplayFunc(depthDisplay); 
   if (key == 'n')
        glutDisplayFunc(colorDisplay);
   if (key == 'p')
        glutDisplayFunc(phongDisplay);
   

   glutPostRedisplay();
}






   




int main(int argc, char *argv[]) 
{ 
   // Create OpenGL window 
   glutInit(&argc, argv); 
   glutInitWindowSize(500, 500); 
   glutInitWindowPosition(250, 250); 
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH); 
   glutCreateWindow("Penny Display"); 
   init(); 
   readDepthFile("penny-depth.txt"); 
   readColorFile("penny-image.txt"); 
   printf(" x y z rotate \n X Y Z rotate opposite\n"); 
   printf(" n = Color Display \n d = Depth Display \n p = Phong Display \n"); 
   // Specify callback function 
   glutDisplayFunc(phongDisplay); 
   glutKeyboardFunc(keyboard); 
   glutMainLoop(); 
   return 0; 
} 