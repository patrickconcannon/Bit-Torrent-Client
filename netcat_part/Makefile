
# Makefile for nc
OBJ = nc.o client.o server.o nc_util.o
HDR = nc.h
CC=gcc
CFLAGS=-I.


%.o: %.c $(HDR)
	$(CC) -c -o $@ $< $(CFLAGS)

nc: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -f $(OBJ) nc testdata/results.txt
