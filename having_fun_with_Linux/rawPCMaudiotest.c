#include <fcntl.h>
#include <stdio.h>

int main(void) {
    int audiocardfd = open("/dev/snd/pcmC0D0p",O_RDWR);
    if (audiocardfd == -1) {
        perror("audiocardfd");
        return -1;
    }
    
}