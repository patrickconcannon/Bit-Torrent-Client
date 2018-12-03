#ifndef _BT_SETUP_H
#define _BT_SETUP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "bt_setup.h"
#include "bt_lib.h"
#include "bencode.h"

/**
 * useage(FILE *file) -> void 
 * 
 * Set's the output e.g. stdout, stderr, to a file, etc.
 **/
void usage(FILE * file);

/**
 * __parse_peer(peer_t * peer, char peer_st) -> void
 *
 * private function: used within parse_args() below
 * parse a peer string, peer_st and store the parsed result in peer
 *
 * ERRORS: Will exit on various errors
 **/

 // void __parse_peer(peer_t *peer, char peer_st) 

/**
 * pars_args(bt_args_t * bt_args, int argc, char * argv[]) -> void
 *
 * parse the command line arguments to bt_client using getopt and
 * store the result in bt_args.
 *
 * ERRORS: Will exit on various errors
 *
 **/

void parse_args(bt_args_t * bt_args, int argc,  char ** argv);



#endif
