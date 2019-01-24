

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>				// for getopt(), optarg, optind, etc.
#include <string.h>				// for memmove()
 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "nc.h"				// header file for nc_args_t structure; this structure comprises of all command line arguments


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


int checkport(unsigned short p) {
    return ((p >= 0 && p < 65535) ? 1 : 0);			// 1: all OK; 0: all NOT OK
}


void parse_args(nc_args_t *nc_args, int argc, char *argv[]){
    
    int ch;
    struct hostent *hostinfo;			/* to store all relevant information regarding the server or destination host and 
						 * then use it to populate our nc_args structure */

    //set defaults
    nc_args->n_bytes = 0;
    nc_args->offset = 0;
    nc_args->listen = 0;
    nc_args->port = 6767;
    nc_args->verbose = 0;
    nc_args->message_mode = 0;
 
    while ((ch = getopt(argc, argv, "lm:hvp:n:o:")) != -1) {			
		switch (ch) {
			case 'l':					// direct server to listen for incoming client connections
				nc_args->listen = 1;
				break;
			case 'o':					// offset into file
				nc_args->offset = atoi(optarg);
				break;
			case 'n':					// number of bytes to be sent
				nc_args->n_bytes = atoi(optarg);
				if (nc_args->n_bytes == 0) {
					fprintf(stderr, "Absurd value entered for number of bytes to read from file. Try again.");
					usage(stdout);
					exit(1);
				}
				break;
			case 'm':
				nc_args->message_mode = 1;		// imply that there will be a message passed to server by client instead of a file
				nc_args->message = (char *) malloc(strlen(optarg) + 1);		// allocate memory for string message, one extra than message length for '\0'
				strncpy(nc_args->message, optarg, strlen(optarg) + 1);	// store command-line message
				break;
			default:
				fprintf(stderr,"ERROR: Unknown option '-%c'\n",ch);
				usage(stdout);
				exit(1);						// exit by indicating unsuccessful program termination
		}
    }
 
    argc -= optind;
    argv += optind;
    
    if (argc < 2 && nc_args->message_mode == 0) {
		fprintf(stderr, "ERROR: Require IP and file\n");
		usage(stderr);
		exit(1);
    } else if (argc != 1 && nc_args->message_mode == 1) {
		fprintf(stderr, "ERROR: Require IP to send/recv from when in message mode\n");
		usage(stderr);
		exit(1);
    }
 
    if( !(hostinfo = gethostbyname(argv[0])) ){				// gethostbyname() returns the hostent structure or NULL on failure
		fprintf(stderr,"ERROR: Invalid host name '%s' specified\n", argv[0]);
		usage(stderr);
		exit(1);
    }

    nc_args->servAddr.sin_family = hostinfo->h_addrtype;
    memmove( (char *) &(nc_args->servAddr.sin_addr.s_addr), (char *) hostinfo->h_addr, hostinfo->h_length );	
    nc_args->servAddr.sin_port = htons(nc_args->port);			
 
    /* Save file names if not in message mode */
    if (nc_args->message_mode != 1) {			// if not in message mode, then
		// either server with listen mode is being called
		if (nc_args->listen == 1) {
			// store server's output file name
			nc_args->serverFilename = (char *) malloc(strlen(argv[1]) + 1);
			strncpy( nc_args->serverFilename, argv[1], (strlen(argv[1]) + 1) );
		}	
		// or client in file send mode is being called
		else {
			// store client's input file name
			nc_args->clientFilename = (char *) malloc(strlen(argv[1]) + 1);
			strncpy( nc_args->clientFilename, argv[1], (strlen(argv[1]) + 1) );
		}
    }
    
    return;
    
}

void createClient(nc_args_t *);
void createServer(nc_args_t *);

int main(int argc, char *argv[]) {
    
    nc_args_t nc_args;
    
    //initializes the arguments struct for your use
    parse_args(&nc_args, argc, argv);

    // set up a client or server based on user input
    if ((&nc_args)->listen == 1) {				// check to see if server is being asked to run or client
	
	// run more sanity checks before initiating server
	int retval;
	if ( ( retval = checkport( (&nc_args)->port ) ) == 0) {
	    printf("testing: reaches here\n");	    
	    fprintf(stderr, "Please enter a valid port number: 0-65535\n");
	    usage(stdout);
	    exit(1);
	} else if ( (&nc_args)->port < 2000) {
	    fprintf(stderr, "Not preferred to have port number lesser than 2000; UNIX usually engages them. Please try again.\n");
	    usage(stdout);
	    exit(1);
	}
	createServer(&nc_args);					// construct a server
    } else
	createClient(&nc_args);					// for all other netcat_part executions, construct the client
								 /* calls to construct server or client are made by passing over all information collected from
								 * the user's command to construct both server & client */				 
									
    return 0;
    
}