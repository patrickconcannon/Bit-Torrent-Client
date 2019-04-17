
#include "nc.h"


/*
    So you will have to take into account what the offset and n_bytes is
    on the server end? 

    So when you try to send the file you set the appropriate offset, and send this
    to the server. On subsequent connections you will continue to download the file
    and keep going from the part you left off on....

    So will you overwrite? You will need to keep track of it properly. Until next time...
    Same Bat time, same Bat channel. 

*/

void createServer(nc_args_t *nc_args) {
    
    int clientSockfd=0, count=1;
    int listenfd, connfd;
    int listenStatus;
    unsigned char buffer[BUF_LEN]; // BUF_LEN = 1024
    unsigned char buffer1[32]; // BUF_LEN = 1024
    ssize_t bytesRead, totalBytesRead = 0;
    ssize_t bytesRead1 = 0;
    ssize_t bytesWritten = 0;
    FILE *fp;

    struct sockaddr_in clientAddr;
    unsigned int clientAddrLength = sizeof(clientAddr);

    bzero(buffer, BUF_LEN); // zero it out
    bzero(buffer1, BUF_LEN); // zero it out

    // Create Socket
    listenfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Bind address to socket
    Bind(listenfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));
    // Assert socket as listener
    listenStatus = Listen(listenfd, MAXQUEUE);

    // Open file 
    fp = fopen(nc_args->serverFilename, "rb");

    for(;;){

        // accept incoming connections
        connfd = Accept(listenfd, (struct sockaddr *) &clientAddr, &clientAddrLength);
        bytesRead1 = read(connfd, buffer1, BUF_LEN); // change to send a struct 
        printf("HERE: %s\n", buffer1);

        if (fp != NULL) {
            // check for offset
            if(nc_args->offset != 0){
                fseek(fp, nc_args->offset, SEEK_SET);
            } 
            if(nc_args->n_bytes != 0){ // check if n_bytes specified
                count=nc_args->n_bytes;
            }
            while(fread(buffer, BUF_LEN, count, fp)) { 
                bytesWritten += Write(connfd, buffer, BUF_LEN);
                if(nc_args->n_bytes){ //check for remaining bytes
                    count--;
                }
            }
            printf("Server: %ld bytes sent to client\n", bytesWritten);
        }else {
            err_sys("ERROR: File could not be opened");
        }
        
        if (bytesWritten < 0 || bytesWritten == 0) {
            perror("ERROR: Nothing written to file");
        }    
    
        // Reset file pointer
        fseek(fp, 0, SEEK_SET);
        // Close file and socket
        fclose(fp);
        Close(connfd);
    }
    return;
}
