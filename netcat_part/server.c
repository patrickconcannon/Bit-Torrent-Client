
#include "nc.h"

void createServer(nc_args_t *nc_args) {
    
    int serverSockfd, newSocketfd;
    int listenStatus;
    char input[BUF_LEN];
    ssize_t bytesWritten = 0;
    FILE *fp;

    struct sockaddr_in clientAddr;
    unsigned int clientAddrLength = sizeof(clientAddr);

    memset(input, 0, BUF_LEN);

    // Create Socket
    serverSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Bind address to socket
    Bind(serverSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));
    // Assert socket as listener
    listenStatus = Listen(serverSockfd, MAXQUEUE);
    // server sets up a new socket to exchange data with client
    newSocketfd = Accept(serverSockfd, (struct sockaddr *) &clientAddr, &clientAddrLength);
	
    
    // Open file 
	fp = fopen(nc_args->serverFilename, "r");
	if (fp != NULL) {
        if(nc_args->offset != 0){
            fseek(fp,sizeof(char)+nc_args->offset,SEEK_SET);
        }
			while(fread(input, sizeof(char), BUF_LEN, fp)) {
				bytesWritten += Write(newSocketfd, input, strlen(input));
			}
	}else {
		err_sys("ERROR: File could not be opened");
	}

	if (bytesWritten < 0 || bytesWritten == 0)
		perror("ERROR: Nothing written to file");
		

    // Reset file pointer
    rewind(fp);
    // Close file
    fclose(fp);
    
    // close the sockets
    close(newSocketfd);
    close(serverSockfd);

    return;
    
}