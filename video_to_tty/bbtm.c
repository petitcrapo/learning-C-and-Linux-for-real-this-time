#include "bbtm.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct bbtm_header {
    unsigned int xres;
    unsigned int yres;
    unsigned int fps;
} bbtm_header_s;

// TODO CREATE FILE FUNC
// TODO FREE BBTM HEADER STRUCT
// TODO FREE IMAGE_LISTE_S

bbtm_header_p read_header(int fd) {
    unsigned int buf[3];
    if (read(fd, buf, sizeof(bbtm_header_s)) == -1) {
        return NULL;
    }
    lseek(fd, 0, SEEK_SET);
    bbtm_header_p header = malloc(sizeof(bbtm_header_s));
    header->xres = buf[0];
    header->yres = buf[1];
    header->fps = buf[2];
    return header;
}

bbtm_header_p create_header(unsigned int xres, unsigned int yres, unsigned int fps) {
    bbtm_header_p bbtm_header_p = malloc(sizeof(bbtm_header_s));
    bbtm_header_p->xres = xres;
    bbtm_header_p->yres = yres;
    bbtm_header_p->fps = fps;
    return bbtm_header_p;
}

int write_header_to_file(int fd, bbtm_header_p header) {
    lseek(fd, 0, SEEK_SET);
    unsigned int buf[3];
    buf[0] = header->xres;
    buf[1] = header->yres;
    buf[2] = header->fps;
    int rwrite = write(fd, buf, sizeof(bbtm_header_s));
    if (rwrite == -1) {
        return -1;
    }
    return rwrite;
}

int get_x(bbtm_header_p bbtm_header_p) {
    return bbtm_header_p->xres;
}

int get_y(bbtm_header_p bbtm_header_p) {
    return bbtm_header_p->yres;
}

image_liste_p create_image_liste() {
    image_liste_p image_liste_p = NULL;
    return image_liste_p;
}

int write_data_to_file(int fd, bbtm_header_p header, image_liste_p image_liste_p) {
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, sizeof(bbtm_header_s), SEEK_SET);
    int rwrite = write(fd, image_liste_p, size - (sizeof(bbtm_header_s)));
    if (rwrite == -1) {
        perror("write");
    }
    return rwrite;
}

int read_image(int fd, bbtm_header_p header, int* image_p, int image_index) {
    lseek(fd, sizeof(bbtm_header_s) + image_index * sizeof(unsigned int) * (header->xres * header->yres), SEEK_SET);
    int rread = read(fd, image_p, sizeof(unsigned int) * (header->xres * header->yres));
    if (rread == -1) {
        perror("read");
    }
    return rread;
}
