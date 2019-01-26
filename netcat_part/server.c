
#include "nc.h"

void createServer(nc_args_t *nc_args) {
    
    int serverSockfd, newSocketfd;
    int listenStatus;
    unsigned char buffer[53]; // BUF_LEN = 1024
    ssize_t bytesWritten = 0;
    FILE *fp;

    struct sockaddr_in clientAddr;
    unsigned int clientAddrLength = sizeof(clientAddr);

    memset(buffer, 0, 53); // zero it out

    // Create Socket
    serverSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Bind address to socket
    Bind(serverSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));
    // Assert socket as listener
    listenStatus = Listen(serverSockfd, MAXQUEUE);
    // set up a client socket
    newSocketfd = Accept(serverSockfd, (struct sockaddr *) &clientAddr, &clientAddrLength);
	
    
    // Open file 
	fp = fopen(nc_args->serverFilename, "r");
	if (fp != NULL) {
        // check for offset
        if(nc_args->offset != 0){
            fseek(fp,nc_args->offset,SEEK_SET);
        }
        while(fread(buffer, sizeof(BUF_LEN), 1, fp)) {
            bytesWritten += Write(newSocketfd, buffer, BUF_LEN);
        }
	}else {
		err_sys("ERROR: File could not be opened");
	}
	if (bytesWritten < 0 || bytesWritten == 0) {
		perror("ERROR: Nothing written to file");
    }    

	// Reset file pointer
	fseek(fp, 0, SEEK_SET);
    // Close file and sockets
    fclose(fp);
    close(newSocketfd);
    close(serverSockfd);

    return;
}