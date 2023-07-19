#include <GL/glut.h> 
#include <cmath>

GLfloat max(GLfloat x, GLfloat y){
    return (x > y ? x : y);
}


GLfloat ratio          = 1;
GLfloat height         = 1; //single Triangle's height = 1
GLdouble RotationAngle = 0; // angle at which the octahedral is placed

//-------------------------------------------------
struct point {
    GLfloat x, y, z;
};
//-------------------------------------------------

//-------------------------------------------------

//Global Variable
point pos,l,r,u;
// eye-position, look Forward, right, up

GLfloat DegreeToRad = M_PI/180;   
GLfloat sideLength = (sqrt(2));

//-------------------------------------------------

point CrossProduct(point p, point q)
{
    point n; //normal
    n.x = p.y * q.z - q.y * p.z ;
    n.y = -(p.x * q.z - q.x * p.z) ;
    n.z = p.x * q.y - q.x * p.y ;
    return n;
}

//-------------------------------------------------
void drawTriangle()
{
    point triangleLeftX, triangleUpY, triangleRightZ;
    triangleLeftX.x = 1,triangleLeftX.y = 0, triangleLeftX.z = 0;
    triangleUpY.x = 0, triangleUpY.y = 1, triangleUpY.z = 0;
    triangleRightZ.x = 0,triangleRightZ.y = 0, triangleRightZ.z = 1;
    glBegin(GL_TRIANGLES);
    glVertex3f(triangleLeftX.x, triangleLeftX.y,triangleLeftX.z);
    glVertex3f(triangleUpY.z,triangleUpY.y,triangleUpY.z);
    glVertex3f(triangleRightZ.x,triangleRightZ.y,triangleRightZ.z);
    glEnd();
}
//-------------------------------------------------

void drawSphere(double radius, int stacks, int slices) {
    
    struct point points[stacks+1][slices+1];
    for (int j = 0; j <= stacks; j++) {
        double phi = -M_PI / 4.0 + j * M_PI / (2*stacks);
        point p2;
        p2.x = - 1 * sin(phi);
        p2.y = cos(phi);
        p2.z = 0;
        for (int i = 0; i < slices+1; i++) {
            double theta = - M_PI/4 + i * M_PI / (2 * slices);
            point p1;
            p1.x = - sin(theta);
            p1.y = 0;
            p1.z = -cos(theta);
            point normal = CrossProduct(p1, p2);
            float length = sqrt(normal.x* normal.x + normal.y * normal.y + normal.z * normal.z);
            normal.x = normal.x * radius/length;
            points[j][i].x = normal.x; 
            normal.y = normal.y * radius/length;
            points[j][i].y = normal.y;  
            normal.z = normal.z * radius/length;
            points[j][i].z = normal.z; 
        }
    }

    glBegin(GL_QUADS);
        for (int j = 0; j < stacks; j++) {
            for (int i = 0; i < slices; i++) {

                glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);

            }
        }

    glEnd();
}


void drawHalfSphere(double radius, int stacks, int slices) {
    
    struct point points[stacks+1][slices+1];
    for (int j = 0; j <= stacks; j++) {
        double phi = 0 + j * M_PI / (2*stacks) * 2;
        point p2;
        p2.x = - 1 * sin(phi);
        p2.y = cos(phi);
        p2.z = 0;
        for (int i = 0; i < slices+1; i++) {
            double theta = - M_PI/4 + i * M_PI / (2 * slices);
            point p1;
            p1.x = - sin(theta);
            p1.y = 0;
            p1.z = -cos(theta);
            point normal = CrossProduct(p1, p2);
            float length = sqrt(normal.x* normal.x + normal.y * normal.y + normal.z * normal.z);
            normal.x = normal.x * radius/length;
            points[j][i].x = normal.x; 
            normal.y = normal.y * radius/length;
            points[j][i].y = normal.y;  
            normal.z = normal.z * radius/length;
            points[j][i].z = normal.z; 
        }
    }

    glBegin(GL_QUADS);
        for (int j = 0; j < stacks; j++) {
            for (int i = 0; i < slices; i++) {

                glVertex3f(points[j][i].x, points[j][i].y, points[j][i].z);
                glVertex3f(points[j][i+1].x, points[j][i+1].y, points[j][i+1].z);

                glVertex3f(points[j+1][i+1].x, points[j+1][i+1].y, points[j+1][i+1].z);
                glVertex3f(points[j+1][i].x, points[j+1][i].y, points[j+1][i].z);

            }
        }

    glEnd();
}


//-------------------------------------------------

void drawCylinder(double height, double radius, int segments) {

    GLfloat cylinderAnglewithTriangle = 180 - 109.47 ;
    double tempx = radius * cos(-cylinderAnglewithTriangle/2 * DegreeToRad);
    double tempy = radius * sin( (-cylinderAnglewithTriangle/2 )* DegreeToRad);
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = -cylinderAnglewithTriangle/2 * DegreeToRad + ( i 
                        *cylinderAnglewithTriangle) * DegreeToRad/segments ;

            currx = radius * cos(theta);
            curry = radius * sin(theta);

            glVertex3f(currx,curry, height/2);
            glVertex3f(currx,curry, -height/2);
            
            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}



void drawHalfCylinder(double height, double radius, int segments) {

    GLfloat cylinderAnglewithTriangle = (180 - 109.47)/2 ;
    
    double tempx = radius * cos(0 * DegreeToRad);
    double tempy = radius * sin( (0 )* DegreeToRad);
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = 0 * DegreeToRad + ( i 
                        *cylinderAnglewithTriangle) * DegreeToRad/segments ;

            currx = radius * cos(theta);
            curry = radius * sin(theta);

            glVertex3f(currx,curry, height/2);
            glVertex3f(currx,curry, -height/2);
            
            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}

//-------------------------------------------------
void OctaHedral(GLfloat transX,GLfloat transY,GLfloat transZ)
{
    glPushMatrix();
    for(int i = 0; i < 4; i++){
    glColor3d(i % 2, 0, i % 2 == 0);
    glPushMatrix();
    glTranslatef(transX,transY,transZ);
    glScalef(ratio,ratio,ratio);
        
    drawTriangle();
    glPopMatrix();
    glRotatef(90, 0, 1, 0);
    }

    // glRotatef(180,1,0,0);
    // for(int i = 0; i < 4; i++){
    // glColor3d(i % 2, 0, i % 2 == 0);
    // glPushMatrix();
    // glTranslatef(transX,transY,transZ);
    // glScalef(ratio,ratio,ratio);  
    // drawTriangle();
    // glPopMatrix();
    // glRotatef(90, 0, 1, 0);
    // }

}

//-------------------------------------------------
void cubeSpehere(GLfloat r, GLfloat transR)
{
    for(int i = 1; i <= 4 ; i++)
    {
        glColor3f(i%2,i*.5,i%2==0);
        glPushMatrix();
        glTranslatef(ratio,0,0);
        glScalef(r-transR,r-transR,r-transR);
        drawHalfSphere(1,150,150);
        glPopMatrix();
        glRotatef(90,0,1,0);
    }

    glRotatef(90,0,0,1);
    for(int i = 1; i <= 1 ; i++)
    {
        glColor3f(i%2,i*.2,i%2==0);
        glPushMatrix();
        glTranslatef(ratio,0,0);
        glScalef(r-transR,r-transR,r-transR);
        drawSphere(1,150,150);
        glPopMatrix();
        glRotatef(180,0,0,1);
    }

}

//-------------------------------------------------
void CylinderPart(GLfloat r, GLfloat transR)
{
    glPushMatrix();
    for(int i=0;i<4;i++)
    {
    glPushMatrix();    
    glColor3f(.7,.8,.2);

    glRotatef(45,0,1,0);
    glTranslatef(sideLength/2 * ratio,0,0);
    //drawCylinder(sideLength * ratio ,r-transR,150);

    drawHalfCylinder(sideLength * ratio ,r-transR,150);

    glPopMatrix();
    glRotatef(90,0,1,0);
    }

    glPopMatrix();

    for(int i=0;i<4;i++)
    {
        glPushMatrix();
        glRotatef(45,0,0,1);
        glRotatef(90,1,0,0);
        glTranslatef(sideLength/2 * ratio,0,0);
        drawCylinder(sideLength * ratio ,r-transR,150);
        glPopMatrix();
        glRotatef(90,0,1,0);
    }

    // glRotatef(180,1,0,0);
    // for(int i=0;i<4;i++)
    // {
    //     glPushMatrix();
    //     glRotatef(45,0,0,1);
    //     glRotatef(90,1,0,0);
    //     glTranslatef(sideLength/2 * ratio,0,0);
    //     drawCylinder(sideLength* ratio ,r-transR,150);
    //     glPopMatrix();
    //     glRotatef(90,0,1,0);
    // }
}
//-------------------------------------------------
void display()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 


	gluLookAt(pos.x,pos.y,pos.z,
        pos.x+l.x,pos.y+l.y,pos.z+l.z,
        u.x,u.y,u.z);
    glRotated(RotationAngle, 0, 1, 0);
	 
    GLfloat angle = (90 - (109.47)/2) * DegreeToRad;
    GLfloat radius= sideLength/2 * cos(angle);
    GLfloat transR = radius* ratio;  
    GLfloat transXZ = (radius-transR) * cos(angle);
    GLfloat transX = transXZ * cos(45 * DegreeToRad);
    GLfloat transY = (radius-transR) * sin(angle);
    GLfloat transZ = transXZ * cos(45 * DegreeToRad);

    glPushMatrix();
    OctaHedral(transX,transY,transZ);
    cubeSpehere(radius,transR);
    glPopMatrix();
    CylinderPart(radius,transR);
    glPopMatrix();    
    glutSwapBuffers();


}

void keyboardListener(unsigned char key, int x, int y) {
    float theta;


    switch (key) {

    // rotate/look left -> clockwise rotation about u
    case '1':
		theta = 0.1;
		l.x = l.x*cos(theta)-r.x*sin(theta);
		l.y = l.y*cos(theta)-r.y*sin(theta);
		l.z = l.z*cos(theta)-r.z*sin(theta);

		r.x = l.x*sin(theta)+r.x*cos(theta);
		r.y = l.y*sin(theta)+r.y*cos(theta);
		r.z = l.z*sin(theta)+r.z*cos(theta);
		break;
        
    //rotate/look right -> anticlockwise rotation about u
    case '2':
        theta = -0.1;
		l.x = l.x*cos(theta)-r.x*sin(theta);
		l.y = l.y*cos(theta)-r.y*sin(theta);
		l.z = l.z*cos(theta)-r.z*sin(theta);

		r.x = l.x*sin(theta)+r.x*cos(theta);
		r.y = l.y*sin(theta)+r.y*cos(theta);
		r.z = l.z*sin(theta)+r.z*cos(theta);
		break;

    //look up -> clockwise rotation about r
    case '3':
        theta = 0.1;
		l.x = l.x*cos(theta)-u.x*sin(theta);
		l.y = l.y*cos(theta)-u.y*sin(theta);
		l.z = l.z*cos(theta)-u.z*sin(theta);

		u.x = l.x*sin(theta)+u.x*cos(theta);
		u.y = l.y*sin(theta)+u.y*cos(theta);
		u.z = l.z*sin(theta)+u.z*cos(theta);
		break;
    
    //look down -> anticlockwise rotation about r
    case '4':
        theta = -0.1;
		l.x = l.x*cos(theta)-u.x*sin(theta);
		l.y = l.y*cos(theta)-u.y*sin(theta);
		l.z = l.z*cos(theta)-u.z*sin(theta);

		u.x = l.x*sin(theta)+u.x*cos(theta);
		u.y = l.y*sin(theta)+u.y*cos(theta);
		u.z = l.z*sin(theta)+u.z*cos(theta);
		break;
    //tilt clockwise->anticlockwise rotation about l
    case '5':
        theta = -0.1;
		u.x = u.x*cos(theta)-r.x*sin(theta);
		u.y = u.y*cos(theta)-r.y*sin(theta);
		u.z = u.z*cos(theta)-r.z*sin(theta);

		r.x = u.x*sin(theta)+r.x*cos(theta);
		r.y = u.y*sin(theta)+r.y*cos(theta);
		r.z = u.z*sin(theta)+r.z*cos(theta);
		break;
    //tilt anticlockwise->clockwise rotation about l
    case '6':
        theta = 0.1;
		u.x = u.x*cos(theta)-r.x*sin(theta);
		u.y = u.y*cos(theta)-r.y*sin(theta);
		u.z = u.z*cos(theta)-r.z*sin(theta);

		r.x = u.x*sin(theta)+r.x*cos(theta);
		r.y = u.y*sin(theta)+r.y*cos(theta);
		r.z = u.z*sin(theta)+r.z*cos(theta);
		break;

    // rotate the object in the clockwise direction about its own axis
	case 'a':
        RotationAngle -= 10;
            if(RotationAngle < 0)RotationAngle += 360;
            break;
        break;

    //rotate the object in the counter-clockwise direction about its own axis
    case 'd':
        RotationAngle += 10;
            if(RotationAngle >= 360)RotationAngle -= 360;
            break;
        break;
        
    // Ratio --
    case ',' :
        ratio -= .01;
        ratio = max(ratio, 0.0);
    break;

    // Ratio ++
    case '.' :
    if(ratio < 1)
        ratio += .01;
        ratio = -max(-ratio, -1.0);
    break;


    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

void specialKeyListener(int key, int x,int y) {


    switch (key) {

   case GLUT_KEY_UP:	// move forward	
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		// move backward
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT: //move right
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT : //move left
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP: //move up
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN: //move down
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;
    
    default:
        return;
    }
    glutPostRedisplay();   
}


void reshapeListener(GLsizei width, GLsizei height) {  
    if (height == 0) height = 1;                
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();             
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}



void init()
{
    pos.x = 4    , pos.y = 4   , pos.z = 4;
    l.x   = -0.1 , l.y   = -0.1 , l.z   = -0.1;
    u.x   = 0    , u.y   = 0.1   , u.z   = 0;
    r.x   = 0.2  , r.y   = 0     , r.z   = -0.2;
}


int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	
	glutCreateWindow("My OpenGL Program");
    init();
    glEnable(GL_CLIP_PLANE0);
	glEnable(GL_DEPTH_TEST);	
	glutDisplayFunc(display);		
	glutReshapeFunc(reshapeListener);
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	
	glutMainLoop();		

	return 0;
}
