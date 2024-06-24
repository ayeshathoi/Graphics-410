#include<bits/stdc++.h>
#include<math.h>
#include "bitmap_image.hpp"
#include "class.hpp"
#include "Util.h"
using namespace std;

Point camera_pos, l, r, u;
string object_type;
void drawAxes()
{
    if (draw_axes == 0)
        return;
    

    glColor3f(0, 1, 0);
    glLineWidth(1);

    glBegin(GL_LINES);{
        glVertex3f( 1000,0,0);
        glVertex3f(-1000,0,0);

        glVertex3f(0,-1000,0);
        glVertex3f(0, 1000,0);

        glVertex3f(0,0, 1000);
        glVertex3f(0,0,-1000);
    }
    glEnd();
}

void input()
{
    ifstream input_from_description;
    input_from_description.open("description.txt");
    input_from_description >> nearplane_distance >> farplane_distance >> fovY >> aspectRatio;
    input_from_description >> recursion_level >> number_of_pixels_both_axes;
    input_from_description >> checkerboard_width ;
    Object *floor = new Floor_Plane(checkerboard_width,100000);


    input_from_description >> floor->light_coefficients[0] >> floor->light_coefficients[1] >>  floor->light_coefficients[3];
    floor->shine = 0;
    floor->light_coefficients[2] = 0;
    objects.push_back(floor);

    input_from_description >> number_of_objects;

    for (int i = 0 ; i < number_of_objects ; i++)
    {
        input_from_description >> object_type;
        if(object_type == "cube")
        {
            Point Bottom_left;
            input_from_description >> Bottom_left.x >> Bottom_left.y >> Bottom_left.z;
            input_from_description >> cube_side;
            Object *cube = new Cube(Bottom_left, cube_side);
            input_from_description >> cube->color.r >> cube->color.g >> cube->color.b;
            for(int i = 0; i < 4; i++)
            {
                input_from_description >> cube->light_coefficients[i];
            }
            
            input_from_description >> cube->shine;
            objects.push_back(cube);
        }
        if (object_type == "sphere")
        {
            Point center;
            input_from_description >> center.x >> center.y >> center.z;
            input_from_description >> sphere_radius;
            Object *sphere = new Sphere(center, sphere_radius);
            input_from_description >> sphere->color.r >> sphere->color.g >> sphere->color.b;
            for(int i = 0; i < 4; i++)
            {
                input_from_description >> sphere->light_coefficients[i];
            }
            input_from_description >> sphere->shine;
            objects.push_back(sphere);
        }
        if (object_type == "pyramid")
        {
            Point lowest_point;
            input_from_description >> lowest_point.x >> lowest_point.y >> lowest_point.z;
            input_from_description >> pyramid_width >> pyramid_height;
            Object *pyramid = new Pyramid(lowest_point, pyramid_height, pyramid_width);
            input_from_description >> pyramid->color.r >> pyramid->color.g >> pyramid->color.b;
            for(int i = 0; i < 4; i++)
            {
                input_from_description >> pyramid->light_coefficients[i];
            }
            //cout << pyramid->light_coefficients[2] << endl;
            input_from_description >> pyramid->shine;
            objects.push_back(pyramid);
        }
    }
    input_from_description >> number_of_normal_lights;

    for (int i = 0 ; i < number_of_normal_lights ; i++)
    {
        point_Light *pl = new point_Light();
        input_from_description >> pl->point_light_position.x >> pl->point_light_position.y >> pl->point_light_position.z;
        input_from_description >> pl->fall_off_param;
        pl->point_light_color = {1,1,0}; // can be used a random function
        point_lights.push_back(pl);
    }

    input_from_description >> number_of_spot_lights;

    for (int i = 0 ; i < number_of_spot_lights ; i++)
    {
        spot_Light *sl = new spot_Light();
        input_from_description >> sl->spot_light.point_light_position.x >> sl->spot_light.point_light_position.y >> sl->spot_light.point_light_position.z;
        input_from_description >> sl->fall_off_param;
        input_from_description >> sl->spot_light_direction.x >> sl->spot_light_direction.y >> sl->spot_light_direction.z;
        input_from_description >> sl->spot_light_cut_off_angle;
        sl->spot_light_direction = Substraction(sl->spot_light_direction, sl->spot_light.point_light_position);
        sl->spot_light.point_light_color = {.8,.8,.8};
        sl->spot_light_direction.Normalize();
        spot_lights.push_back(sl);
    }
    cout << "Done Taking input" << endl;
    input_from_description.close();
}

void DrawAll()
{
    drawAxes();
    for(int i = 0; i < objects.size(); i++)
        objects[i]->draw();
    for(int i = 0; i < number_of_normal_lights; i++)
        point_lights[i]->Draw();
    for(int i = 0; i < number_of_spot_lights; i++)
        spot_lights[i]->Draw();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, 
                camera_pos.x + l.x, camera_pos.y + l.y, camera_pos.z + l.z, u.x, u.y, u.z);
    DrawAll();
    glutSwapBuffers();
}

void init()
{
    //init pos
    camera_pos = {0,-200,50};
    l = {0,1,0};
    u = {0,0,1};
    r = {1,0,0};
    u.Normalize();
    r.Normalize();
    l.Normalize();

}


void reshapeListener(GLsizei width, GLsizei height) {  
    if (height == 0) height = 1;                
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity();             
    gluPerspective(fovY, aspect, nearplane_distance, farplane_distance);
}


Object* getNearest(Ray ray){
    double t = -1;
    Object* nearest_object = NULL;
    for(int i = 0; i < objects.size(); i++){
        double temp = objects[i]->getIntersectingT(ray);
        if(temp > 0 && (t < 0 || temp < t)){
            t = temp;
            nearest_object = objects[i];
        }
    }
    return nearest_object;
}

Color getRayColor(int recursion_level, Ray ray)
{
    if(recursion_level < 0)
    {
        return {0,0,0};
    }

    Object* nearest_object = getNearest(ray);
    if(nearest_object == NULL)
    {
        return {0,0,0};
    }
    double t = nearest_object->getIntersectingT(ray);
    if(t > farplane_distance)
    {
        return {0,0,0};
    }
    Point intersection_point = Addition(ray.start_point, Scalar_Multiplication(t, ray.direction));


    Point normal = nearest_object->getNormal(intersection_point, ray);
    normal.Normalize();
    Point reflected_ray_direction = Substraction(ray.direction, Scalar_Multiplication(2 * DotProduct(ray.direction, normal), normal));
    reflected_ray_direction.Normalize();
    double epsilon = 1e-6;

    // When sending a reflected ray from intersecting point P, do not start it at P. Rather,
    //advance it a little bit. Otherwise, the ray may generate intersection at the same object.
    Ray reflected_ray(Addition(intersection_point, Scalar_Multiplication(epsilon, reflected_ray_direction)), reflected_ray_direction);
    //phong model
    double lambert = 0;
    double phong   = 0;
    for(int i= 0 ; i < number_of_normal_lights; i++)
    {
        Point light_direction = Substraction(point_lights[i]->point_light_position, intersection_point);
        double light_direction_magnitude = intersection_point.distance_calc(point_lights[i]->point_light_position);
        light_direction.Normalize();

        // When sending a reflected ray from intersecting point P, do not start it at P. Rather,
        //advance it a little bit. Otherwise, the ray may generate intersection at the same object.
        Ray light_ray(Addition(intersection_point, Scalar_Multiplication(epsilon, light_direction)), light_direction);
        Object* nearest_object_from_light = getNearest(light_ray);
        double t = -1;
        if(nearest_object_from_light != NULL)t = nearest_object_from_light->getIntersectingT(light_ray);
        if(t <= epsilon || t > light_direction_magnitude)
        {
            double scaling_factor = exp(-light_direction_magnitude * light_direction_magnitude * point_lights[i]->fall_off_param);
            lambert+= max(DotProduct(light_direction,normal) ,0.0)* scaling_factor;
            phong+= pow(DotProduct(reflected_ray_direction, light_direction),nearest_object->shine) * scaling_factor; 

        }
    }

    for(int i= 0 ; i < number_of_spot_lights; i++)
    {
        Point light_direction = Substraction(spot_lights[i]->spot_light.point_light_position, intersection_point);
        Point light_direction_reverse = Substraction(intersection_point, spot_lights[i]->spot_light.point_light_position);
        double light_direction_magnitude = intersection_point.distance_calc(spot_lights[i]->spot_light.point_light_position);
        light_direction.Normalize();
        light_direction_reverse.Normalize();

        // When sending a reflected ray from intersecting point P, do not start it at P. Rather,
        //advance it a little bit. Otherwise, the ray may generate intersection at the same object.
        Ray light_ray(Addition(intersection_point, Scalar_Multiplication(epsilon, light_direction)), light_direction);
        Object* nearest_object_from_light = getNearest(light_ray);
        double t = -1;
        if(nearest_object_from_light != NULL)t = nearest_object_from_light->getIntersectingT(light_ray);
        if(t <= epsilon || t > light_direction_magnitude)
        {
            double angle = acos(DotProduct(light_direction_reverse, spot_lights[i]->spot_light_direction)) * 180 / M_PI;
            angle = min(angle, 180-angle);
            if(angle <= spot_lights[i]->spot_light_cut_off_angle)
            {
                double scaling_factor = exp(-light_direction_magnitude * light_direction_magnitude * spot_lights[i]->fall_off_param);
                lambert+= max(DotProduct(light_direction,normal),0.0) * scaling_factor;
                phong+= pow(max(DotProduct(reflected_ray_direction, light_direction),0.0),nearest_object->shine) * scaling_factor;
            } 

        }
    }
    Color color = {0,0,0};
    color = Addition(color, Scalar_Multiplication(nearest_object->light_coefficients[0], nearest_object->getColor(intersection_point)));
    Color temp = color;
    color = Addition(color, Scalar_Multiplication(nearest_object->light_coefficients[1] * lambert,  nearest_object->getColor(intersection_point)));
    color = Addition(color, Scalar_Multiplication(nearest_object->light_coefficients[2] * phong,  nearest_object->getColor(intersection_point)));

    // add reflection
    Color reflected_color = getRayColor(recursion_level - 1, reflected_ray);
    color = Addition(color, Scalar_Multiplication(nearest_object->light_coefficients[3], reflected_color));


    return color;
    
}

void capture()
{
    Point center_buffer = Addition(camera_pos,Scalar_Multiplication(nearplane_distance,l));
    double fovY_buffer = fovY * M_PI/180;
    double fovX_buffer = fovY * aspectRatio * M_PI/180;
    double height = 2 * nearplane_distance * tan(fovY_buffer/2);
    double width  = 2 * nearplane_distance * tan(fovX_buffer/2);
    double dx = width / number_of_pixels_both_axes;
    double dy = height / number_of_pixels_both_axes;


    Point **buffer;
    buffer = new Point*[number_of_pixels_both_axes];
    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        buffer[i] = new Point[number_of_pixels_both_axes];
    }

    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        for(int j = 0 ; j < number_of_pixels_both_axes; j ++)
        {
            Point temp = Addition(center_buffer, Scalar_Multiplication(dx * (i - number_of_pixels_both_axes/2), r));
            temp = Addition(temp, Scalar_Multiplication(dy * (j - number_of_pixels_both_axes/2), u));
            buffer[i][j] = temp;
        }
    }

    cout << "Point buffer generation done" << endl;

    Color **color_buffer;
    color_buffer = new Color*[number_of_pixels_both_axes];
    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        color_buffer[i] = new Color[number_of_pixels_both_axes];
    }


    int part = number_of_pixels_both_axes / 10;
    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        for(int j = 0 ; j < number_of_pixels_both_axes; j ++)
        {
            Point ray_direction = Substraction(buffer[i][j], camera_pos);
            ray_direction.Normalize();
            Ray ray(buffer[i][j], ray_direction);
            color_buffer[i][j] = getRayColor(recursion_level, ray);
        }

        if(i%part == part - 1)
        {
            cout << "Rendering " << (i/part + 1)*10 << "% complete" << endl;
        }
    }

    if (texture == 1)
        cout << "Rendering image with texture" << endl;

    bitmap_image image(number_of_pixels_both_axes, number_of_pixels_both_axes);

    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        for(int j = 0 ; j < number_of_pixels_both_axes; j ++)
        {
            image.set_pixel(i,number_of_pixels_both_axes - 1 - j,color_buffer[i][j].r * 255, color_buffer[i][j].g * 255, color_buffer[i][j].b * 255);
        }
    }

    image.save_image("out.bmp"); 
    cout << "Image Saved " <<endl;

    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        delete[] buffer[i];
    }
    delete[] buffer;

    for(int i = 0; i < number_of_pixels_both_axes; i++)
    {
        delete[] color_buffer[i];
    }
    delete[] color_buffer;

}



void keyboardListener(unsigned char key, int x, int y) {
    float theta;


    switch (key) {

    // rotate/look left -> clockwise rotation about u
    case '1':
		theta = .3;
		l.x = l.x*cos(theta)-r.x*sin(theta);
		l.y = l.y*cos(theta)-r.y*sin(theta);
		l.z = l.z*cos(theta)-r.z*sin(theta);
        r = CrossProduct(l, u);
		break;
        
    //rotate/look right -> anticlockwise rotation about u
    case '2':
        theta = -0.3;
		l.x = l.x*cos(theta)-r.x*sin(theta);
		l.y = l.y*cos(theta)-r.y*sin(theta);
		l.z = l.z*cos(theta)-r.z*sin(theta);
        r = CrossProduct(l, u);
		break;

    //look up -> clockwise rotation about r
    case '3':
        theta = 0.3;
		l.x = l.x*cos(theta)-u.x*sin(theta);
		l.y = l.y*cos(theta)-u.y*sin(theta);
		l.z = l.z*cos(theta)-u.z*sin(theta);
        u = CrossProduct(r, l);
		break;
    
    //look down -> anticlockwise rotation about r
    case '4':
        theta = -0.3;
		l.x = l.x*cos(theta)-u.x*sin(theta);
		l.y = l.y*cos(theta)-u.y*sin(theta);
		l.z = l.z*cos(theta)-u.z*sin(theta);
        u = CrossProduct(r, l);
		break;
    //tilt clockwise->anticlockwise rotation about l
    case '5':
        theta = -0.3;
		u.x = u.x*cos(theta)-r.x*sin(theta);
		u.y = u.y*cos(theta)-r.y*sin(theta);
		u.z = u.z*cos(theta)-r.z*sin(theta);
        r = CrossProduct(l, u);
		break;
    //tilt anticlockwise->clockwise rotation about l
    case '6':
        theta = 0.3;
		u.x = u.x*cos(theta)-r.x*sin(theta);
		u.y = u.y*cos(theta)-r.y*sin(theta);
		u.z = u.z*cos(theta)-r.z*sin(theta);
        r = CrossProduct(l, u);
		break;

    //capture
    case '0':
        capture();
        break;

    case ' ':
        texture = 1 - texture;
        break;

    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    case 'a':
        draw_axes = 1 - draw_axes;
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

void specialKeyListener(int key, int x,int y) {


    switch (key) {

        case GLUT_KEY_UP:	// move forward	
			camera_pos = {camera_pos.x + 10 * l.x, camera_pos.y + 10 * l.y, camera_pos.z + 10 * l.z};
			break;
		case GLUT_KEY_DOWN:		// move backward
            camera_pos = {camera_pos.x - 10 * l.x, camera_pos.y - 10 * l.y, camera_pos.z - 10 * l.z};
			break;

		case GLUT_KEY_RIGHT: //move right
            camera_pos = {camera_pos.x + r.x, camera_pos.y + r.y, camera_pos.z + r.z};
			break;
		case GLUT_KEY_LEFT : //move left
            camera_pos = {camera_pos.x - r.x, camera_pos.y - r.y, camera_pos.z - r.z};
			break;

		case GLUT_KEY_PAGE_UP: //move up
            camera_pos = {camera_pos.x + u.x, camera_pos.y + u.y, camera_pos.z + u.z};
			break;
		case GLUT_KEY_PAGE_DOWN: //move down
            camera_pos = {camera_pos.x - u.x, camera_pos.y - u.y, camera_pos.z - u.z};
			break;
    
    default:
        return;
    }
    glutPostRedisplay();   
}

void mouseListener(int button, int state, int x, int y){
	switch(button){
		case GLUT_LEFT_BUTTON:
            draw_axes = 1 - draw_axes;
            break;



		default:
			break;
    
	}
    glutPostRedisplay();
}


void freeMemory()
{
    for(int i = 0;i <= objects.size(); i++){
        delete objects.at(i);
	}
	for(int i = 0;i <= point_lights.size(); i++){
        delete point_lights.at(i);
	}
	for(int i = 0;i <= spot_lights.size(); i++){
        delete spot_lights.at(i);
	}

}

int main(int argc, char **argv){

    input();
    
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	
	glutCreateWindow("Ray");
    init();
	glEnable(GL_DEPTH_TEST);	
	glutDisplayFunc(display);		
	glutReshapeFunc(reshapeListener);
	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);
	glutMainLoop();

    freeMemory();

	return 0;
}
