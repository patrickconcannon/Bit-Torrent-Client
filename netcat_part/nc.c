#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "nc.h"

void createClient(nc_args_t *);
void createServer(nc_args_t *);


int main(int argc, char *argv[]) {
    
    nc_args_t nc_args;
    parse_args(&nc_args, argc, argv);

    if (nc_args.listen == 1) {
		  createServer(&nc_args);
    } else{
		  createClient(&nc_args);
  	}
    return 0; 
}