#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <iostream>
#endif

#define XDIM 800
#define YDIM 600

// Global variables
int xangle = 10;
int yangle = 15;

// Structure to represent a firework
struct Firework {
    float size;
    float colorR, colorG, colorB;
    float centerX, centerY, centerZ;
};

// Global array of fireworks
Firework fireworks[10];

void init(){
      glClearColor(0.0, 0.0, 0.0, 1.0);
      
      glOrtho(-XDIM, XDIM, -YDIM, YDIM, -XDIM, XDIM);
}


// Function to draw an exploded firework
void drawExplosion(float centerX, float centerY,float centerZ, float size, float colorR, float colorG, float colorB) {
    glColor3f(colorR, colorG, colorB);
    for (int j = 0; j < 360; j += 15) {

        //Uncomment to make it look like a circle
        // float angle = static_cast<float>(j) * (M_PI / 180.0f);
        // float x2 = centerX + size * cos(angle);
        // float y2 = centerY + size * sin(angle);

        //Uncomment to make it look like random
        float randomFactor = 0.8f + static_cast<float>(rand()) / (RAND_MAX / 0.4f);
        float lineLength = size * randomFactor;
        float angle = static_cast<float>(j) * (rand() % 360);
        float x2 = centerX + lineLength * cos(angle);
        float y2 = centerY + lineLength * sin(angle);
        float z2 = centerZ + (rand() % 100) / 10.0f;

        glBegin(GL_LINES);
        glVertex3f(centerX, centerY, centerZ);
        glVertex3f(x2, y2, z2);
        glEnd();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r') {
       glutPostRedisplay();
    }
    else if (key == 'x'){
        xangle -= 5;
        glutPostRedisplay();
    }
    else if (key == 'y'){
        yangle -= 5;
        glutPostRedisplay();
    }
    else if (key == 'X'){
        xangle += 5;
        glutPostRedisplay();
    }
    else if (key == 'Y'){
        yangle += 5;
        glutPostRedisplay();
    }
    
    
}

// Function to display fireworks
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    int displayCount = rand() % 6 + 5;
     for (int i = 0; i < displayCount; i++) {
    fireworks[i].size = rand() % 100;
    fireworks[i].colorR = static_cast<float>(rand() % 256) / 255.0f;
    fireworks[i].colorG = static_cast<float>(rand() % 256) / 255.0f;
    fireworks[i].colorB = static_cast<float>(rand() % 256) / 255.0f;
    fireworks[i].centerX = rand() % XDIM - 400;// xwindow width divided by 2
    fireworks[i].centerY = rand() % YDIM - 300;// ywindow height divided by 2
    fireworks[i].centerZ = rand() % 100;
    }

    for (int i = 0; i < displayCount; i++) {
        drawExplosion(fireworks[i].centerX, fireworks[i].centerY,fireworks[i].centerZ, fireworks[i].size,
                       fireworks[i].colorR, fireworks[i].colorG, fireworks[i].colorB);
    }
    glRotatef(xangle, 1.0, 0.0, 0.0);
    glRotatef(yangle, 0.0, 1.0, 0.0);
    glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fireworks Display");
    std::cout << "Press 'r' to display fireworks" << std::endl;
    std::cout << "Press 'x' to rotate the fireworks on the x-axis" << std::endl;
    std::cout << "Press 'y' to rotate the fireworks on the y-axis" << std::endl;
    std::cout << "Press 'X' to rotate the fireworks on the x-axis" << std::endl;
    std::cout << "Press 'Y' to rotate the fireworks on the y-axis" << std::endl;
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}
