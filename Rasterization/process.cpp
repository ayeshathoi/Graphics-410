#include "Util.h"
#include "bitmap_image.hpp"

// -------------------------------------------------

//Global Variable
// view model
point eye, look, up, field_of_view;
point l, r, u;

// project model
double aspect_ratio, near, far , tP , rP , diff;
//matrix for view, projection and model transform
Array2D viewMatrix           = generate_identity_matrix();
Array2D projectionMatrix     = generate_identity_matrix();
Array2D modelTransformMatrix = generate_identity_matrix();

//model transform stack
stack<Array2D> modelTransformStack;
stack<stack<Array2D>> states;

//zBuffer and frameBuffer
double **zbuffer;
color **frameBuffer;
int triangleCount = 0;
triangle *triangleList;

int screenwidth,screenheight;

// -------------------------------------------------
//scna clipping variable
double left_limit_x , Bottom_limit_Y , min_z , max_z;
double dx, dy, TOP_Y, BOTTOM_Y, LEFT_X, RIGHT_X;
// -------------------------------------------------

//view Transformation - stage 2
void viewTransformation()
{
    Array2D transformMatrix = generate_identity_matrix(); 

    transformMatrix.arr_matrix[0][3] = - eye.x;
    transformMatrix.arr_matrix[1][3] = - eye.y;
    transformMatrix.arr_matrix[2][3] = - eye.z;

    //Apply the following rotation R such that the l aligns with the -Z axis, r with X axis, and u with Y axis.
    Array2D rotationMatrix = generate_identity_matrix();

    rotationMatrix.arr_matrix[0][0] = r.x;
    rotationMatrix.arr_matrix[0][1] = r.y;
    rotationMatrix.arr_matrix[0][2] = r.z;

    rotationMatrix.arr_matrix[1][0] = u.x;
    rotationMatrix.arr_matrix[1][1] = u.y;
    rotationMatrix.arr_matrix[1][2] = u.z;

    rotationMatrix.arr_matrix[2][0] = -l.x;
    rotationMatrix.arr_matrix[2][1] = -l.y;
    rotationMatrix.arr_matrix[2][2] = -l.z; 

    // V = RT
    viewMatrix = matrix_multiplication(rotationMatrix, transformMatrix);
}

// -------------------------------------------------

//projection Transformation - stage 3
void projectionTransform()
{
    projectionMatrix.arr_matrix[0][0] = near / rP;
    projectionMatrix.arr_matrix[1][1] = near / tP;
    projectionMatrix.arr_matrix[2][2] = -(far + near) / diff;
    projectionMatrix.arr_matrix[2][3] = -(2 * far * near) / diff;
    projectionMatrix.arr_matrix[3][2] = -1;
    projectionMatrix.arr_matrix[3][3] = 0;
}

// -------------------------------------------------

//model Transformation - stage1
void modelTransform(ifstream &scene,int test)
{
    string s1 = "./test_case/" + to_string(test)+ string("/1805062_stage1.txt");
    string s2 = "./test_case/" + to_string(test)+ string("/1805062_stage2.txt");
    string s3 = "./test_case/" + to_string(test)+ string("/1805062_stage3.txt");
    ofstream stage1(s1);
    ofstream stage2(s2);     
    ofstream stage3(s3);    
    modelTransformStack.push(modelTransformMatrix);
    string sceneLines;
    int Triangle_point_Number = 3;

    while (scene >> sceneLines)
    {
        if (sceneLines == "triangle")
            {

                for(int i = 0; i < Triangle_point_Number ; i++)
                {
                    point x;
                    scene >> x.x >> x.y >> x.z;

                    point transpoint = Transformation(x, modelTransformStack.top());
                    point viewPoint  = Transformation(transpoint, viewMatrix);
                    point projectionPoint = Transformation(viewPoint, projectionMatrix);

                    stage1 << fixed << setprecision(7);
                    stage2 << fixed << setprecision(7);
                    stage3 << fixed << setprecision(7);
                   
                    stage1 << transpoint.x      << " " << transpoint.y      << " " << transpoint.z      << endl;
                    stage2 << viewPoint.x       << " " << viewPoint.y       << " " << viewPoint.z       << endl;
                    stage3 << projectionPoint.x << " " << projectionPoint.y << " " << projectionPoint.z << endl;
                }

                //--------------
                triangleCount++;
                //--------------
                
                stage1 << endl;
                stage2 << endl;
                stage3 << endl;
        }

        else if (sceneLines == "push")
        {
            stack<Array2D>top = modelTransformStack;
            states.push(top);
        }
            
        else if (sceneLines == "pop")
        {
            modelTransformStack = states.top();
            states.pop();
        }

        else if (sceneLines == "rotate")
        {
            double angle;
            point point;
            scene >> angle >> point.x >> point.y >> point.z;
            double radian_angle = angle * radian * -1;
            Array2D rotationMatrix  = Rotation(point,radian_angle);
            Array2D top             = matrix_multiplication(modelTransformStack.top(), rotationMatrix);

            modelTransformStack.push(top);
        }

        else if (sceneLines == "scale")
        {
            point scale;
            scene >> scale.x >> scale.y >> scale.z;

            Array2D scaleMatrix             = generate_identity_matrix();
            scaleMatrix.arr_matrix[0][0]    = scale.x;
            scaleMatrix.arr_matrix[1][1]    = scale.y;
            scaleMatrix.arr_matrix[2][2]    = scale.z;

            Array2D top                     = matrix_multiplication(modelTransformStack.top(), scaleMatrix);
            modelTransformStack.push(top);
        }

        else if (sceneLines == "translate")
        {
            point translate;
            scene >> translate.x >> translate.y >> translate.z;

            Array2D translateMatrix             = generate_identity_matrix();
            translateMatrix.arr_matrix[0][3]    = translate.x;
            translateMatrix.arr_matrix[1][3]    = translate.y;
            translateMatrix.arr_matrix[2][3]    = translate.z;

            Array2D top                         = matrix_multiplication(modelTransformStack.top(), translateMatrix);
            modelTransformStack.push(top);
        }

        else if (sceneLines == "end")
            break;
    }
    stage1.close();
    stage2.close();
    stage3.close();   
}

// -------------------------------------------------


void BMPConfig(int test)
{   
    string triangle_file = "./test_case/" + to_string(test)+ string("/1805062_stage3.txt");
    ifstream triangleData(triangle_file);
    
    //----------------------------------------------


    // initialize zBuffer and frameBuffer 
    //---------------------------------------------
    zbuffer     = new double*[screenwidth];
    frameBuffer = new color*[screenwidth];

    for(int i = 0; i < screenwidth; i++){
            zbuffer[i]      = new double[screenheight];
            frameBuffer[i]  = new color[screenheight];
            for(int j = 0; j < screenwidth ; j++)
            {
                // set color to black
                zbuffer[i][j] = max_z;
                frameBuffer[i][j].r = 0;
                frameBuffer[i][j].b = 0;
                frameBuffer[i][j].g = 0;                
            }
    }
    //---------------------------------------------


    for(int i = 0; i<triangleCount;i++)
    {
        triangle cur_triangle;
        for(int j = 0; j < 3; j++)
            triangleData >> cur_triangle.p[j].x >> cur_triangle.p[j].y >> cur_triangle.p[j].z;;

        cur_triangle.clr.r = randomColor();
        cur_triangle.clr.g = randomColor();
        cur_triangle.clr.b = randomColor();

        int topScanLine = 0;
        int bottomScanLine = screenheight - 1;
        int leftScanLine = 0;
        int rightScanLine  = screenwidth  - 1;

        double MaxY, MinY;
        int maxIndex = -1;
        int minIndex = -1;

        double MAX_X_TRIANGLE = -INFINITY;
        double MIN_X_TRIANGLE = INFINITY;

        MaxY = MinY = cur_triangle.p[0].y;
        
        // find out the max Y and min Y co-ordinate to find topscan and bottomscan
        for(int j = 0 ; j < 3 ; j++ )
        {
            if(cur_triangle.p[j].y > MaxY)
                MaxY = cur_triangle.p[j].y;
            if(cur_triangle.p[j].y < MinY)
                    MinY = cur_triangle.p[j].y;  
        }

        
        //  clipping topScanLine and bottomScanLine 
        if(MinY < BOTTOM_Y)
            topScanLine = round((TOP_Y - MaxY) / dy);
        
        if (MinY >= BOTTOM_Y)
            bottomScanLine -= round((MinY - BOTTOM_Y) / dy);
        
        for (int row = topScanLine; row <= bottomScanLine; row++)
        {
            // each row's y co-ordinate
            double row_y_mid = TOP_Y - row * dy;
            point intersect_point[3]; 
            
            for (int k = 0 ; k < 3; k++)
            {
                intersect_point[k] = {INFINITY, row_y_mid, INFINITY};

                point a = cur_triangle.p[(3-k)%3];
                point b = cur_triangle.p[(3-k-1) % 3];

                double diff_length_y = b.y - a.y;
                double diff_length_x = b.x - a.x;
                double diff_length_z = b.z - a.z;
                
                double maxX = max(a.x, b.x);
                double minX = min(a.x, b.x);
                double maxZ = max(a.z, b.z);
                double minZ = min(a.z, b.z);

                if (a.y != b.y)
                {
                    intersect_point[k].x = a.x + (row_y_mid - a.y) * (diff_length_x/diff_length_y);
                    intersect_point[k].z = a.z + (row_y_mid - a.y) * (diff_length_z/diff_length_y);

                    if (intersect_point[k].x != INFINITY && ((intersect_point[k].x > maxX) || (intersect_point[k].x < minX)))
                        intersect_point[k].x = INFINITY;
                }
               
                if (intersect_point[k].x < MIN_X_TRIANGLE) {
                    minIndex = k;
                    MIN_X_TRIANGLE = intersect_point[k].x;
                }

                if (intersect_point[k].x!=INFINITY && intersect_point[k].x > MAX_X_TRIANGLE) {
                    maxIndex = k;
                    MAX_X_TRIANGLE = intersect_point[k].x;
                }
                
            }


            //clipping leftScanLine and rightScanLine 
            if (MIN_X_TRIANGLE > LEFT_X)
                leftScanLine = round((MIN_X_TRIANGLE - LEFT_X)/dx);
              
            if (MAX_X_TRIANGLE < RIGHT_X )
                rightScanLine = screenwidth - 1 - round((RIGHT_X  - MAX_X_TRIANGLE)/dx);

            
            //Calculate z values
            double diff_x = intersect_point[maxIndex].x - intersect_point[minIndex].x;
            double diff_z = intersect_point[maxIndex].z - intersect_point[minIndex].z;
            double z_depth = 0;

            for (int col = leftScanLine; col <= rightScanLine; col++)
            {
                double mid_col_x = LEFT_X + col * dx;
                if (col == leftScanLine) 
                    z_depth = intersect_point[minIndex].z + 
                                (mid_col_x-intersect_point[minIndex].x)/diff_x*diff_z;
                else 
                    z_depth += dx * diff_z/diff_x;
                
                //Compare with z-buffer and z_front_limit = min_z
                if (z_depth > min_z && z_depth < zbuffer[row][col])
                {
                    zbuffer[row][col] = z_depth;
                    //update pixel info
                    frameBuffer[row][col] = cur_triangle.clr;
                }
            }
        }
    }
}

void freeMem()
{
    for(int row = 0; row < screenwidth; row++){
        delete [] zbuffer[row];
        delete [] frameBuffer[row];
    }

    delete[] zbuffer;
    delete[] frameBuffer;
}


void imageSave(int test)
{
    string z_filename = "./test_case/" + to_string(test) +string("/1805062_z_buffer.txt");
    ofstream zBuffertxt(z_filename);

    // constructor bitmap_image(width, height)
    bitmap_image image(screenwidth,screenheight);
    zBuffertxt << fixed << setprecision(6);

    for(int i = 0 ; i < screenwidth ; i++)
    {

        for(int j = 0 ; j < screenheight ; j++)
        {
        // Save the z_buffer values in “z_buffer.txt”. Save only those values which are less than z_max i.e.
        // for each row and col, z-buffer[row][col] < z_max.
            if(zbuffer[i][j] < max_z)
                zBuffertxt << zbuffer[i][j] << "\t";
            color c = frameBuffer[i][j];
            
            image.set_pixel(j,i,c.r,c.g,c.b);
        }
        zBuffertxt << endl;
    }
    string filename = "./test_case/" + to_string(test) +string("/1805062_out.bmp");
    //save the image
    image.save_image(filename);
    zBuffertxt.close();
}




void clipScan(int test)
{    
    BMPConfig(test);
    imageSave(test);
    freeMem(); 
}
