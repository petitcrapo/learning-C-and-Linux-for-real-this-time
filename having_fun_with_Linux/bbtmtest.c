#include "bbtm.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void) {
    int bbtmfd = open("/home/moi/CODE/Cprojects/learning-C-and-Linux-for-real-this-time/having_fun_with_Linux/test.bbtm", O_RDWR | O_CREAT);
    if (bbtmfd == -1) {
        perror("open");
        return -1;
    }
    bbtm_header_p bbtm_header_p = create_header(1920, 1080, 30);
    image_liste_p image_liste_p = create_image_liste(bbtm_header_p);
    if (write_header_to_file(bbtmfd, bbtm_header_p) == -1) {
        perror("write_header_to_file");
        return -1;
    }
    int buf[1920 * 1080];
    for (int i = 0; i < 1920 * 1080; i++) {
        buf[i] = 0;
        printf(":3");
    }
    for (int i = 0; i < 1920 * 10; i++) {
        buf[i] = 0x00ffffff;
        if (lseek(bbtmfd, 0, SEEK_END) == -1) {
            perror("lseek");
            return -1;
        }
        if (write(bbtmfd, buf, sizeof(buf)) == -1) {
            perror("write bbtm");
            return -1;
        }
        printf("%d\n", i);
    }
    return 0;
}