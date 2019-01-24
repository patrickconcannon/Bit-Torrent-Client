
#ifndef NETCAT_PART
#define NETCAT_PART


#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_LEN 1024				// data buffer length
#define MAXQUEUE 5			        // maximum number of pending client connections 

typedef struct nc_args {
    struct sockaddr_in servAddr;    // server address information
    unsigned short port;            // server's listening port
    unsigned short listen;			// listen flag
    int n_bytes;				    // number of bytes to send
    int offset;					    // file offset
    int verbose;				    // verbose output info
    int message_mode;				// to indicate message is being sent by client
    char *message;				    // if message_mode is activated, this will store the message
    char *serverFilename;			// output file's name
    char *clientFilename;			// input file's name
} nc_args_t;

void err_sys(const char* x);
int Socket(int family, int type, int protocol);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Listen(int fd, int backlog);
ssize_t Write(int fd, void *ptr, size_t nbytes);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
void Close(int fd);

#endif