#include "nc.h"


void err_sys(const char* x) {
    perror(x);
    exit(1);
}

int Socket(int family, int type, int protocol) {
    int n;
    if ((n = socket(family, type, protocol)) < 0)
        err_sys("socket error");
        return (n);
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen){
	if (bind(fd, sa, salen) < 0)
		err_sys("bind error");
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen){
	if (connect(fd, sa, salen) < 0)
		err_sys("connect error");
}

int Listen(int fd, int backlog){
    int n;
	if ((n = listen(fd, backlog)) < 0)
		err_sys("listen error");
    return n;
}

ssize_t Write(int fd, void *ptr, size_t nbytes){
    ssize_t n;
	if ((n = write(fd, ptr, nbytes)) != nbytes)
		err_sys("write error");
    return n;
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr){
	int	n;
	if ((n = accept(fd, sa, salenptr)) < 0) {
			err_sys("accept error");
	}
	return(n);
}

void Close(int fd){
	if (close(fd) == -1)
		err_sys("close error");
}


int checkport(unsigned short p) {
    return ((p >= 0 && p < 65535) ? 1 : 0);
}


void parse_args(nc_args_t *nc_args, int argc, char *argv[]){
    
    int ch;
    struct hostent *hostinfo;			
    //set defaults
    nc_args->n_bytes = 0;
    nc_args->offset = 0;
    nc_args->listen = 0;
    nc_args->port = 6767;
    nc_args->verbose = 0;
    nc_args->message_mode = 0;
 
    while ((ch = getopt(argc, argv, "lm:hvp:n:o:")) != -1) {			
		switch (ch) { 
			case 'l':
				nc_args->listen = 1;
				break;
			case 'o':
				nc_args->offset = atoi(optarg);
				break;
			case 'n':
				nc_args->n_bytes = atoi(optarg);
				if (nc_args->n_bytes == 0) {
					fprintf(stderr, "ERROR: Invalid byte value");
					exit(1);
				}
				break;
			case 'm':
				nc_args->message_mode = 1;
				nc_args->message = (char *) malloc(strlen(optarg) + 1);
				strncpy(nc_args->message, optarg, strlen(optarg) + 1);
				break;
			default:
				usage(stdout);
				fprintf(stderr,"ERROR: Unknown option '-%c'\n",ch);
				exit(1);
		}
    }
 
    argc -= optind;
    argv += optind;
    
    if (argc < 2 && nc_args->message_mode == 0) {
		fprintf(stderr, "ERROR: Require IP and file\n");
		exit(1);
    } else if (argc != 1 && nc_args->message_mode == 1) {
		fprintf(stderr, "ERROR: Require IP to send/recv from when in message mode\n");
		exit(1);
    }
 
    if( !(hostinfo = gethostbyname(argv[0])) ){
		fprintf(stderr,"ERROR: Invalid host name '%s' specified\n", argv[0]);
		exit(1);
    }
    nc_args->servAddr.sin_family = hostinfo->h_addrtype;
    memmove( (char *) &(nc_args->servAddr.sin_addr.s_addr), (char *) hostinfo->h_addr, hostinfo->h_length );	
    nc_args->servAddr.sin_port = htons(nc_args->port);			
 

    if (nc_args->message_mode != 1) {
		if (nc_args->listen == 1) {
			// store server's output file name
			nc_args->serverFilename = (char *) malloc(strlen(argv[1]) + 1);
			strncpy( nc_args->serverFilename, argv[1], (strlen(argv[1]) + 1) );
		}	

		else {
			// store client's input file name
			nc_args->clientFilename = (char *) malloc(strlen(argv[1]) + 1);
			strncpy( nc_args->clientFilename, argv[1], (strlen(argv[1]) + 1) );
		}
    }
    return;
}

void usage(FILE *file){
    fprintf(file,
	    "netcat_part [OPTIONS]dest_ip [file] \n"
	    "\t -m \"MSG\"   \t\t Send the message specified on the command line. \n"
	    "                \t\t Warning: if you specify this option, you do not specify a file. \n"
	    "\t -n bytes     \t\t Number of bytes to send, defaults whole file\n"
	    "\t -o offset    \t\t Offset into file to start sending\n"
	    "\t -l           \t\t Listen on port instead of connecting and write output to file\n"
	    "                \t\t and dest_ip refers to which ip to bind to (dflt: localhost)\n"
	    );
}