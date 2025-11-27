#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <signal.h>

int main(void) {
    shm_unlink("/sem");
    int semfd;
    if ((semfd = shm_open("/sem", O_CREAT | O_RDWR, 0644)) == -1) {
        perror("didn't open semaphore fd");
        return -1;
    }
    int flag = 1;
    pwrite(semfd, &flag, 1, 0);
    if (semfd == -1) {
        perror("didn't write to semaphore");
        return -1;
    }
    int tcp_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socketfd == -1) {
        perror("socket");
        return -1;
    }
    int pid;
    switch (pid = fork()) {
        case -1: {
            perror("couldn't fork");
            return -1;
        }
        
        case 0: {
            printf("i'm going to sleep");
            int semaphore;
            pread(semfd, &semaphore, 1, 0);
            if (semaphore == 1) {
                pause();
            }
            printf("i woke up");
            struct sockaddr_in cli_addr;
            socklen_t cli_adr_len = sizeof(cli_addr);
            int ns = accept(tcp_socketfd, (struct sockaddr *)&cli_addr, &cli_adr_len);
            if (ns == -1) {
                perror("didn't accept :< ");
                close(semfd);
                close(tcp_socketfd);
                exit(0);
            }
            // perform action
            exit(0);
        }
    }
    
    int port = 6761;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (bind(tcp_socketfd,(struct sockaddr*) &addr, sizeof(addr))) {
        perror("couldn't bind");
        return -1;
    }
    if (listen(tcp_socketfd, 10)) {
        perror("couldn't listen");
        return -1;
    }
    flag = 0;
    pwrite(semfd, &flag, 1, 0);
    kill(pid, SIGCONT);
    sleep(1);
    // fork when new connection
    return 0;
}
