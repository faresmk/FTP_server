/*
 * echoserveri.c - An iterative echo server
 */

#include "csapp.h"

#define MAX_NAME_LEN 256
#define NB_PROC 2
void ftp(int connfd);


void handler(int sig)
{
    while (waitpid(-1, 0, WNOHANG) > 0);
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
    port = 2204;
    
    Signal(SIGCHLD, handler);
    
    clientlen = (socklen_t)sizeof(clientaddr);

    listenfd = Open_listenfd(port);
    for (int i = 0; i < NB_PROC; i++) {
       if ((pid[i] = Fork()) == 0) {
        while (1) {
            while((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen))<0);

            /* determine the name of the client */
            Getnameinfo((SA *) &clientaddr, clientlen,
                        client_hostname, MAX_NAME_LEN, 0, 0, 0);
            
            /* determine the textual representation of the client's IP address */
            Inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip_string,
                      INET_ADDRSTRLEN);
            
            printf("server connected to %s (%s)\n", client_hostname,
                   client_ip_string);
            ftp(connfd);

            Close(connfd);
        }
       }
    }
  
    while (1);
    exit(0);
}

