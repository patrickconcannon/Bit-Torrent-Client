
#include "nc.h"


void createClient(nc_args_t *nc_args) {			
    
    int clientSockfd=0, connfd=0;
    unsigned char buffer[BUF_LEN];
    unsigned char buffer1[32] = "Hello, world";
    ssize_t bytesRead, totalBytesRead = 0;
    FILE *fp;

    bzero(buffer, BUF_LEN);

    // Open socket
    clientSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Connect
    Connect(clientSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));

    Write(connfd, buffer1, 32);
    // Open file 
    fp = fopen(nc_args->clientFilename, "w+");
	
    // read data from client and write to file
    while((bytesRead = read(clientSockfd, buffer, BUF_LEN)) != 0 ) {
        totalBytesRead += bytesRead;
        fwrite(buffer, BUF_LEN, 1, fp); 
        bzero(buffer, BUF_LEN);
    }
    printf("Client: %ld bytes written to file '%s'\n", totalBytesRead, nc_args->clientFilename);

    // Reset file pointer
    fseek(fp, 0, SEEK_SET);
    totalBytesRead = 0;
    // Close file and socket
    fclose(fp);
    close(clientSockfd);
    
    return;
}
