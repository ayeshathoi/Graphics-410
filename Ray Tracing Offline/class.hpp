#include<bits/stdc++.h>
#include "bitmap_image.hpp"
#include <GL/glut.h>
using namespace std;

int texture = 0;

struct Color
{
    double r, g, b;
    Color() {
        r = 0;
        g = 0;
        b = 0;
    }
    Color(double r, double g, double b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};



Color Scalar_Multiplication(double scalar, Color p) {
    Color ret;
    ret.r = scalar * p.r;
    ret.g = scalar * p.g;
    ret.b = scalar * p.b;
    return ret;
}


Color Addition(Color p1, Color p2) {
    Color ret;
    ret.r = p1.r + p2.r;
    ret.g = p1.g + p2.g;
    ret.b = p1.b + p2.b;
    return ret;
}

Color Substraction(Color p1, Color p2) {
    Color ret;
    ret.r = p1.r - p2.r;
    ret.g = p1.g - p2.g;
    ret.b = p1.b - p2.b;
    return ret;
}



//point
struct Point
{
    double x, y, z;
    Point(){}
    Point(double x,double y,double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    void Normalize() {
        double len = sqrt(x * x + y * y + z * z);
        x /= len;
        y /= len;
        z /= len;
    }

    double distance_calc(Point p) {
        return sqrt((this->x - p.x) * (this->x - p.x) + (this->y - p.y) * (this->y - p.y) + (this->z - p.z) * (this->z - p.z));
    }
};

Point CrossProduct(Point p1, Point p2) {
    Point ret;
    ret.x = p1.y * p2.z - p1.z * p2.y;
    ret.y = p1.z * p2.x - p1.x * p2.z;
    ret.z = p1.x * p2.y - p1.y * p2.x;
    return ret;
}

double DotProduct(Point p1, Point p2) {
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

Point Scalar_Multiplication(double scalar, Point p) {
    Point ret;
    ret.x = scalar * p.x;
    ret.y = scalar * p.y;
    ret.z = scalar * p.z;
    return ret;
}


Point Addition(Point p1, Point p2) {
    Point ret;
    ret.x = p1.x + p2.x;
    ret.y = p1.y + p2.y;
    ret.z = p1.z + p2.z;
    return ret;
}

Point Substraction(Point p1, Point p2) {
    Point ret;
    ret.x = p1.x - p2.x;
    ret.y = p1.y - p2.y;
    ret.z = p1.z - p2.z;
    return ret;
}
//--------------------------------------



struct Ray
{
    Point start_point, direction;
    Ray() {}
    Ray(Point start_point, Point direction) {
        this->start_point = start_point;
        direction.Normalize();
        this->direction = direction;
    }
};

struct point_Light
{
    Point point_light_position;
    Color point_light_color;
    double fall_off_param;
    point_Light() {}
    
    void Draw()
    {
        glColor3d(point_light_color.r, point_light_color.g,point_light_color.b);
        glPushMatrix();
        glTranslated(point_light_position.x, point_light_position.y,point_light_position.z);
        glutSolidSphere(10,100,100);
        glPopMatrix();
    }
};

struct spot_Light
{
    point_Light spot_light;
    Point spot_light_direction; // point at which it is looking
    double spot_light_cut_off_angle;
    double fall_off_param;
    spot_Light() {}
    
    void Draw()
    {
        glColor3d(spot_light.point_light_color.r,spot_light.point_light_color.g,spot_light.point_light_color.r);
        glPushMatrix();
        glTranslatef(spot_light.point_light_position.x,spot_light.point_light_position.y,spot_light.point_light_position.z);
        Point cross = CrossProduct(Point(0,0,1), spot_light_direction);
        cross.Normalize();
        double angle = acos(DotProduct(spot_light_direction,Point(0,0,1))) * 180 / M_PI;
        glRotated(angle,cross.x, cross.y, cross.z);
        glutSolidCone(10,20,100,100);
        glPopMatrix();
    }
};

//---------------------------------------------------------------------------

//for pyramid triangle and base intersection

bool isInsideTriangle(Point p0, Point p1, Point p2, Point p) {
    //barycentric coordinates
    Point side1 = Substraction(p1, p0);
    Point side2 = Substraction(p2, p0);
    Point side3 = Substraction(p, p0);

    Point check = CrossProduct(side1, side2);
    double denominator = DotProduct(check, check);
    double a = DotProduct(check, CrossProduct(side1, side3)) / denominator;
    double b = DotProduct(check, CrossProduct(side3, side2)) / denominator;
    double c = 1 - a - b;
    if (a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1)
    {
        return true;
    }
    return false;
}



double TriangleIntersection(Point p0, Point p1, Point p2, Ray ray) {
    Point edge1 = Substraction(p1, p0);
    Point edge2 = Substraction(p2, p0);
    Point h = CrossProduct(edge1, edge2);
    double a = DotProduct(ray.direction, h);
    double d = -DotProduct(h, p0);

    if(a == 0) {
        return -1;
    }

    double t = -(DotProduct(h, ray.start_point) + d) / a;
    if(t <= 0) {
        return -1;
    }

    Point p = Addition(ray.start_point, Scalar_Multiplication(t, ray.direction));
    if(isInsideTriangle(p0, p1, p2, p)) {
        return t;
    }
    return -1;
}

double SquareIntersection(Point p0, Point p1, Point p2, Point p3, Ray ray) {
    double t1 = TriangleIntersection(p0, p1, p2, ray);
    double t2 = TriangleIntersection(p0, p2, p3, ray);
    if(t1 > 0 && t2 > 0) {
        return min(t1, t2);
    }
    else if(t1 > 0) {
        return t1;
    }
    else if(t2 > 0) {
        return t2;
    }
    return -1;
}

//--------------------------------------------------------



struct Object{
    Point Reference_point;
    Color color;
    double height, width, length;
    double light_coefficients[4]; // ambient, diffuse, specular, reflection
    double shine; // specular
    virtual void draw() {}
    virtual double getIntersectingT(Ray ray) {
        return -1;
    };

    virtual Color getColor(Point intersection_point) {
        return Color(0, 0, 0);
    };

    virtual Point getNormal(Point intersection_point, Ray to_understand_ray_direction) {
        return Point(0, 0, 0);
    };

    virtual Color light(Object* object,Point intersection_point, Point view_direction){
        return Color(0, 0, 0);
    }
};

vector<point_Light*> point_lights;
vector<spot_Light*> spot_lights;
vector<Object*> objects;

// now write the code for the object --> Sphere, Pyramid, Cube and Plane.

class Floor_Plane : public Object
{
    public:
        double floor_width, tile_width;
        bitmap_image texture_w, texture_b;
        Floor_Plane(double t_width, double f_width)
        {
            this->floor_width = f_width;
            this->tile_width  = t_width;
            this->Reference_point = {-(floor_width / 2) , -(floor_width / 2), 0};
            this->length = t_width;
            this->texture_w = bitmap_image("./texture_w.bmp");
            this->texture_b = bitmap_image("./texture_b.bmp");
            
        }
        void draw(){

        if (texture == 0)
        {
            int loop = this->floor_width/this->tile_width;

            for(int i=-loop/2;i<=loop/2;i++){
                for(int j= -loop/2; j <= loop/2; j++ ){
                    if( (i+j)%2 == 0)
                        glColor3f(0,0,0);
                    else
                        glColor3f(1,1,1);
                    glBegin(GL_QUADS);{
                        glVertex3f( i*this->tile_width, j*this->tile_width,0);
                        glVertex3f( i*this->tile_width,(j+1)*this->tile_width,0);
                        glVertex3f((i+1)*this->tile_width,(j+1)*this->tile_width,0);
                        glVertex3f((i+1)*this->tile_width,j*this->tile_width,0);
                        }
                        glEnd();
                    }
		        }   
            }
        }

        double getIntersectingT(Ray ray)
        {
            double t = -1;
            double d = ray.direction.z;
            if(d != 0) {
                t = -ray.start_point.z / d;
            }
            return t;
        }

        Color getColor(Point intersection_point) {

            int rowx = (int)((intersection_point.x - this->Reference_point.x)/ this->tile_width);
            int rowy = (int)((intersection_point.y - this->Reference_point.y)/ this->tile_width);

            if(texture == 0)
            {
                if((rowx+rowy)%2)
                    return Color(1,1,1); //white
                else
                    return Color(0,0,0); //black
            }

            double x = (intersection_point.x - this->Reference_point.x ) - this->tile_width * rowx;
            double y = (intersection_point.y - this->Reference_point.y ) - this->tile_width * rowy;  
            
            if ((rowx+rowy)%2 == 0) // Black
            {
                int tex_x = (int)(this->texture_b.width() * x) / this->tile_width ;
                int tex_y = (int)(this->texture_b.height() * y) / this->tile_width;

                tex_x = tex_x % this->texture_b.width();
                tex_y = tex_y % this->texture_b.height();

                unsigned char r, g, b;
                this->texture_b.get_pixel(tex_x, tex_y, r, g, b);

                return Color((r / 255.0), (g / 255.0), (b / 255.0));
            }
            else if ((rowx+rowy)%2 == 1)
            {
                int tex_x = (int)(this->texture_w.width() * x) / this->tile_width;
                int tex_y = (int)(this->texture_w.height() * y) / this->tile_width;

                tex_x = tex_x % this->texture_w.width();
                tex_y = tex_y % this->texture_w.height();

                unsigned char r, g, b;
                this->texture_w.get_pixel(tex_x, tex_y, r, g, b);
                return Color((r / 255.0), (g / 255.0), (b / 255.0));
            }

            return Color(0, 0, 0);
        }

        // 0,0,-1 or 0,0,1
        Point getNormal(Point intersection_point, Ray to_understand_ray_direction) {
            Point ret;
            if(to_understand_ray_direction.direction.z > 0) {
                ret = Point(0, 0, -1);
            }
            else {
                ret = Point(0, 0, 1);
            }
            return ret;
           // return Point(0, 0, 1);
        }
};

class Cube : public Object
{
    public:
        Cube(Point p, double side)
        {
            this->Reference_point = p;
            this->length = side;
            this->width = side;
            this->height = side;
        }
        void draw()
        {
            glPushMatrix();
            glTranslatef(this->Reference_point.x,this->Reference_point.y,this->Reference_point.z);
            glColor3f(this->color.r,this->color.g,this->color.b);
            glBegin(GL_QUADS);{
                glVertex3f(0,0,0);
                glVertex3f(this->width,0,0);
                glVertex3f(this->width,this->height,0);
                glVertex3f(0,this->height,0);
            }glEnd();
            glBegin(GL_QUADS);{
                glVertex3f(0,0,0);
                glVertex3f(0,this->height,0);
                glVertex3f(0,this->height,this->length);
                glVertex3f(0,0,this->length);
            }glEnd();
            glBegin(GL_QUADS);{
                glVertex3f(0,0,0);
                glVertex3f(0,0,this->length);
                glVertex3f(this->width,0,this->length);
                glVertex3f(this->width,0,0);
            }glEnd();
            glBegin(GL_QUADS);{
                glVertex3f(this->width,0,0);
                glVertex3f(this->width,0,this->length);
                glVertex3f(this->width,this->height,this->length);
                glVertex3f(this->width,this->height,0);
            }glEnd();
            glBegin(GL_QUADS);{
                glVertex3f(0,this->height,0);
                glVertex3f(0,this->height,this->length);
                glVertex3f(this->width,this->height,this->length);
                glVertex3f(this->width,this->height,0);
            }glEnd();
            glBegin(GL_QUADS);{
                glVertex3f(0,0,this->length);
                glVertex3f(0,this->height,this->length);
                glVertex3f(this->width,this->height,this->length);
                glVertex3f(this->width,0,this->length);
            }glEnd();
            glPopMatrix();
        }

        bool insideCube(Point p){
            double epsilon = 1e-6;
            if(length != 0)
                if(p.x < Reference_point.x - epsilon || p.x > (Reference_point.x + length) + epsilon)
                    return false;
            if(width != 0)
                if(p.y < Reference_point.y - epsilon || p.y > (Reference_point.y + width) + epsilon)
                    return false;
            if(height != 0)
                if(p.z < Reference_point.z - epsilon || p.z > (Reference_point.z + height) + epsilon)
                    return false;

            return true;
        }
        
        Color getColor(Point intersection_point) {
            return this->color;
        }

        double getIntersectingT(Ray ray)
        {
            double t_min = -1;
            double t1 = -1, t2 = -1, t3 = -1, t4 = -1, t5 = -1, t6 = -1;
            if(ray.direction.x != 0) {
                t1 = (this->Reference_point.x - ray.start_point.x) / ray.direction.x;
                t2 = (this->Reference_point.x + this->width - ray.start_point.x) / ray.direction.x;
            }
            if(ray.direction.y != 0) {
                t3 = (this->Reference_point.y - ray.start_point.y) / ray.direction.y;
                t4 = (this->Reference_point.y + this->length - ray.start_point.y) / ray.direction.y;
            }
            if(ray.direction.z != 0) {
                t5 = (this->Reference_point.z - ray.start_point.z) / ray.direction.z;
                t6 = (this->Reference_point.z + this->height - ray.start_point.z) / ray.direction.z;
            }
            if(t1 > 0 && insideCube(Addition(ray.start_point, Scalar_Multiplication(t1, ray.direction)))) {
                t_min = t1;
            }
            if(t2 > 0 && insideCube(Addition(ray.start_point, Scalar_Multiplication(t2, ray.direction)))) {
                if(t_min < 0) {
                    t_min = t2;
                }
                else {
                    t_min = min(t_min, t2);
                }
            }
            if(t3 > 0 && insideCube(Addition(ray.start_point, Scalar_Multiplication(t3, ray.direction)))) {
                if(t_min < 0) {
                    t_min = t3;
                }
                else {
                    t_min = min(t_min, t3);
                }
            }
            if(t4 > 0 && insideCube(Addition(ray.start_point, Scalar_Multiplication(t4, ray.direction)))) {
                if(t_min < 0) {
                    t_min = t4;
                }
                else {
                    t_min = min(t_min, t4);
                }
            }
            if(t5 > 0 && insideCube(Addition(ray.start_point, Scalar_Multiplication(t5, ray.direction)))) {
                if(t_min < 0) {
                    t_min = t5;
                }
                else {
                    t_min = min(t_min, t5);
                }
            }
            if(t6 > 0 && insideCube(Addition(ray.start_point, Scalar_Multiplication(t6, ray.direction)))) {
                if(t_min < 0) {
                    t_min = t6;
                }
                else {
                    t_min = min(t_min, t6);
                }
            }
            return t_min;
        }

        Point getNormal(Point intersection_point, Ray to_understand_ray_direction) {
            Point ret;
            double t_min = -1;
            double t1 = -1, t2 = -1, t3 = -1, t4 = -1, t5 = -1, t6 = -1;
            if(to_understand_ray_direction.direction.x != 0) {
                t1 = (this->Reference_point.x - to_understand_ray_direction.start_point.x) / to_understand_ray_direction.direction.x;
                t2 = (this->Reference_point.x + this->width - to_understand_ray_direction.start_point.x) / to_understand_ray_direction.direction.x;
            }
            if(to_understand_ray_direction.direction.y != 0) {
                t3 = (this->Reference_point.y - to_understand_ray_direction.start_point.y) / to_understand_ray_direction.direction.y;
                t4 = (this->Reference_point.y + this->length - to_understand_ray_direction.start_point.y) / to_understand_ray_direction.direction.y;
            }
            if(to_understand_ray_direction.direction.z != 0) {
                t5 = (this->Reference_point.z - to_understand_ray_direction.start_point.z) / to_understand_ray_direction.direction.z;
                t6 = (this->Reference_point.z + this->height - to_understand_ray_direction.start_point.z) / to_understand_ray_direction.direction.z;
            }
            if(t1 > 0 && insideCube(Addition(to_understand_ray_direction.start_point, Scalar_Multiplication(t1, to_understand_ray_direction.direction)))) {
                t_min = t1;
                ret = {-1, 0, 0};
            }
            if(t2 > 0 && insideCube(Addition(to_understand_ray_direction.start_point, Scalar_Multiplication(t2, to_understand_ray_direction.direction)))) {
                if(t_min < 0 || t2 < t_min) {
                    t_min = t2;
                    ret = {1, 0, 0};
                }
            }
            if(t3 > 0 && insideCube(Addition(to_understand_ray_direction.start_point, Scalar_Multiplication(t3, to_understand_ray_direction.direction)))) {
                if(t_min < 0 || t3 < t_min) {
                    t_min = t3;
                    ret = {0, 1, 0};
                }
            }
            if(t4 > 0 && insideCube(Addition(to_understand_ray_direction.start_point, Scalar_Multiplication(t4, to_understand_ray_direction.direction)))) {
                if(t_min < 0 || t4 < t_min) {
                    t_min = t4;
                    ret = {0, -1, 0};
                }
            }

            if(t5 > 0 && insideCube(Addition(to_understand_ray_direction.start_point, Scalar_Multiplication(t5, to_understand_ray_direction.direction)))) {
                if(t_min < 0 || t5 < t_min) {
                    t_min = t5;
                    ret = {0, 0, -1};
                }
            }
            if(t6 > 0 && insideCube(Addition(to_understand_ray_direction.start_point, Scalar_Multiplication(t6, to_understand_ray_direction.direction)))) {
                if(t_min < 0 || t6 < t_min) {
                    t_min = t6;
                    ret = {0, 0, 1};
                }
            }
            // assert(ret.x != -1);
            return ret; 
        }
};


class Sphere : public Object
{
    public:
        double radius;
        Sphere(Point p, double r)
        {
            this->Reference_point = p;
            this->radius = r;
        }
        void draw(){
            glPushMatrix();
            glTranslatef(this->Reference_point.x,this->Reference_point.y,this->Reference_point.z);
            glColor3f(this->color.r,this->color.g,this->color.b);
            
            glutSolidSphere(this->radius,100,100);
            glPopMatrix();
        }

        double getIntersectingT(Ray ray)
        {
            double t = -1;
            double a = 1;
            double b = 2 * (ray.direction.x * (ray.start_point.x - this->Reference_point.x) + ray.direction.y * (ray.start_point.y - this->Reference_point.y) + ray.direction.z * (ray.start_point.z - this->Reference_point.z));
            double c = (ray.start_point.x - this->Reference_point.x) * (ray.start_point.x - this->Reference_point.x) + (ray.start_point.y - this->Reference_point.y) * (ray.start_point.y - this->Reference_point.y) + (ray.start_point.z - this->Reference_point.z) * (ray.start_point.z - this->Reference_point.z) - this->radius * this->radius;
            double d = b * b - 4 * a * c;
            if(d >= 0) {
                double t1 = (-b + sqrt(d)) / (2 * a);
                double t2 = (-b - sqrt(d)) / (2 * a);
                if(t1 > 0 && t2 > 0) {
                    t = min(t1, t2);
                }
                else if(t1 > 0) {
                    t = t1;
                }
                else if(t2 > 0) {
                    t = t2;
                }
            }
            return t;
        }

        Color getColor(Point intersection_point) {
            return this->color;
        }

        Point getNormal(Point intersection_point, Ray to_understand_ray_direction) {
            Point ret = Substraction(intersection_point, this->Reference_point);
            ret.Normalize();
            return ret;
        }
        
};


class Pyramid : public Object
{
    public:
        Pyramid(Point p, double h, double w)
        {
            this->Reference_point = p;
            this->height = h;
            this->width = w;
        }
        void draw()
        {
            Point bottom1 = this->Reference_point;
            Point bottom2 = Addition(this->Reference_point, Point(this -> width, 0, 0));
            Point bottom3 = Addition(this->Reference_point, Point(this->width,this->width,0));
            Point bottom4 = Addition(this->Reference_point, Point(0, this -> width, 0));
            Point Up      = Addition(this->Reference_point, Point(this->width/2, this->width/2, this->height));

            glColor3d(this->color.r, this->color.b, this->color.g);
            glBegin(GL_TRIANGLES);
            {
                glVertex3d(bottom1.x,bottom1.y, bottom1.z);
                glVertex3d(bottom2.x,bottom2.y, bottom2.z);
                glVertex3d(Up.x,Up.y, Up.z);
            }
            glBegin(GL_TRIANGLES);
            {
                glVertex3d(bottom3.x,bottom3.y, bottom3.z);
                glVertex3d(bottom2.x,bottom2.y, bottom2.z);
                glVertex3d(Up.x,Up.y, Up.z);
            }
            glBegin(GL_TRIANGLES);
            {
                glVertex3d(bottom4.x,bottom4.y, bottom4.z);
                glVertex3d(bottom3.x,bottom3.y, bottom3.z);
                glVertex3d(Up.x,Up.y, Up.z);
            }
            glBegin(GL_TRIANGLES);
            {
                glVertex3d(bottom1.x,bottom1.y, bottom1.z);
                glVertex3d(bottom4.x,bottom4.y, bottom4.z);
                glVertex3d(Up.x,Up.y, Up.z);
            }
            glBegin(GL_QUADS);
            {
                glVertex3d(bottom1.x,bottom1.y, bottom1.z);
                glVertex3d(bottom2.x,bottom2.y, bottom2.z);
                glVertex3d(bottom3.x,bottom3.y, bottom3.z);
                glVertex3d(bottom4.x,bottom4.y, bottom4.z);

            }
            glEnd();
        }

        double getIntersectingT(Ray ray)
        {
            Point bottom1 = this->Reference_point;
            Point bottom2 = Addition(this->Reference_point, Point(this -> width, 0, 0));
            Point bottom3 = Addition(this->Reference_point, Point(this->width,this->width,0));
            Point bottom4 = Addition(this->Reference_point, Point(0, this -> width, 0));
            Point Up      = Addition(this->Reference_point, Point(this->width/2, this->width/2, this->height));

            double t = -1;
            double t1 = TriangleIntersection(bottom1, bottom2, Up, ray);
            double t2 = TriangleIntersection(bottom2, bottom3, Up, ray);
            double t3 = TriangleIntersection(bottom3, bottom4, Up, ray);
            double t4 = TriangleIntersection(bottom4, bottom1, Up, ray);

            double t5 = SquareIntersection(bottom1, bottom2, bottom3, bottom4, ray);

            if(t1 > 0) {
                t = t1;
            }
            if(t2 > 0) {
                if(t < 0) {
                    t = t2;
                }
                else {
                    t = min(t, t2);
                }
            }
            if(t3 > 0) {
                if(t < 0) {
                    t = t3;
                }
                else {
                    t = min(t, t3);
                }
            }
            if(t4 > 0) {
                if(t < 0) {
                    t = t4;
                }
                else {
                    t = min(t, t4);
                }
            }

            if(t5 > 0) {
                if(t < 0) {
                    t = t5;
                }
                else {
                    t = min(t, t5);
                }
            }
            return t;

            
        }



        Color getColor(Point intersection_point) {
            return this->color;
        }

         Point getNormal(Point intersection_point, Ray to_understand_ray_direction) {
            Point bottom1 = this->Reference_point;
            Point bottom2 = Addition(this->Reference_point, Point(this -> width, 0, 0));
            Point bottom3 = Addition(this->Reference_point, Point(this->width,this->width,0));
            Point bottom4 = Addition(this->Reference_point, Point(0, this -> width, 0));
            Point Up      = Addition(this->Reference_point, Point(this->width/2, this->width/2, this->height));

            Point ret;
            if(isInsideTriangle(bottom1, bottom2, Up, intersection_point)) {
                ret = CrossProduct(Substraction(bottom2, bottom1), Substraction(Up, bottom1));
            }
            else if(isInsideTriangle(bottom2, bottom3, Up, intersection_point)) {
                ret = CrossProduct(Substraction(bottom3, bottom2), Substraction(Up, bottom2));
            }
            else if(isInsideTriangle(bottom3, bottom4, Up, intersection_point)) {
                ret = CrossProduct(Substraction(bottom4, bottom3), Substraction(Up, bottom3));
            }
            else if(isInsideTriangle(bottom4, bottom1, Up, intersection_point)) {
                ret = CrossProduct(Substraction(bottom1, bottom4), Substraction(Up, bottom4));
            }
            else if(isInsideTriangle(bottom1, bottom2, bottom3, intersection_point)) {
                ret = CrossProduct(Substraction(bottom2, bottom1), Substraction(bottom3, bottom1));
            }
            else if(isInsideTriangle(bottom2, bottom3, bottom4, intersection_point)) {
                ret = CrossProduct(Substraction(bottom3, bottom2), Substraction(bottom4, bottom2));
            }
            else if(isInsideTriangle(bottom3, bottom4, bottom1, intersection_point)) {
                ret = CrossProduct(Substraction(bottom4, bottom3), Substraction(bottom1, bottom3));
            }
            else if(isInsideTriangle(bottom4, bottom1, bottom2, intersection_point)) {
                ret = CrossProduct(Substraction(bottom1, bottom4), Substraction(bottom2, bottom4));
            }
            //check angle
            double angle = acos(DotProduct(ret, to_understand_ray_direction.direction)) * 180 / M_PI;
            if(angle > 90) {
                ret = Scalar_Multiplication(-1, ret);
            }
            ret.Normalize();
            return ret;

        }
};
