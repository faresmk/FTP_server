/*
 * echo - read and echo text lines until client closes connection
 */
#include "csapp.h"

#define CMD_GET "get" // get file
#define CMD_PUT "put" // put file

void ftp(int connfd) {
    //size_t n;
    char filename[MAXLINE];
    char buf[170000];
    int fd;
    rio_t rio, rio2;
    int file_size;


    Rio_readinitb(&rio, connfd);
   
    Rio_readlineb(&rio, filename, MAXLINE);
    
    if (filename[strlen(filename)-1] == '\n') {
        filename[strlen(filename)-1] = '\0';
    }
    //add Server/ to the file name
    char *file = malloc(strlen(filename) + 8);
    strcpy(file, "Server/");
    strcat(file, filename);
  





    fd = open(file, O_RDONLY);

    if(fd == -1) {
        //print in the client side
        sprintf(buf, "File not found");
        Rio_writen(connfd, buf, strlen(buf));
    }else if(fd != -1) { // file exists
        file_size = lseek(fd, 0, SEEK_END);
        lseek(fd, 0, SEEK_SET);
        sprintf(buf, "%d", file_size); // convert the file size to string
        Rio_readinitb( &rio2, fd);
        Rio_readnb(&rio2, buf, file_size);
        Rio_writen(connfd, buf, file_size); // send the file size
        }
        close(fd);
        
}
