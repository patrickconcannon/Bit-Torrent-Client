
#include "nc.h"


void createClient(nc_args_t *nc_args) {			

    int clientSockfd;
	unsigned char buffer[BUF_LEN];
   	ssize_t bytesRead, totalBytesRead = 0;
    FILE *fp;

    memset(buffer, 0, BUF_LEN);

	// Open socket
	clientSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Connect
    Connect(clientSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));



    // Open file 
    fp = fopen(nc_args->clientFilename, "w+");
    
    while((bytesRead = read(clientSockfd, buffer, BUF_LEN)) != 0 ) {	
	    totalBytesRead += bytesRead;
	    fwrite(buffer, sizeof(BUF_LEN), 1, fp);
	    memset(buffer, 0, BUF_LEN);
    }
    printf("Client: %ld bytes written to file '%s'\n", totalBytesRead, nc_args->clientFilename);

	// Reset file pointer
	fseek(fp, 0, SEEK_SET);
	// Close file and socket
	fclose(fp);
    Close(clientSockfd);
    
    return;
}