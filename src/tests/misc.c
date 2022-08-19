/*
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
*/
#include <sys/select.h>
#include <stdlib.h>

int fd_available(int fd)
{
#if 1
	fd_set	rfds;
	struct timeval tout = { .tv_sec = 0, .tv_usec = 1000000/100 };
	FD_ZERO(&rfds);
	FD_SET(fd, &rfds);
	select(FD_SETSIZE, &rfds, NULL, NULL, &tout);
	if(FD_ISSET(fd, &rfds))
		return 1;
	return 0;
#else
	int bytes = -1;
	ioctl(fd, FIONREAD, &bytes);
	return bytes;
#endif
}

