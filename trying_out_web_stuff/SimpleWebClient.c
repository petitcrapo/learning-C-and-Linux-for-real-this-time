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
    char buf[2048];
    if (connect(tcp_socketfd,(struct sockaddr*) &addr, sizeof(addr))) {
        perror("Connect");
        return -1;
    }
    sleep(1);
    int recieved = recv(tcp_socketfd, &buf, sizeof(buf), 0);
    write(1, buf, recieved);
    close(tcp_socketfd);
    return 0;
}