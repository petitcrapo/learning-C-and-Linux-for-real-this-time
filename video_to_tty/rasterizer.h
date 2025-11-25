#ifndef RASTERIZER_H
#define RASTERIZER_H

typedef double** triangle;
typedef triangle* triangle_p;
typedef struct circle circle_s;
typedef circle_s* circle_p;
typedef int* image;
typedef image* image_p;

double aire_tri(triangle triangle);
void rast_triangle(int x_res, int y_res, image_p image_ptr, triangle tri, int color);
circle_p create_circle(double radius, int res);
void rast_circle(int x_res, int y_res, image_p image_ptr, circle_p circle, int color);


#endif