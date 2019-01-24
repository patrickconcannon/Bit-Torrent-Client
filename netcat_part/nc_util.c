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