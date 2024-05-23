#include <GL/glut.h>
#include <cmath>

#define WIDTH 960
#define HEIGHT 540

void DrawLine()
{
    glBegin(GL_LINES);
    glColor3ub(255, 0, 0);
    glVertex2f(-0.5f, -0.5f);
    glColor3ub(0, 255, 0);
    glVertex2f(0.5f, 0.5f);
    glEnd();
}

void DrawTriangle()
{
    glBegin(GL_TRIANGLES);
    glColor3ub(6, 31, 73);
    glVertex2f(-0.5f, -0.5f);
    glColor3ub(15, 79, 183);
    glVertex2f(0.5f, -0.5f);
    glColor3ub(0, 0, 0);
    glVertex2f(0.0f, 0.5f);
    glEnd();
}


void DrawSquare()
{
    glBegin(GL_QUADS);
    glColor3ub(255, 0, 0);
    glVertex2f(100, 100);
    glColor3ub(0, 255, 0);
    glVertex2f(200, 100);
    glColor3ub(0, 0, 255);
    glVertex2f(200, 200);
    glColor3ub(255, 255, 0);
    glVertex2f(100, 200);
    glEnd();
}

void Render()
{
    float time = glutGet(GLUT_ELAPSED_TIME)/1000.f;

    float tx = 0;
    float ty = 0;
    float tz = 10.0f * time;

    GLfloat translationMatrix[16] = {
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        tx,     ty,     tz,     1.0f
    };

    glClearColor(0, 0, 0, 1.0); // noir
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear depth buffer
    glEnable(GL_DEPTH_TEST);  // Enable depth testing

    // Configuration de la projection en perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)WIDTH / (double)HEIGHT, 0.1, 1000.0);

    // Dessin du carré
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glMultMatrixf(translationMatrix);
    DrawSquare();
    glPopMatrix();
}

// la ‘callback’ executee par glutDisplayFunc()
void Display()
{
    Render();
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Triangle");
    glEnable(GL_CULL_FACE);
    glEnable(GL_SCISSOR_TEST);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}