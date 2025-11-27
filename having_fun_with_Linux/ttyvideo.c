#include "bbtm.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <unistd.h>


// void write_to_screen(int fbfd, int xres, int yres, int xshift, int yshift,
// int* buf) {
//     int tmpbuf[1920 * 1080];
//     for (int y = 0; y < 1080; y++) {
//         for (int x = 0; x < 1920; x++) {
//             if (x>xshift && y + yshift < 1080) {
//                 tmpbuf[y * 1920 + x] = buf[y * xres + x - xshift];
//             } else {
//                 tmpbuf[y * 1920 + x] = 0;
//             }
//         }
//     }
//     write(fbfd, tmpbuf, sizeof(tmpbuf));
// }

int main(void) {
    int bbtmfd = open("/home/moi/CODE/Cprojects/learning-C-and-Linux-for-real-this-time/having_fun_with_Linux/test.bbtm", O_RDONLY);
    if (bbtmfd == -1) {
        perror("open");
        return -1;
    }
    int pipefd[2];
    if ((pipe(pipefd) == -1)) {
        perror("open pipe");
        return -1;
    }
    switch (fork()) {
    case -1: {
        perror("fork");
        return -1;
    }
    case 0: {
        close(pipefd[0]);
        bbtm_header_p header = read_header(bbtmfd);
        int* decodebuf = malloc(sizeof(unsigned int) * get_x(header) * get_y(header));
        int* decodelinebuf = malloc(sizeof(unsigned int) * get_x(header));
        int image_index = 0;
        int ret;
        int i = 0;
        while((ret = read_image(bbtmfd, header, decodebuf, i)) > 0) {
            for (int y = 0; y < get_y(header); y++) {
                for (int x = 0; x < get_x(header); x++) {
                    decodelinebuf[x] = decodebuf[y * get_x(header) + x];
                }
                if (write(pipefd[1], decodelinebuf, sizeof(unsigned int) * get_x(header)) == -1) {
                    perror("child wroten't");
                    return -1;
                }
                
            }
            i++;
        }
        if (ret == -1) {
            perror("read image");
        }
        close(pipefd[1]);
        exit(0);
    }
    
    }
    close(pipefd[1]);
    const int x_resolution = 1920;
    const int y_resolution = 1080;
    int buf[x_resolution * y_resolution];
    for (int i = 0; i < (x_resolution * y_resolution); i++) {
        buf[i] = 0;
    }
    int fb0fd = open("/dev/fb0", O_RDWR);
    if (fb0fd == -1) {
        perror("open fb0");
        return -1;
    }
    int ttyfd = open("/dev/tty3", O_RDWR);
    if (ttyfd == -1) {
        perror("open tty3");
        return -1;
    }
    // ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    bbtm_header_p header = read_header(bbtmfd);
    int ret;
    int y = 0;
    int* image_p = malloc(sizeof(unsigned int) * get_x(header) * get_y(header));
    int* image_line_p = malloc(sizeof(unsigned int) * get_x(header));
    while ((ret = read(pipefd[0], image_line_p, sizeof(image_line_p))) > 0) {
        usleep(16667);
        if (y < 10) {
            for (int x = 0; x < get_x(header); x++) {
                image_p[y * get_x(header) + x] = image_line_p[x];
            }
            y++;
        } else {
            if (write(fb0fd, image_p, sizeof(unsigned int) * (x_resolution * y_resolution)) == -1) {
                perror("frame buffer write");
            }
            y = 0;
            lseek(fb0fd, 0, SEEK_SET);
        }
    }
    // ioctl(ttyfd, KDSETMODE, KD_TEXT);
    close(ttyfd);
    close(pipefd[1]);
    close(fb0fd);
    return 0;
}
