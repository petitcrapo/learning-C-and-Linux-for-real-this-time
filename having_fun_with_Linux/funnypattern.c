#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>

int main(void) {
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
    int mask = 1;
    for (int y = 0; y < y_resolution; y++) {
        for (int x = 0; x < x_resolution; x++) {
            if (x & mask) {
                buf[y * x_resolution + x] = 0x00ffffff;
            }
        }
        mask = mask<<1;
    }
    
    write(fbfd, buf, 4 * (x_resolution * y_resolution));
    sleep(120);
    ioctl(ttyfd, KDSETMODE, KD_TEXT);
    close(ttyfd);
    close(fbfd);
    return 0;
}