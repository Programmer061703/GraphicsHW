#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define XDIM 800
#define YDIM 600

// Structure to represent a firework
struct Firework {
    float size;
    float colorR, colorG, colorB;
    float centerX, centerY;
};

// Global array of fireworks
Firework fireworks[10];

void init(){
      glClearColor(0.0, 0.0, 0.0, 1.0);
      gluOrtho2D(0, 800, 0, 600);
}

// Function to draw an exploded firework
void drawExplosion(float centerX, float centerY, float size, float colorR, float colorG, float colorB) {
    glColor3f(colorR, colorG, colorB);
    for (int j = 0; j < 360; j += 15) {
        float angle = static_cast<float>(j) * (M_PI / 180.0f);
        float x2 = centerX + size * cos(angle);
        float y2 = centerY + size * sin(angle);
        glBegin(GL_LINES);
        glVertex2f(centerX, centerY);
        glVertex2f(x2, y2);
        glEnd();
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
    fireworks[i].centerX = rand() % XDIM;
    fireworks[i].centerY = rand() % YDIM;
    }

    for (int i = 0; i < 10; i++) {
        drawExplosion(fireworks[i].centerX, fireworks[i].centerY, fireworks[i].size,
                       fireworks[i].colorR, fireworks[i].colorG, fireworks[i].colorB);
    }

    glFlush();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Fireworks Display");
    init();
    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}
