#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include<GL/glut.h>

void display()
{
    glClearColor(0.0f,0.0f,0.0f,1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
        glColor3f(0.0f,1.0f,0.0f);
        glVertex2f(-0.5f,-0.5f);
        glVertex2f(0.5f,-0.5f);
        glVertex2f(0.5f,0.5f);
        glVertex2f(-0.5f,0.5f);


    glEnd();

    glFlush();


}

void idle()
{
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc , argv);
    glutCreateWindow("My First OpenGl");
    glutInitWindowSize(620,620);
    glutInitWindowPosition(50,50);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMainLoop();
    return 0;

}
