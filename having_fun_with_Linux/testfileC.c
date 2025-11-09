#include <fcntl.h>
#include <unistd.h>

int main (void){
    int file = open("/home/moi/CODE/Cprojects/hello.txt", O_RDWR);
    char buf[2048];
    int size = read(file, buf, 2048);
    write(1, buf, size);
    close(file);
    return 0;
}
