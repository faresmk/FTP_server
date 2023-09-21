/*

echo - read and echo text lines until client closes connection
*/
#include "csapp.h"
#define BLOCKSIZE 16284
void ftp(int connfd){

    size_t file_size;
    //size_t block; 
    size_t n;
    char filename[BLOCKSIZE];
    rio_t rio, rio2;
    int fd;    
    char filesize_str[20];

    //initiate the rio
    Rio_readinitb(&rio, connfd); 
  while (1) {
        
    //read the filename
        Rio_readlineb(&rio, filename, BLOCKSIZE);
        //remove the newline character
        if (filename[strlen(filename)-1] == '\n') {
            filename[strlen(filename)-1] = '\0';
        }

    //if file name is bye, disconnect
        if (strcmp(filename, "bye") == 0) {
            //send bye to client
            Rio_writen(connfd, filename, strlen(filename));
            printf("client disconnected\n");
            break;
        }


    //get the file from Server/ directory
        char *file = malloc(strlen(filename) + 8);
        strcpy(file, "Server/");
        strcat(file, filename);
        printf("filename: %s\n", file);

        //open the file
        fd = open(file, O_RDONLY, 0);
        if (fd < 0) {
            //send file not found to client
            //put File not found in the filename
            strcpy(filename, "File not found");
            Rio_writen(connfd, filename, strlen(filename));
            printf("File not found\n");
            free(file);
            close(fd);
            break;

            }

    //get the file size
        file_size = lseek(fd, 0, SEEK_END);
        //make filesize string
        sprintf(filesize_str, "%ld", file_size);
        //send the file size
        Rio_writen(connfd, &filesize_str, BLOCKSIZE);
        //reset the file pointer
        lseek(fd, 0, SEEK_SET);
        printf("file size: %ld\n", file_size);
        Rio_readinitb(&rio2, fd);
        //send the file
        int i = 0;
        while ((n = Rio_readnb(&rio2, filename, BLOCKSIZE)) > 0 && i < file_size ) {
            printf("server sent %u bytes\n", (unsigned int)n);
            Rio_writen(connfd, filename, n);
            i += n;
        } 
        Close(fd);   
        free(file);
        printf("file sent\n");
    }
}