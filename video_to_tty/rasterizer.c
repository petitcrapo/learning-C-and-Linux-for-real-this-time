#include "rasterizer.h"
#include <stdlib.h>
#include <math.h>

typedef struct circle {
    int res;
    triangle_p triangles;
} circle_s;

double aire_tri(triangle tri) {
    double aire = 0.5 * fabs(tri[0][0] * (tri[1][1] - tri[2][1]) + tri[1][0] * (tri[2][1] - tri[0][1]) + tri[2][0] * (tri[0][1] - tri[1][1]));
    return aire;
}

void rast_triangle(int x_res, int y_res, image_p image_ptr, triangle tri, int color) {
    triangle triTemp;
    int x_min,x_max = tri[0][0];
    int y_min,y_max = tri[0][1];
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++) {
            triTemp[i][j] = tri[i][j];
            if (j == 0) {
                if (tri[i][j] > x_max) {
                    x_max = tri[i][j];
                }
                if (tri[i][j] < x_min) {
                    x_min = tri[i][j];
                }
            } else {
                if (tri[i][j] > y_max) {
                    y_max = tri[i][j];
                }
                if (tri[i][j] < y_min) {
                    y_min = tri[i][j];
                }
            }
        }
    }
    for (int y = y_min; y < y_max; y++) {
        for (int x = x_min; x < x_max; x++) {
            double aire1 = aire_tri(tri);
            double aire2 = 0;
            for (int i = 0; i < 3; i++) {
                triTemp[i][0] = x;
                triTemp[i][1] = y;
                aire2 = aire2 + aire_tri(triTemp);
                triTemp[i][0] = tri[i][0];
                triTemp[i][1] = tri[i][1];
            }
            if (aire1 >= aire2) {
                *image_ptr[(y * x_res) + x] = color;
            }
        }
    }
}

circle_p create_circle(double radius, int res) {
    double TAUbyRes = (2 * 3.141592653589793238462643383279502884197169399375105820974944592307816406286) / res;
    circle_p circle = malloc(sizeof(circle_s));
    circle->res = res;
    for (int i = 0; i < res; i++) {
        for (int j = 0; j < 3; j++) {
            switch (j) {
                case 0: {
                    circle->triangles[i][0][0] = 0;
                    circle->triangles[i][0][1] = 0;
                }
                case 1: {
                    circle->triangles[i][1][0] = cos(i * TAUbyRes);
                    circle->triangles[i][1][1] = sin(i * TAUbyRes);
                }
                case 2: {
                    circle->triangles[i][2][0] = cos((i+1) * TAUbyRes);
                    circle->triangles[i][2][1] = sin((i+1) * TAUbyRes);
                }
            }
        }
    }
    return circle;
}

void rast_circle(int x_res, int y_res, image_p image_ptr, circle_p circle, int color) {
    for (int i = 0; i < circle->res; i++) {
        rast_triangle(x_res, y_res, image_ptr, circle->triangles[i], color);
    }
}