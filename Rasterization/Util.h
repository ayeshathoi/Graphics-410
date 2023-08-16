#include<bits/stdc++.h>
using namespace std;
double radian = M_PI/180; 

// -------------------------------------------------

struct point
{
    double x,y,z;
};

struct color{
    double r,g,b;
};

struct triangle{
    point p[3];
    color clr;
};


static unsigned long int g_seed = 1;
inline int randomColor()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}


struct Array2D
{
    double arr_matrix[4][4];
};

// -------------------------------------------------

Array2D generate_identity_matrix()
{
    Array2D IdentityMatrix;
    for(int i = 0; i < 4 ; i++)
    {
        for(int j = 0; j< 4 ;j++)
            {
                IdentityMatrix.arr_matrix[i][j] = 0;
                if(i==j)
                    IdentityMatrix.arr_matrix[i][j] = 1;
            }
    }
    return IdentityMatrix;
}

double dot_product(point a, point b)
{
    double dotValue = a.x*b.x + a.y*b.y + a.z*b.z;
    return dotValue;
}

point crossProduct(point a, point b)
{
    point crossProductValue = {a.y*b.z - a.z*b.y , a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
    return crossProductValue;
}

point normalization(point p)
{
    double length = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
    point normalized_point = {p.x/length,p.y/length,p.z/length};
    return normalized_point;
}

Array2D matrix_multiplication(Array2D matrix1, Array2D matrix2)
{
    Array2D result;
    for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4 ; j++)
            {
                result.arr_matrix[i][j] = 0;
                for(int k = 0; k < 4; k++)
                    result.arr_matrix[i][j] += matrix1.arr_matrix[i][k] * matrix2.arr_matrix[k][j];
            }
        }
    return result;
}

// -------------------------------------------------

point Rotation_rodriguez(point p1, point p2, double radian_angle)
{
    point result;
    point  k    = crossProduct(p1,p2);
    double dot  = dot_product(p1,p2);

    result.x = p1.x * cos(radian_angle) + (1-cos(radian_angle)) * dot * p2.x + sin(radian_angle) * k.x;
    result.y = p1.y * cos(radian_angle) + (1-cos(radian_angle)) * dot * p2.y + sin(radian_angle) * k.y;
    result.z = p1.z * cos(radian_angle) + (1-cos(radian_angle)) * dot * p2.z + sin(radian_angle) * k.z;

    return result;
}

Array2D Rotation(point p, double radian_angle)
{
    Array2D matrix = generate_identity_matrix();

    point normalized = normalization(p);
    point x = {1,0,0};
    point y = {0,1,0};
    point z = {0,0,1};
    point c1 = Rotation_rodriguez(x,normalized,radian_angle);
    point c2 = Rotation_rodriguez(y,normalized,radian_angle);
    point c3 = Rotation_rodriguez(z,normalized,radian_angle);

    matrix.arr_matrix[0][0] = c1.x;
    matrix.arr_matrix[1][0] = c1.y;
    matrix.arr_matrix[2][0] = c1.z;

    matrix.arr_matrix[0][1] = c2.x;
    matrix.arr_matrix[1][1] = c2.y;
    matrix.arr_matrix[2][1] = c2.z;

    matrix.arr_matrix[0][2] = c3.x;
    matrix.arr_matrix[1][2] = c3.y;
    matrix.arr_matrix[2][2] = c3.z;

    return matrix;

}

point Transformation(point p, Array2D matrix)
{
    point TransformedPoint;

    double w           =  matrix.arr_matrix[3][0] * p.x + matrix.arr_matrix[3][1] * p.y + matrix.arr_matrix[3][2] * p.z + matrix.arr_matrix[3][3] * 1;
    TransformedPoint.x = (matrix.arr_matrix[0][0] * p.x + matrix.arr_matrix[0][1] * p.y + matrix.arr_matrix[0][2] * p.z + matrix.arr_matrix[0][3] * 1)/w;
    TransformedPoint.y = (matrix.arr_matrix[1][0] * p.x + matrix.arr_matrix[1][1] * p.y + matrix.arr_matrix[1][2] * p.z + matrix.arr_matrix[1][3] * 1)/w;
    TransformedPoint.z = (matrix.arr_matrix[2][0] * p.x + matrix.arr_matrix[2][1] * p.y + matrix.arr_matrix[2][2] * p.z + matrix.arr_matrix[2][3] * 1)/w;
    
    return TransformedPoint;
}
