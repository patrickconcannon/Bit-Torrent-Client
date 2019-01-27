
#include "nc.h"

void createServer(nc_args_t *nc_args) {
    
    int listenfd, connfd;
    int listenStatus;
    unsigned char buffer[BUF_LEN]; // BUF_LEN = 1024
    ssize_t bytesWritten = 0;
    FILE *fp;

    struct sockaddr_in clientAddr;
    unsigned int clientAddrLength = sizeof(clientAddr);

    bzero(buffer, BUF_LEN); // zero it out

    // Create Socket
    listenfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Bind address to socket
    Bind(listenfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));
    // Assert socket as listener
    listenStatus = Listen(listenfd, MAXQUEUE);

    while(1){
        // accept incoming connections
        connfd = Accept(listenfd, (struct sockaddr *) &clientAddr, &clientAddrLength);
        // Open file 
        fp = fopen(nc_args->serverFilename, "rb");
        if (fp != NULL) {
            // check for offset
            if(nc_args->offset > 0){
                fseek(fp, nc_args->offset, SEEK_SET);
            } 
            while(fread(buffer, sizeof(BUF_LEN), 1, fp)) {
                bytesWritten += Write(connfd, buffer, BUF_LEN);
            }
            printf("Server: %ld bytes sent to client\n", bytesWritten);
        }else {
            err_sys("ERROR: File could not be opened");
        }
        if (bytesWritten < 0 || bytesWritten == 0) {
            perror("ERROR: Nothing written to file");
        }    

        // Reset file pointer and bytesWritten
        fseek(fp, 0, SEEK_SET);
        bytesWritten = 0;
        // Close file and socket
        fclose(fp);
        close(connfd);
    }

    return;
}