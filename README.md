# Bit Torrent Client
**Name:** Patrick Concannon

**Username:** pconcannon

**Project:** Bit Torrent Client

## Description
This is the README file for the bit torrent client
In here you will find the structure and outline of how this program will work

## Files
* bencode.c|h
* bt_lib.c|h
* bt_setup.c|h
* bt_client
* Makefile
* client_trace.[n].log
* sample_torrent.torrent
* README
* fileTransfer/
  - testdata/
    - alphabet.txt
  - Makefile
  - client.c
  - nc.c|h
  -  nc_util.c
  - server.c

## Example Usage

```

nc [OPTIONS] dest_ip [file]
	 -h          		 Print this help screen
	 -p port     		 Set the port to connect on (dflt: 6767)
	 -m maintain     Maintain offset from previous data transfer
   -n bytes    		 Number of bytes to send - defaults to entire file
	 -o offset   		 Offset into file to begin sending
	 -l          		 Create listen server with specified address
   
```

## Build

*note* Currently, only the fileTransfer section is running. 

From the fileTransfer folder, you must first build the project:
```
$ cd github.com/patrickconcannon/Bit-Torrent-Client/fileTransfer`
$ make 
```

To run the server:
```
$ ./nc -l localhost testdata/alphabet.txt
```

To run the client:
```
$ ./nc localhost testdata/result.txt
```

In addition to this standard usage, the fileTransfer section has the additional feature of being able to send parts of a file. For example if we wish to send a file, such as an .mp3, and wish to only want to receive the first 100 bytes of the file
```
$ ./nc -n 100 localhost test.mp3
```
And if you want to send the first 100 bytes offset into the file, you can use this similar command:
```
$ ./nc -o 256 -n 100  localhost test.mp3
```
Which will send 100 bytes of the file starting at byte 256. That is, it will send bytes 256 through 355
of the file.
By specifying the -m flag, the user can write the 100 bytes to the output file at the appropriate offset, and on successive runs of fileTransfer for the same output file, it will not truncated the file and continue to write data to the appropriate location in the file.
```
$ ./nc -o 256 -n 100 -m localhost test.mp3
```
