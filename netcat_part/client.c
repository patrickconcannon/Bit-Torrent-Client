
#include "nc.h"


void createClient(nc_args_t *nc_args) {			

    int clientSockfd;
    char input[BUF_LEN];
    ssize_t bytesWritten = 0;
    FILE *fp;

    memset(input, 0, BUF_LEN);

	// Open socket
	clientSockfd = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Connect
    Connect(clientSockfd, (struct sockaddr *) &nc_args->servAddr, sizeof(nc_args->servAddr));

    // Open file 
	fp = fopen(nc_args->clientFilename, "r");
	if (fp != NULL) {
			while(fread(input, sizeof(char), BUF_LEN, fp) ) {
				bytesWritten += Write(clientSockfd, input, strlen(input));
			}
	}else {
		err_sys("ERROR: File could not be opened");
	}

	if (bytesWritten < 0 || bytesWritten == 0)
		perror("ERROR: Nothing written to file");
		
	// Reset file pointer
	fseek(fp, SEEK_SET, 0);
	// Close file
	fclose(fp);


    Close(clientSockfd);
    
    return;
    
}