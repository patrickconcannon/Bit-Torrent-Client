
#include "nc.h"

void createServer(nc_args_t *nc_args) {
    
    int serverSockfd, newSocketfd;
    int listenStatus;
    char buffer[BUF_LEN];
    ssize_t bytesRead, totalBytesRead = 0;
    FILE *fp;

    struct sockaddr_in clientAddr;
    unsigned int clientAddrLength = sizeof(clientAddr);

    memset(buffer, 0, BUF_LEN);

    // Create Socket
    serverSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Bind address to socket
    Bind(serverSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));
    // Assert socket as listener
    listenStatus = Listen(serverSockfd, MAXQUEUE);
    // server sets up a new socket to exchange data with client
    newSocketfd = Accept(serverSockfd, (struct sockaddr *) &clientAddr, &clientAddrLength);
	
    

    // Open file 
    fp = fopen(nc_args->serverFilename, "w+");
    
    while( (bytesRead = read(newSocketfd, buffer, BUF_LEN)) != 0 ) {	
	    totalBytesRead += bytesRead;
	    fwrite(buffer, sizeof(char), bytesRead, fp);
	    memset(buffer, 0, BUF_LEN);
    }
        
    printf("Server says: %ld bytes written to file '%s'\n", totalBytesRead, nc_args->serverFilename);
    
    // Reset file pointer
    rewind(fp);
    // Close file
    fclose(fp);
    
    // close the sockets
    close(newSocketfd);
    close(serverSockfd);

    return;
    
}