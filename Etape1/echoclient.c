/*
 * getclient.c - An echo client
 */
#include "csapp.h"

int main(int argc, char **argv)
{
    int clientfd, port;
    char *host, buf[170000];
    rio_t rio;
    //FILE *fp;
    //file size
    //ssize_t n;

    
    host = "localhost";
    port = 2222;

    /*
     * Note that the 'host' can be a name or an IP address.
     * If necessary, Open_clientfd will perform the name resolution
     * to obtain the IP address.
     */
    clientfd = Open_clientfd(host, port);
    
    /*
     * At this stage, the connection is established between the client
     * and the server OS ... but it is possible that the server application
     * has not yet called "Accept" for this connection
     */
    printf("client connected to server OS\n"); 
    
    Fgets(buf, 170000, stdin);
    Rio_writen(clientfd, buf, strlen(buf)); // send the file name
    
        int fd;
    fd = open("Client/pdf.pdf", O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR, 0);
    Rio_readinitb(&rio, clientfd);

    if(fd != -1) {
        
        int n = Rio_readnb(&rio, buf, 170000);
        write(fd, buf, n);
        close(fd);
        printf("Transfer complete\n");
        printf("%d bytes received \n", n);
    }
    Close(clientfd);
    exit(0);
}
