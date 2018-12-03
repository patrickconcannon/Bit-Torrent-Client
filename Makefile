CC=gcc
CPFLAGS=-g -Wall # C preprocessor flag
LDFLAGS= -lcrypto # linker flags

# For more info on how make files work -- see below
# https://www.gnu.org/software/make/manual/html_node/index.html#SEC_Contents

SRC= bencode.c  bt_client.c bt_lib.c bt_setup.c  # variable to keep track of source filees
OBJ=$(SRC:.c=.o) # and object files
BIN=bt_client # the binary to be created

all: $(BIN) # tag for all files

$(BIN): $(OBJ) # rule for object files
	$(CC) $(CPFLAGS) $(LDFLAGS) -o $(BIN) $(OBJ) # how to compile this binary 


%.o:%.c # 
	$(CC) -c $(CPFLAGS) -o $@ $<  # how to compile; two at end stand for variables left and right of ':' 

$(SRC): # tag for what to do with src files

clean: # what the command 'make clean' will run  
	rm -rf $(OBJ) $(BIN)
