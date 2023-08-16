#include "process.cpp"

void init(ifstream &scene, ifstream &config)
{    
    scene >> eye.x >> eye.y >> eye.z;
    scene >> look.x >> look.y >> look.z;
    scene >> up.x >> up.y >> up.z;
    scene >> field_of_view.y;
    scene >> aspect_ratio >> near >> far;

    //config file
    config >> screenwidth >> screenheight;


    //view Model
    l = {look.x - eye.x , look.y - eye.y , look.z - eye.z};

    l = normalization(l);
    r = normalization(crossProduct(l, up));
    u = crossProduct(r, l);


    //projectionModel
    field_of_view.x = field_of_view.y * aspect_ratio;
    // t and r from projection model
    tP = near * tan((field_of_view.y * M_PI) / 360);
    rP = near * tan((field_of_view.x * M_PI) / 360);
    diff = far - near;

    //scan clipping - stage 4
    //pixel mapping
    left_limit_x = -1;
    Bottom_limit_Y = -1;
    min_z = -1; 
    max_z = 1;
    dx = (-1.0 * left_limit_x * 2) / screenwidth;
    dy = (-1.0 * Bottom_limit_Y * 2) / screenheight;
    LEFT_X = left_limit_x +(dx /2.0);
    RIGHT_X  = -1.0 * LEFT_X;
    TOP_Y = -Bottom_limit_Y - (dy/ 2.0);
    BOTTOM_Y = -1.0 * TOP_Y;
    
}

int main(int argc, char* argv[])
{

    // store the "test_case" folder in the current directory
    // then run the program with the test case number as the argument
    
    int test = atoi(argv[1]);
    cout << "Test that you want to execute : " << test << endl;
    

    ifstream scene          ("./test_case/" + to_string(test) +string("/scene.txt"));
    ifstream config         ("./test_case/" + to_string(test) +string("/config.txt"));

    init(scene, config);
    viewTransformation();
    projectionTransform();
    modelTransform(scene,test);
    clipScan(test);
    cout << "Output files are in ./test_case/" << test << "/" << endl;
    
    // for(int test = 1 ; test <= 5 ; test++)
    // {            

    //     ifstream scene          ("./test_case/" + to_string(test) +string("/scene.txt"));
    //     ifstream config         ("./test_case/" + to_string(test) +string("/config.txt"));

    //     init(scene, config);
    //     viewTransformation();
    //     projectionTransform();
    //     modelTransform(scene,test);
    //     clipScan(test);
    // }
    
    return 0;
}
