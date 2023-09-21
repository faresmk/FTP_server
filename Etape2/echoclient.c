/*

echoclient.c - An echo client
*/
#include "csapp.h"
#define BLOCKSIZE 16284
int main(int argc, char **argv)
{
int clientfd, port;
char *host, filename[BLOCKSIZE], path[BLOCKSIZE+8], filesize_str[20] ;
rio_t rio;
int fd;
int block;
clock_t start, end;

host = "localhost";
port = 2204;
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

Rio_readinitb(&rio, clientfd);

    printf("ftp>");
while (Fgets(filename, MAXLINE, stdin) != NULL) {
    //send the filename to the server
    Rio_writen(clientfd, filename, strlen(filename));
    //initiate filesize_str
    memset(filesize_str, 0, 20);
    //get the file size
    Rio_readnb(&rio, filesize_str, BLOCKSIZE);
    printf("filesize: %s\n", filesize_str);
    //filesize_str == bye means the server is going to disconnect
    if (strcmp(filesize_str, "bye") == 0) {
        printf("server disconnected\n");
        break;
    }
    //file not found case
    if (strcmp(filesize_str, "File not found") == 0) {
        printf("File not found\n");
        printf("ftp>");
        continue;
    }
    //put the file receiver to the path Client directory
    sprintf(path, "Client/%s", filename);
    path[strlen(path)-1] = '\0';
    //open the file
    fd = Open(path, O_WRONLY | O_CREAT , S_IRUSR | S_IWUSR);
    if (fd < 0) {
        printf("creating err\n");
    }

    int file_size = atoi(filesize_str); //convert the filesize string to int
    //receive the file
    size_t total_read = 0, remaining, to_read;
    start = clock();
    while (total_read < file_size) { //read until the file is completely received
        remaining = file_size - total_read;
        to_read = remaining > BLOCKSIZE ? BLOCKSIZE : remaining;
        block = Rio_readnb(&rio, filename, to_read);
        total_read += block;
        Rio_writen(fd, filename, to_read);
    }
    end = clock();
    printf("Transfer successfully complete\n");
    printf("%u bytes received in %f seconds\n", (unsigned int)file_size, (double)(end-start)/CLOCKS_PER_SEC);

    close(fd);
    printf("ftp>");
}
Close(clientfd);
exit(0);
}