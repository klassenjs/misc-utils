/*
 * socket.c
 *
 * Test IPC via UNIX sockets.
 *
 * Copyright (c) 2017 James Klassen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies of this Software or works derived from this Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char **argv)
{
  struct sockaddr_un addr;
	socklen_t addr_length = sizeof(struct sockaddr_un);
	int fd;
  int conn;

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0) {
		printf("socket failed\n");
		return 1;
	}

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "my.sock");
  if (bind(fd, (struct sockaddr*) &addr, addr_length) != 0) {
		printf("bind() failed\n");
		return 1;
	}

	if (listen(fd, 5) == -1) {
		printf("listen() failed\n");
		return 1;
	}

	while ((conn = accept(fd, (struct sockaddr*) &addr, &addr_length)) > -1) {
		char *buf = "Hello world";
		write(conn, buf, strlen(buf));
		close(conn);
	}
  unlink("my.sock");
}
