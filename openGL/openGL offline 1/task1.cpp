#include<iostream>
#include<GL/glut.h>
#include <math.h>

#include <time.h>
#include <sys/timeb.h>

#define HOUR_HAND_LENGTH .09
#define MINUTE_HAND_LENGTH .1
#define SECOND_HAND_LENGTH .125

#define CIRCLE_MARK_RADIUS .17

double secondAngle = 0 , minuteAngle = 0 , hourAngle = 0;
double PendulumAngle = 0;
double PendulumMaxAngle = 30;
int updateInterval = 20;
int WindowWidth = 640;
int WindowLength = 640;
int origin = 0;


typedef struct Point {
    double x , y;
}point;



//Main Circle
point MainCirclePosition;
double mainCircleR = .2;
//middle Black Circle
double middleCircleR = .19;
// point of the hands - circle
double smallCircleR = .015;

int currentMilliSecond = 0;


void Intro()
{
    MainCirclePosition.x = .3;
    MainCirclePosition.y = .45;


    time_t tim=time(0);
	struct tm* t;
	t=localtime(&tim); 
    currentMilliSecond = (t->tm_hour % 12 * 3600000 + t->tm_min * 60000 + t->tm_sec * 1000) ;
}


void drawHand(double y , double angle) {
    glBegin(GL_LINES);
	glVertex2f(0 , 0);
	glVertex2f(y*cos(angle) , y*sin(angle) );
    glEnd();
}

void drawPoint(point p)
{
    glBegin(GL_POINTS); 
        glVertex2d(p.x, p.y);
    glEnd();
}

void drawLine(point p1,point p2)
{
    // glPointSize(pSize);
    glBegin(GL_LINES);
        glVertex2d(p1.x,p1.y);
        glVertex2d(p2.x,p2.y);
    glEnd();
}

void drawMarks(void) {
	point p,p2;
    float linediff = CIRCLE_MARK_RADIUS - .03;
	int count = 0;
	double i = M_PI/6.0;

    glPushMatrix();
    glTranslatef(MainCirclePosition.x,MainCirclePosition.y,0);
	
	for(i=0 ; i<=2*M_PI ; i+=M_PI/30.0) {
		if(i==M_PI/2.0) {
			p.x  = CIRCLE_MARK_RADIUS;
            p2.x = linediff;
		}
		else if(i==3*M_PI/2.0) {
			p.x = -CIRCLE_MARK_RADIUS;
            p2.x = -linediff;
		}
		else {
			p.x = CIRCLE_MARK_RADIUS*sin(i);
            p2.x = linediff * sin(i);
		}
		p.y  = CIRCLE_MARK_RADIUS * cos(i);
        p2.y = linediff * cos(i); 
;
		if(count%5==0) {
            drawLine(p,p2); // draw the 3,6,9,12 hour
			
		}
		else {
			glPointSize(1.0);
			drawPoint(p);
		}
		count++;
	}
    glPopMatrix();
}



void drawCircle(float cx,float cy, float rx, float ry)
{
    glBegin(GL_POLYGON);    // All vertices form a single polygon
        for (float theta = 0; theta < 360; theta++) {
            float x = cx + rx * cos(theta/180*M_PI);
            float y = cy + ry * sin(theta/180*M_PI);
            glVertex2f(x,y);
    }
    glEnd();
}


void drawCircle_Unfilled(float cx,float cy, float rx, float ry)
{
    glBegin(GL_LINE_LOOP);    // All vertices form a single polygon
        for (float theta = 0; theta < 360 ; theta ++) {
            float x = cx + rx * cos(theta/180*M_PI);
            float y = cy + ry * sin(theta/180*M_PI);
            glVertex2f(x,y);
    }
    glEnd();
}


void drawTriangle(double x1,double y1,double x2,double y2,double x3,double y3)
{
    glBegin(GL_TRIANGLES);
        glVertex2d(x1,y1);
        glVertex2d(x2,y2);
        glVertex2d(x3,y3);
    glEnd(); 

}

void drawQuad(double x1, double y1, double x2, double y2,
                double x3, double y3,double x4, double y4)
{
    glBegin(GL_QUADS);
        glVertex2d(x1,y1);
        glVertex2d(x2,y2);
        glVertex2d(x3,y3);
        glVertex2d(x4,y4);
    glEnd();
}


void drawLine(double a, double b){
    glBegin(GL_LINES);{
        glVertex2d(0, a);
        glVertex2d(0, b);
    }glEnd();
}


void drawUpperTriangle()
{
    point triangleleft, triangleUp,triangleRight;
    triangleleft.x = -.1, triangleleft.y = .6;
    triangleUp.x = .3, triangleUp.y = .9;
    triangleRight.x = .7,triangleRight.y = .6;
    drawTriangle(triangleleft.x,triangleleft.y,triangleUp.x,triangleUp.y,triangleRight.x,triangleRight.y);
}


void drawMiddlePentagon()
{
    point topLeft, topUp,topRight, bottomRight,bottomLeft;
    topLeft.x = 0, topLeft.y = .6;
    topUp.x = 0.3, topUp.y = .8;
    topRight.x = .6, topRight.y = .6;
    bottomRight.x = .5, bottomRight.y = .2;
    bottomLeft.x = .1, bottomLeft.y = .2;
    glBegin(GL_POLYGON);
    glColor3f(0.5f, 0.35f, 0.05f);
        glVertex2d(topLeft.x,topLeft.y);
        glVertex2d(topUp.x,topUp.y);
        glVertex2d(topRight.x,topRight.y);
        glVertex2d(bottomRight.x,bottomRight.y);
        glVertex2d(bottomLeft.x,bottomLeft.y);
    glEnd(); 

}


void LowerPolygon()
{
    point topLeft, bottomDown,topRight, bottomRight,bottomLeft;
    topLeft.x = 0.1, topLeft.y = .2;
    bottomLeft.x = 0, bottomLeft.y = .1;
    bottomDown.x = .3, bottomDown.y = .15;
    bottomRight.x = .6, bottomRight.y = .1;
    topRight.x = .5, topRight.y = .2;
    glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.2f, 0.0f);
        glVertex2d(0.1,.2);
        glVertex2d(0,.1);
        glVertex2d(.3,.15);
        glVertex2d(0.6,.1);
        glVertex2d(0.5,.2);
    glEnd(); 

    // point topLeft, bottomMiddle,topRight, bottomRight,bottomLeft;
    // topLeft.x = 0.1, topLeft.y = .2;
    // bottomLeft.x = 0, bottomLeft.y = .1;
    // bottomMiddle.x = .3, bottomMiddle.y = .15;
    // bottomRight.x = .6, bottomRight.y = .1;
    // topRight.x = .5, bottomRight.y = .2;
    // glBegin(GL_POLYGON);
    // glColor3f(0.2f, 0.2f, 0.0f);
    //     glVertex2d(topLeft.x,topLeft.y);
    //     glVertex2d(bottomLeft.x,bottomLeft.y);
    //     glVertex2d(bottomMiddle.x,bottomMiddle.y);
    //     glVertex2d(bottomRight.x,bottomRight.y);
    //     glVertex2d(topRight.x,topRight.y);
        
    // glEnd(); 

}


void DrawPendulum()
{
    //pendulum Stick
    point PendulumCenter,PendulumUpperPoint,PendulumLowerPoint;
    PendulumCenter.x = .3, PendulumCenter.y = .45;
    PendulumUpperPoint.x = 0;
    PendulumLowerPoint.x = 0;
    PendulumUpperPoint.y = -.65;
    PendulumLowerPoint.y = .2;

    //Bob Radius
    point BobCenter,BobOvalRadius;
    BobCenter.x = 0;
    BobCenter.y = -.65;
    BobOvalRadius.x = .08;
    BobOvalRadius.y = .05;
    
    glColor3f(0.5f, 0.35f, 0.05f);
    glPushMatrix();
    glLineWidth(10);
    glTranslatef(PendulumCenter.x,PendulumCenter.y,0);
    glRotatef(PendulumAngle,PendulumUpperPoint.x,PendulumUpperPoint.x,1);
    drawLine(PendulumUpperPoint,PendulumLowerPoint);
    drawCircle(BobCenter.x,BobCenter.y,BobOvalRadius.x, BobOvalRadius.y);
    glPopMatrix();

}



void drawMainCircles()
{
    glColor3f(0, 0, 0);
    drawCircle(MainCirclePosition.x,MainCirclePosition.y,mainCircleR,mainCircleR);
    
    glColor3f(1, 1, 1);
    drawCircle_Unfilled(MainCirclePosition.x,MainCirclePosition.y,middleCircleR,middleCircleR);
    
    glColor3f(1, 1, 1);
    drawCircle(MainCirclePosition.x,MainCirclePosition.y,smallCircleR,smallCircleR);

}

void drawHands()
{
    glPushMatrix();
    glColor3f(1, 1, 1);
    glTranslatef(MainCirclePosition.x,MainCirclePosition.y,0);
    glLineWidth(1.0);
    drawHand(SECOND_HAND_LENGTH,-secondAngle + M_PI/2);
    glLineWidth(3.0);
    drawHand(MINUTE_HAND_LENGTH,-minuteAngle+M_PI/2);
    glLineWidth(5.0);
    drawHand(HOUR_HAND_LENGTH,-hourAngle+M_PI/2);
    glPopMatrix();
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();    

    glPushMatrix();

    glColor3f(0.2f, 0.2f, 0.0f);
    drawUpperTriangle();
    drawMiddlePentagon();
    DrawPendulum();
    LowerPolygon();
    drawMainCircles();
    drawHands();

    //quad-shaped HourHand
    // glPushMatrix();
    // //glRotatef(-secondAngle + M_PI/2,0,0,1);
    // drawQuad(.3,.58,.31,.50,.3,.455,.29,.5);
    // glPopMatrix();
    //-----------------------------------------------

    glPushMatrix();
    glLineWidth(1);
    drawMarks();
    glPopMatrix();  

    glutSwapBuffers();
    glFlush();

}

void update(int value){

    glutTimerFunc(updateInterval, update, 0); 
    time_t timer=time(0);
	struct tm* t;
	t=localtime(&timer); 

    currentMilliSecond = (currentMilliSecond + updateInterval) % (updateInterval*100);

    if(t->tm_hour > 12)
        t->tm_hour -=12;


	secondAngle = (double)(t->tm_sec)/30.0 * M_PI;
	minuteAngle = (double)(t->tm_min)/30.0 * M_PI + secondAngle/60.0;
	hourAngle = (double)t->tm_hour/6.0 * M_PI+ minuteAngle/12.0;
    
    PendulumAngle = PendulumMaxAngle * cos(currentMilliSecond /1000.0 * M_PI + 0);

    glutPostRedisplay(); 
    
    
}


void reshape(GLsizei width, GLsizei height) {  
   
    if (height == 0) height = 1;               
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(origin, origin, width, height);
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();           
    if (width >= height) {
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(WindowWidth,WindowLength);
    glutInitWindowPosition(origin,origin);
    glutCreateWindow("My OpenGL Program"); //Title

    Intro();
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutReshapeFunc(reshape); 
    glutMainLoop();
    return 0;
}