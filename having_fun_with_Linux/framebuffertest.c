#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>


double aire_tri(int tri[3][2]) {
    double aire = 0.5 * abs(tri[0][0] * (tri[1][1] - tri[2][1]) + tri[1][0] * (tri[2][1] - tri[0][1]) + tri[2][0] * (tri[0][1] - tri[1][1]));
    return aire;
}

int* rast_triangle(int x_resolution, int y_resolution, int *buf, int tri[3][2], int color) {
    int triTemp[3][2];
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
                buf[(y * x_resolution) + x] = color;
            }
        }
    }
    return buf;
}

int main() {
    const int x_resolution = 1920;
    const int y_resolution = 1080;
    int tri0[3][2] = {{0, 0}, {10, 0}, {0, 10}};
    int buf[x_resolution * y_resolution];
    for (int i = 0; i <(x_resolution * y_resolution); i++) {
        buf[i] = 0;
    }
    int fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("open fb0");
        return -1;
    }
    int ttyfd = open("/dev/tty3", O_RDWR);
    if (ttyfd == -1) {
        perror("open tty3");
        return -1;
    }
    ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    for (int j = 0; j < 192; j++) {
        rast_triangle(x_resolution, y_resolution, buf, tri0, 0x00f38ba8);
        for (int k = 0; k < 3; k++) {
            tri0[k][0] = tri0[k][0] + 10;
        }
    }
    write(fbfd, buf, 4 * (x_resolution * y_resolution));
    sleep(5);
    ioctl(ttyfd, KDSETMODE, KD_TEXT);
    close(ttyfd);
    close(fbfd);
    return 0;
}
