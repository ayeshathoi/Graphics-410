#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <GL/glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
double theta;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};

struct point pos = {0.707*150, 0, -0.707*150};
struct point u = {0, 1, 0};
struct point r = {0.707,0, 0.707};
struct point l = {-0.707, 0, 0.707};

struct point cubePoints[8];
double total_length=50;
double radius=20;
double height;


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f(100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

// void drawSquare(double a)
// {
//     glColor3f(1.0,1.0,1.0);
// 	glBegin(GL_QUADS);{
// 		glVertex3f( a, a,2);
// 		glVertex3f( a,-a,2);
// 		glVertex3f(-a,-a,2);
// 		glVertex3f(-a, a,2);
// 	}glEnd();
// }


// void drawCircle(double radius,int segments)
// {
//     int i;
//     struct point points[100];
//     glColor3f(0.7,0.7,0.7);
//     //generate points
//     for(i=0;i<=segments;i++)
//     {
//         points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
//         points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
//     }
//     //draw segments using generated points
//     for(i=0;i<segments;i++)
//     {
//         glBegin(GL_LINES);
//         {
// 			glVertex3f(points[i].x,points[i].y,0);
// 			glVertex3f(points[i+1].x,points[i+1].y,0);
//         }
//         glEnd();
//     }
// }

// void drawCone(double radius,double height,int segments)
// {
//     int i;
//     double shade;
//     struct point points[100];
//     //generate points
//     for(i=0;i<=segments;i++)
//     {
//         points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
//         points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
//     }
//     //draw triangles using generated points
//     for(i=0;i<segments;i++)
//     {
//         //create shading effect
//         if(i<segments/2)shade=2*(double)i/(double)segments;
//         else shade=2*(1.0-(double)i/(double)segments);
//         glColor3f(shade,shade,shade);

//         glBegin(GL_TRIANGLES);
//         {
//             glVertex3f(0,0,height);
// 			glVertex3f(points[i].x,points[i].y,0);
// 			glVertex3f(points[i+1].x,points[i+1].y,0);
//         }
//         glEnd();
//     }
// }


void drawCube()
{
    glColor3f(1,1,1);

    glBegin(GL_QUADS);{
         glVertex3f(-height/2,height/2,total_length/2);
         glVertex3f(height/2,height/2,total_length/2);
         glVertex3f(height/2,-height/2,total_length/2);
         glVertex3f(-height/2,-height/2,total_length/2);

         glVertex3f(total_length/2,height/2,height/2);
         glVertex3f(total_length/2,height/2,-height/2);
         glVertex3f(total_length/2,-height/2,-height/2);
         glVertex3f(total_length/2,-height/2,height/2);

         glVertex3f(-height/2,height/2,-total_length/2);
         glVertex3f(height/2,height/2,-total_length/2);
         glVertex3f(height/2,-height/2,-total_length/2);
         glVertex3f(-height/2,-height/2,-total_length/2);

         glVertex3f(-total_length/2,height/2,height/2);
         glVertex3f(-total_length/2,height/2,-height/2);
         glVertex3f(-total_length/2,-height/2,-height/2);
         glVertex3f(-total_length/2,-height/2,height/2);

         glVertex3f(-height/2,total_length/2,-height/2);
         glVertex3f(height/2,total_length/2,-height/2);
         glVertex3f(height/2,total_length/2,height/2);
         glVertex3f(-height/2,total_length/2,height/2);

         glVertex3f(-height/2,-total_length/2,-height/2);
         glVertex3f(height/2,-total_length/2,-height/2);
         glVertex3f(height/2,-total_length/2,height/2);
         glVertex3f(-height/2,-total_length/2,height/2);

    }glEnd();
}

void drawSphere(double radius,int slices,int stacks)
{
  //  glColor3f(1,0,0);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*1/2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*1/2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius,double cylh,int slices,int stacks)
{
    glColor3f(0,1,0);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=cylh/2*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(pi/2));
			points[i][j].y=r*sin(((double)j/(double)slices)*(pi/2));
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
//        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawSpheres()
{
    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,height/2,height/2);
    glRotatef(270,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,height/2,height/2);
    glRotatef(270,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(total_length/2-radius/2,height/2,height/2);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-height/2,-height/2,total_length/2-radius/2);
    glRotatef(-90,0,1,0);
    glRotatef(180,1,0,0);
    glRotatef(90,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,height/2,-height/2);
    glRotatef(180,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,-height/2,-height/2);
    glRotatef(180,0,1,0);
    glRotatef(180,1,0,0);
    glRotatef(90,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(total_length/2-radius/2,-height/2,height/2);
    glRotatef(180,1,0,0);
    glRotatef(90,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(total_length/2-radius/2,-height/2,-height/2);
    glRotatef(90,0,1,0);
    glRotatef(180,1,0,0);
    glRotatef(90,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(total_length/2-radius/2,height/2,-height/2);
    glRotatef(90,0,1,0);
    drawSphere(radius/2,25,25);
    glPopMatrix();

}

void drawCylinders()
{
    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,height/2,0);
    glRotatef(90,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-total_length/2+radius/2+height/2,height/2,height/2);
    glRotatef(270,0,1,0);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(total_length/2-radius/2,height/2,0);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-height/2,-height/2,total_length/2-radius/2-height/2);
    glRotatef(180,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-total_length/2+radius/2+height/2,-height/2,height/2);
    glRotatef(270,0,1,0);
    glRotatef(-90,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-total_length/2+radius/2+height/2,-height/2,-height/2);
    glRotatef(270,0,1,0);
    glRotatef(180,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-total_length/2+radius/2+height/2,height/2,-height/2);
    glRotatef(270,0,1,0);
    glRotatef(90,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(total_length/2-radius/2,-height/2,0);
    glRotatef(-90,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(total_length/2-radius/2,0,-height/2);
    glRotatef(-90,1,0,0);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,0,height/2);
    glRotatef(-90,1,0,0);
    glRotatef(180,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(total_length/2-radius/2,0,height/2);
    glRotatef(-90,1,0,0);
    glRotatef(-90,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(-total_length/2+radius/2,0,-height/2);
    glRotatef(-90,1,0,0);
    glRotatef(90,0,0,1);
    drawCylinder(radius/2,height,25,25);
    glPopMatrix();
}

void drawSS()
{

    height=total_length-radius;

    drawCube();
    drawSpheres();
    drawCylinders();

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
    //look left->clockwise rotation about u
        case '1':
            theta = 0.03;
			l.x = l.x*cos(theta)-r.x*sin(theta);
			l.y = l.y*cos(theta)-r.y*sin(theta);
			l.z = l.z*cos(theta)-r.z*sin(theta);

			r.x = l.x*sin(theta)+r.x*cos(theta);
			r.y = l.y*sin(theta)+r.y*cos(theta);
			r.z = l.z*sin(theta)+r.z*cos(theta);
			break;
    //look right->anticlockwise rotation about u
        case '2':
            theta = -0.03;
			l.x = l.x*cos(theta)-r.x*sin(theta);
			l.y = l.y*cos(theta)-r.y*sin(theta);
			l.z = l.z*cos(theta)-r.z*sin(theta);

			r.x = l.x*sin(theta)+r.x*cos(theta);
			r.y = l.y*sin(theta)+r.y*cos(theta);
			r.z = l.z*sin(theta)+r.z*cos(theta);
			break;

    //look up->clockwise rotation about r
       case '3':
            theta = 0.03;
			l.x = l.x*cos(theta)-u.x*sin(theta);
			l.y = l.y*cos(theta)-u.y*sin(theta);
			l.z = l.z*cos(theta)-u.z*sin(theta);

			u.x = l.x*sin(theta)+u.x*cos(theta);
			u.y = l.y*sin(theta)+u.y*cos(theta);
			u.z = l.z*sin(theta)+u.z*cos(theta);
			break;
    //look down->anticlockwise rotation about r
        case '4':
            theta = -0.03;
			l.x = l.x*cos(theta)-u.x*sin(theta);
			l.y = l.y*cos(theta)-u.y*sin(theta);
			l.z = l.z*cos(theta)-u.z*sin(theta);

			u.x = l.x*sin(theta)+u.x*cos(theta);
			u.y = l.y*sin(theta)+u.y*cos(theta);
			u.z = l.z*sin(theta)+u.z*cos(theta);
			break;
    //tilt clockwise->anticlockwise rotation about l
        case '5':
            theta = -0.03;
			u.x = u.x*cos(theta)-r.x*sin(theta);
			u.y = u.y*cos(theta)-r.y*sin(theta);
			u.z = u.z*cos(theta)-r.z*sin(theta);

			r.x = u.x*sin(theta)+r.x*cos(theta);
			r.y = u.y*sin(theta)+r.y*cos(theta);
			r.z = u.z*sin(theta)+r.z*cos(theta);
			break;
    //tilt anticlockwise->clockwise rotation about l
        case '6':
            theta = 0.03;
			u.x = u.x*cos(theta)-r.x*sin(theta);
			u.y = u.y*cos(theta)-r.y*sin(theta);
			u.z = u.z*cos(theta)-r.z*sin(theta);

			r.x = u.x*sin(theta)+r.x*cos(theta);
			r.y = u.y*sin(theta)+r.y*cos(theta);
			r.z = u.z*sin(theta)+r.z*cos(theta);
			break;


		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
//			move along negative l
			pos.x = pos.x - 3*l.x;
			pos.y = pos.y - 3*l.y;
			pos.z = pos.z - 3*l.z;
			break;
		case GLUT_KEY_UP:		// up arrow key
//			move along positive l
			pos.x = pos.x + 3*l.x;
			pos.y = pos.y + 3*l.y;
			pos.z = pos.z + 3*l.z;
			break;

		case GLUT_KEY_RIGHT:
//			move along negative r
            pos.x = pos.x - 3*r.x;
			pos.y = pos.y - 3*r.y;
			pos.z = pos.z - 3*r.z;
			break;
		case GLUT_KEY_LEFT:
//			move along positive r
            pos.x = pos.x + 3*r.x;
			pos.y = pos.y + 3*r.y;
			pos.z = pos.z + 3*r.z;
			break;

		case GLUT_KEY_PAGE_UP:
//			move along positive u
            pos.x = pos.x + 3*u.x;
			pos.y = pos.y + 3*u.y;
			pos.z = pos.z + 3*u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
//			move along negative u
            pos.x = pos.x - 3*u.x;
			pos.y = pos.y - 3*u.y;
			pos.z = pos.z - 3*u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		    if(radius<total_length)
            {
                radius+=1.0;
                height=total_length-radius;
            }

			break;
		case GLUT_KEY_END:
		    if(radius>0)
            {
                radius-=1.0;
                height=total_length-radius;
            }
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(pos.x,pos.y,pos.z,	0,0,0,	0,1,0);
    gluLookAt(pos.x,pos.y,pos.z,	pos.x+l.x,pos.y+l.y,pos.z+
              l.z,	u.x,u.y,u.z);
//

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();
    glEnable(GL_CLIP_PLANE0);
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}