#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#include "bt_setup.h"
#include "bt_lib.h"
#include "bencode.h"


/**
 * usage(FILE * file) -> void
 *
 * print the usage of this program to the file stream file
 *
 **/

void usage(FILE * file){
  if(file == NULL){
    file = stdout;
  }

  fprintf(file,
          "bt-client [OPTIONS] file.torrent\n"
          "  -h            \t Print this help screen\n"
          "  -b ip         \t Bind to this ip for incoming connections, ports\n"
          "                \t are selected automatically\n"
          "  -s save_file  \t Save the torrent in directory save_dir (dflt: .)\n"
          "  -l log_file   \t Save logs to log_filw (dflt: bt-client.log)\n"
          "  -p ip:port    \t Instead of contacing the tracker for a peer list,\n"
          "                \t use this peer instead, ip:port (ip or hostname)\n"
          "                \t (include multiple -p for more than 1 peer)\n"
          "  -I id         \t Set the node identifier to id (dflt: random)\n"
          "  -v            \t verbose, print additional verbose info\n");
}

/**
 * __parse_peer(peer_t * peer, char peer_st) -> void
 *
 * parse a peer string, peer_st and store the parsed result in peer
 *
 * ERRORS: Will exit on various errors
 **/

void __parse_peer(peer_t * peer, char * peer_st){
  char * parse_str;
  char * word;
  unsigned short port;
  char * ip;
  char id[20];
  char sep[] = ":";
  int i;

  //need to copy becaus strtok mangels things
  parse_str = malloc(strlen(peer_st)+1);
  strncpy(parse_str, peer_st, strlen(peer_st)+1);


  /**
   * The C library function char *strtok(char *str, const char *delim) 
   * breaks string str into a series of tokens using the delimiter delim. 
   * 
   * Works as a string tokenizer returning a sub-string / string token each
   * time it is called. 
   **/

  // This takes in the address as a xxx.xxx.xxx.xxx:xx, ip:port
  // could be just ip... see below
  //only can have 2 tokens max, but may have less
  for(word = strtok(parse_str, sep), i=0;   //get the first token
      (word && i < 3);  // word !=null &&  i < 3
      word = strtok(NULL,sep), i++){ // keep returning tokens until empty

    printf("%d:%s\n",i,word);
    switch(i){
    case 0://id
      ip = word;
      break;
    case 1://ip
      port = atoi(word);
    default:
      break;
    }

  }

  if(i < 2){ 
    fprintf(stderr,"ERROR: Parsing Peer: Not enough values in '%s'\n",peer_st);
    usage(stderr); // change print out to std, and write to it
    exit(1);
  }

  if(word){
    fprintf(stderr, "ERROR: Parsing Peer: Too many values in '%s'\n",peer_st);
    usage(stderr);
    exit(1);
  }


  //calculate the id, value placed in id -- hashs these values using SHA1()
  calc_id(ip,port,id); // can't understand why the id is hashed ??

  //build the object we need
  init_peer(peer, id, ip, port);
  
  //free extra memory
  free(parse_str);

  return;
}

/**
 * pars_args(bt_args_t * bt_args, int argc, char * argv[]) -> void
 *
 * parse the command line arguments to bt_client using getopt and
 * store the result in bt_args.
 *
 * ERRORS: Will exit on various errors
 *
 **/
void parse_args(bt_args_t * bt_args, int argc,  char * argv[]){
  int ch; //ch for each flag
  int n_peers = 0;
  int i;

  /* set the default args */
  bt_args->verbose=0; //no verbosity
  
  //null save_file, log_file and torrent_file -- set the file to empty 
  memset(bt_args->save_file,0x00,FILE_NAME_MAX);
  memset(bt_args->torrent_file,0x00,FILE_NAME_MAX);
  memset(bt_args->log_file,0x00,FILE_NAME_MAX);
  
  //null out file pointers
  bt_args->f_save = NULL;

  //null bt_info pointer, should be set once torrent file is read
  bt_args->bt_info = NULL;

  //default lag file
  strncpy(bt_args->log_file,"bt-client.log",FILE_NAME_MAX);
  
  for(i=0;i<MAX_CONNECTIONS;i++){
    bt_args->peers[i] = NULL; //initially NULL
  }

  bt_args->id = 0;
  
  while ((ch = getopt(argc, argv, "hp:s:l:vI:")) != -1) { // reading the flags
    switch (ch) {
    case 'h': //help
      usage(stdout);
      exit(0);
      break;
    case 'v': //verbose
      bt_args->verbose = 1;
      break;
    case 's': //save file
      /**
       * The optarg() function takes in the next command-line arg.
       * In this case will be the file to be saved into
       **/ 
      strncpy(bt_args->save_file,optarg,FILE_NAME_MAX);
      break;
    case 'l': //log file
      strncpy(bt_args->log_file,optarg,FILE_NAME_MAX);
      break;
    case 'p': //peer
      n_peers++;
      //check if we are going to overflow
      if(n_peers > MAX_CONNECTIONS){
        fprintf(stderr,"ERROR: Can only support %d initial peers",MAX_CONNECTIONS);
        usage(stderr);
        exit(1);
      }

      /**
       * Each element is given its allocation of memory, one n_peer at a time
       **/ 
      bt_args->peers[n_peers] = malloc(sizeof(peer_t));

      //parse peers
      __parse_peer(bt_args->peers[n_peers], optarg); // initialise each peer struct
      break;
    case 'I':
      bt_args->id = atoi(optarg);
      break;
    default:
      fprintf(stderr,"ERROR: Unknown option '-%c'\n",ch);
      usage(stdout);
      exit(1);
    }
  }


  argc -= optind; // argc is the count of arguments passed in via the command line
  argv += optind; // argv stands for argument vector; optind is the index of that vector

  if(argc == 0){
    fprintf(stderr,"ERROR: Require torrent file\n");
    usage(stderr);
    exit(1);
  }

  //copy torrent file over
  strncpy(bt_args->torrent_file,argv[0],FILE_NAME_MAX);

  return ;
}

