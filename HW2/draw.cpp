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

#include <vector>

// Global variables for line coordinates
float lineStart[2] = {-1.0f, -1.0f}; // Initialize to an off-screen position
float lineEnd[2] = {-1.0f, -1.0f}; // Initialize to an off-screen position

// Global variable for cube's position
float cubePos[3] = {0.0f, 0.0f, -7.0f}; // Initial position of the cube

// Global index for the current target point in the linePoints vector
size_t currentPointIndex = 0;


// Structure to represent a 2D point
struct Point {
    float x, y;
};

// Vector to store the line points
std::vector<Point> linePoints;

bool isDrawing = false; // Flag to track if we're currently drawing






// Initialize OpenGL Graphics
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    
}

void mouse(int button, int state, int x, int y) {
    // Get the current window dimensions
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    
    float worldX = (x / (float)windowWidth) * 2.0f - 1.0f;
    float worldY = 1.0f - (y / (float)windowHeight) * 2.0f; // Invert Y

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        linePoints.clear(); // Clear previous points
        linePoints.push_back({worldX, worldY}); // Start a new line
        isDrawing = true;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDrawing = false;
        currentPointIndex = 0;
    }
}

void mouseMotion(int x, int y) {
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // Convert mouse coordinates to OpenGL world coordinates
    float ndcX = (x / (float)windowWidth) * 2.0f - 1.0f;
    float ndcY = 1.0f - (y / (float)windowHeight) * 2.0f;

    // Convert from NDC to world coordinates
    float worldX = ndcX * (windowWidth / 100.0f);
    float worldY = ndcY * (windowHeight / 100.0f);

    if (isDrawing) {
        linePoints.push_back({worldX, worldY});
        glutPostRedisplay(); 
    }
}





void drawLine() {
    if (!linePoints.empty()) {
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f); // White
        for (size_t i = 0; i < linePoints.size(); i++) {
            glVertex3f(linePoints[i].x, linePoints[i].y, -7.0f);
        }
        glEnd();
    }
}





// Function to draw cube
void drawCube() {
    glBegin(GL_QUADS); // Start drawing the cube with 6 quads

    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f); // Orange
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // Front face  (z = 0.5f)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // Back face (z = -0.5f)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // Left face (x = -0.5f)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // Right face (x = 0.5f)
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    glEnd(); // End of drawing color-cube
}

// Called when the window needs to be redrawn
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW); 

    // Reset transformations and move to starting position of the cube
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -7.0f);

    // Draw the line
    drawLine();

    // Draw the cube at the first point of the line
    glPushMatrix();
    glTranslatef(cubePos[0], cubePos[1], cubePos[2]);
    glScalef(0.5f, 0.5f, 0.5f); // Scale down the cube to half its size
    drawCube();
    glPopMatrix();

    glutSwapBuffers();
}




// Called when the window is resized
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
}
void timer(int value) {
    if (!linePoints.empty() && currentPointIndex < linePoints.size()) {
        // Update cube position to the next point
        cubePos[0] = linePoints[currentPointIndex].x;
        cubePos[1] = linePoints[currentPointIndex].y;
        
        // Increment the point index to move to the next point in the next frame
        currentPointIndex++;

        glutPostRedisplay(); // Mark the current window as needing to be redisplayed
    }

    // Continue the animation
    glutTimerFunc(1, timer, 0); // Adjust the timeout to control speed
}


int main(int argc, char** argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE); 
    glutInitWindowSize(640, 480); 
    glutCreateWindow("Line Cube Follow"); 

    initGL(); 
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape); 
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion); 

    glutTimerFunc(0, timer, 0); 

    glutMainLoop(); 
    return 0;
}

