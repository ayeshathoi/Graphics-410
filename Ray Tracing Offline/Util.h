//camera 
int draw_axes = 1;
// press a or mouse for removing axes


Point camera_pos, l, r, u;

// projection 
double nearplane_distance, farplane_distance, fovY, aspectRatio;

//recursion
int recursion_level, number_of_pixels_both_axes;

// checkerboard
double checkerboard_width;

//number of objects
int number_of_objects;
string object_type;
//sphere
double sphere_radius;

//pyramid
double lowest_x, lowest_y, lowest_z, pyramid_height, pyramid_width;

//cube
double bottom_lower_left_x, bottom_lower_left_y, bottom_lower_left_z, cube_side;


// normal light
int number_of_normal_lights;
// spot light
int number_of_spot_lights;



