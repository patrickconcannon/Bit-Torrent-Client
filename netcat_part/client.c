
#include "nc.h"


void createClient(nc_args_t *nc_args) {			
    
    int clientSockfd;
    unsigned char buffer[BUF_LEN];
    ssize_t bytesWritten = 0;
    FILE *fp;

    bzero(buffer, BUF_LEN);

    // Open socket
    clientSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // Connect
    Connect(clientSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));

    // Open file 
    fp = fopen(nc_args->clientFilename, "rb");
	
    if (fp != NULL) {
        // check for offset
        if(nc_args->offset != 0){
            fseek(fp, nc_args->offset, SEEK_SET);
        } 
        while(fread(buffer, sizeof(BUF_LEN), 1, fp)) {
            bytesWritten += Write(clientSockfd, buffer, BUF_LEN);
        }
        printf("Client: %ld bytes sent to server\n", bytesWritten);
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
    Close(clientSockfd);
    
    return;
}
