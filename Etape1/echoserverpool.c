/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
#define NB_PROC 2


void ftp(int connfd);



#define CMD_GET "get" // get file
#define CMD_PUT "put" // put file

void handler(int sig)
{
    while (waitpid(-1, NULL, WNOHANG) > 0);
    return;
}
/* 
 * Note that this code only works with IPv4 addresses
 * (IPv6 is not supported)
 */
int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    char client_ip_string[INET_ADDRSTRLEN];
    char client_hostname[MAX_NAME_LEN];
    pid_t pid[NB_PROC];
    port = 2222;
    
    Signal(SIGCHLD, handler);

    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(port);

    for (int i = 0; i < NB_PROC; i++) {
        if ((pid[i] = Fork()) == 0) { // Child process
            while (1) {
                while((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen))<0);

                Getnameinfo((SA *)&clientaddr, clientlen,
                            client_hostname, MAX_NAME_LEN, 0, 0, 0);

                Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                          INET_ADDRSTRLEN);

                printf("server connected to %s (%s)\n", client_hostname,
                       client_ip_string);

                ftp(connfd);
                Close(connfd);
                
            }
            exit(0);
        }
    }
    while (1) {
        sleep(1);
    }

    return 0;
}





