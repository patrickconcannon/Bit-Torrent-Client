
#include "nc.h"

void createServer(nc_args_t *nc_args) {
    
    int listenfd, connfd;
    int listenStatus;
    unsigned char buffer[BUF_LEN]; // BUF_LEN = 1024
    ssize_t bytesRead, totalBytesRead = 0;
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

    // Open file 
    fp = fopen(nc_args->serverFilename, "w+");

    while(1){
        // accept incoming connections
        connfd = Accept(listenfd, (struct sockaddr *) &clientAddr, &clientAddrLength);
        
        // read data from client and write to file
        while((bytesRead = read(connfd, buffer, BUF_LEN)) != 0 ) {	
            totalBytesRead += bytesRead;
            fwrite(buffer, sizeof(BUF_LEN), 1, fp);
            bzero(buffer, BUF_LEN);
        }
        printf("Server: %ld bytes written to file '%s'\n", totalBytesRead, nc_args->serverFilename);

        // Reset file pointer
	    fseek(fp, 0, SEEK_SET);
        totalBytesRead = 0;
	    // Close file and socket
        fclose(fp);
        close(connfd);
    }
    return;
}