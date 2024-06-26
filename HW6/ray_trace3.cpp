//---------------------------------------
// Program: ray_trace.cpp
// Purpose: Demonstrate ray tracing.
// Author:  John Gauch
// Date:    Spring 2019
//---------------------------------------
#include <cmath>
#include <cstdio>
#include <cstdlib>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Include ray tracing and phong shading classes
#include "ray_classes.h"

// Global variables
#define XDIM 600
#define YDIM 600
#define ZDIM 600
unsigned char image[YDIM][XDIM][3];
float position = -1;
string mode = "phong";
float Bounce = -0.5;
const float RADIUS = 2.0;
const int SPHERES = 2;
Sphere3D sphere[SPHERES];
ColorRGB color[SPHERES];

//---------------------------------------
// Calculate random value between [min..max]
//---------------------------------------
float myrand(float min, float max)
{
   return rand() * (max - min) / RAND_MAX + min;
}

//---------------------------------------
// Check to see if point is in shadow
//---------------------------------------
bool in_shadow(Point3D pt, Vector3D dir, int current, Sphere3D sphere[], int count)
{
   // Define ray to light source
   Ray3D shadow_ray;
   shadow_ray.set(pt, dir);

   // Check to see ray intersects any sphere
   Point3D point;
   Vector3D normal;
   for (int index=0; index<count; index++)
      if ((index != current) && 
         (sphere[index].get_intersection(shadow_ray, point, normal)))
         return true;
   return false;
}

struct LightSource {
    Vector3D direction;
    ColorRGB color;
    float intensity;
};

//---------------------------------------
// Perform ray tracing of scene
//---------------------------------------
void ray_trace()
{
   // Define camera point
   Point3D camera;
   camera.set(0,0,position);
   int lightcount = 2; 

   // Define light source
   //Make Array of multiple light colors and directions 
   ColorRGB light_color[lightcount];
   Vector3D light_dir[lightcount];
   
   light_color[0].set(250,250,250);
   light_dir[0].set(-1,-1,-1);
   light_dir[0].normalize();

   light_color[1].set(150,150,150);
   light_dir[1].set(1,-1,-1);
   light_dir[1].normalize();



   // Define shader
   Phong shader;
   shader.SetCamera(camera);



   // Perform ray tracing
   for (int y = 0; y < YDIM; y++)
   for (int x = 0; x < XDIM; x++)
   {
      // Clear image
      image[y][x][0] = 0;
      image[y][x][1] = 0;
      image[y][x][2] = 0;

      // Define sample point on image plane
      float xpos = (x - XDIM/2) * 2.0 / XDIM;
      float ypos = (y - YDIM/2) * 2.0 / YDIM;
      Point3D point;
      point.set(xpos, ypos, 0);
   
      // Define ray from camera through image
      Ray3D ray;
      ray.set(camera, point);

      // Perform sphere intersection
      int closest = -1;
      Point3D p, closest_p;
      Vector3D n, closest_n;
      closest_p.set(0,0,ZDIM);
      for (int s=0; s<SPHERES; s++)
      {
         if ((sphere[s].get_intersection(ray, p, n)) && (p.pz < closest_p.pz))
         {
            closest = s;
            closest_p = p;
            closest_n = n;
         }
      }

      // Calculate pixel color
      if (closest >= 0)
      {
         // Display surface normal
         if (mode == "normal")
         {
            image[y][x][0] = 127 + closest_n.vx * 127;
            image[y][x][1] = 127 + closest_n.vy * 127;
            image[y][x][2] = 127 + closest_n.vz * 127;
         }

         // Calculate Phong shade
         if (mode == "phong")
         {
            ColorRGB total;
            total.set(0,0,0);
         for (int i = 0; i < lightcount; i++){
            // Check see if in shadow 
            if (in_shadow(closest_p, light_dir[i], closest, sphere, SPHERES))
               shader.SetObject(color[closest], 0.4, 0.0, 0.0, 1);
            else
               shader.SetObject(color[closest], 0.4, 0.4, 0.4, 10);

      
            // Calculate pixel color
            ColorRGB pixel;
            shader.SetLight(light_color[i], light_dir[i]);
            shader.GetShade(closest_p, closest_n, pixel);
            total.add(pixel);
            image[y][x][0] = total.R;
            image[y][x][1] = total.G;
            image[y][x][2] = total.B;
         }
         }
      }
   }
}
 
//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
    // Initialize OpenGL
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // Print command menu
    cout << "Program commands:\n"
         << "   '+' - increase camera distance\n"
         << "   '-' - decrease camera distance\n"
         << "   'p' - show Phong shading\n"
         << "   'n' - show surface normals\n"
         << "   'q' - quit program\n";

    // Define two spheres: one stationary, one rotating
    Point3D center;
    Vector3D stationary;
    Vector3D rotating;

    stationary.set(0,0,0);
    rotating.set(0,0.5,0);
    

    // Stationary sphere
    center.set(0, 0, 0);
    sphere[0].set(center, stationary, position/4);  // No motion
    color[0].set(255, 0, 0);  // Red color

    // Rotating sphere
    center.set(position / 1.5, 0, 0);
    sphere[1].set(center, stationary, position /4);  
    color[1].set(0, 0, 255);  // Blue color



    // Initialize ray tracing
    cout << "camera: 0,0," << position << endl;
     ray_trace();
}


//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   // Display image
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawPixels(XDIM, YDIM, GL_RGB, GL_UNSIGNED_BYTE, image);
   glFlush();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
// Keyboard callback for OpenGL
void keyboard(unsigned char key, int x, int y)
{
    bool update_needed = false;

    // End program
    if (key == 'q')
    {
        exit(0);
    }

    // Move camera position smoothly
    else if (key == '+')
    {
        if (position > -10)
        {
            position -= 0.1; // Smaller increment for smoother transition
            update_needed = true;
        }
    }
    else if (key == '-')
    {
        if (position < -1) // Ensure there's a sensible minimum distance
        {
            position += 0.1; // Smaller increment for smoother transition
            update_needed = true;
        }
    }

    // Change display mode
    else if (key == 'n')
    {
        mode = "normal";
        update_needed = true;
    }
    else if (key == 'p')
    {
        mode = "phong";
        update_needed = true;
    }

    // Re-render only if necessary
    if (update_needed)
    {

      cout << position<<endl;
        ray_trace();
        glutPostRedisplay();
    }
}

//---------------------------------------
// Timer callback for OpenGL
//---------------------------------------
void timer(int value)
{
    // Update the position of the second sphere to rotate around the first
    float angle = 0.05; // Adjust this for faster or slower rotation
    float oldX = sphere[1].center.px;
    float oldZ = sphere[1].center.pz;
    sphere[1].center.px = cos(angle) * oldX - sin(angle) * oldZ;
    sphere[1].center.pz = sin(angle) * oldX + cos(angle) * oldZ;

    // Re-render the scene
    ray_trace();
    glutPostRedisplay();
    glutTimerFunc(33, timer, 0);  
}



//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   // Create OpenGL window
   glutInit(&argc, argv);
   glutInitWindowSize(XDIM, YDIM);
   glutInitWindowPosition(0, 0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Ray Trace");
   init();

   // Specify callback function
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(10, timer, 0);
   glutMainLoop();
   return 0;
}
