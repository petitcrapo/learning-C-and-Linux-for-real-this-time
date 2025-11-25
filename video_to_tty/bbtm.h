#ifndef BBTM_H
#define BBTM_H

#include <sys/types.h>


typedef struct bbtm_header bbtm_header_s;
typedef bbtm_header_s *bbtm_header_p;

typedef int* image_liste_p;


bbtm_header_p create_header(unsigned int xres, unsigned int yres, unsigned int fps);
bbtm_header_p read_header(int fd);
int write_header_to_file(int fd, bbtm_header_p header);
int get_x(bbtm_header_p);
int get_y(bbtm_header_p);
image_liste_p create_image_liste();
int write_data_to_file(int fd, bbtm_header_p header, image_liste_p bitmaps);
int read_image(int fd, bbtm_header_p header, int* image_p, int image_index);

#endif