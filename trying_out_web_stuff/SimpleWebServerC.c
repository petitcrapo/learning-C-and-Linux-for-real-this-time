#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int tcp_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socketfd == -1) {
        perror("Socket");
        return -1;
    }
    int port = 6761;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (bind(tcp_socketfd,(struct sockaddr*) &addr, sizeof(addr))) {
        perror("Bind");
        return -1;
    }
    if (listen(tcp_socketfd, 10)) {
        perror("listen");
        return -1;
    }
    struct sockaddr_in cli_addr;
    socklen_t cli_adr_len = sizeof(cli_addr);
    char buf[2048];
    int file = open("/home/moi/CODE/Cprojects/hello.txt", O_RDWR);
    int size = read(file, buf, 2048);
    while (1) {
        int ns = accept(tcp_socketfd, (struct sockaddr *)&cli_addr, &cli_adr_len);
        if (ns < 0) {
            perror("accept");
            close(tcp_socketfd);
            return -1;
        }
        send(tcp_socketfd, buf, size, 0);
    }
    return 0;
}