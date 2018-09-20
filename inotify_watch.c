/*
 * inotify_watch.c
 *
 * Test the Linux inotify API.
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
#include <errno.h>
#include <limits.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <alloca.h>

int main(int argc, char** argv) {
	int fd, i;
  struct inotify_event *evt;
  int* watches;

  watches = (int*)alloca(sizeof(int)*(argc+1));
  evt = (struct inotify_event*)alloca(sizeof(struct inotify_event) + NAME_MAX + 1);

	fd = inotify_init();

  if(fd == -1) {
		fprintf(stderr, "inotify_init() failed, aborting.\n");
    return 1;
  }

	i = argc;	
	while(i > 1) {
		char *fname = argv[--i];
		watches[i] = inotify_add_watch(fd, fname, IN_ALL_EVENTS);
    if(watches[i] == -1) {
     perror("inotify_add_watch failed:");
     return 1;
    }
	}	

 	while(1) {
    char* fname;
		read(fd, evt, sizeof(struct inotify_event) + NAME_MAX + 1);
   	for(i = 0; i < argc; i++) {
      if(watches[i] == evt->wd) {
        fname = argv[i];
      }
    }  
	  printf("Watch id %d (0x%04x): %s ( ", evt->wd, evt->mask, fname);
    if(evt->mask & IN_ACCESS) printf("IN_ACCESS ");
    if(evt->mask & IN_ATTRIB) printf("IN_ATTRIB ");
    if(evt->mask & IN_CLOSE_WRITE) printf("IN_CLOSE_WRITE ");
    if(evt->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
    if(evt->mask & IN_CREATE) printf("IN_CREATE %s ", evt->name);
    if(evt->mask & IN_DELETE) printf("IN_DELETE ");
    if(evt->mask & IN_DELETE_SELF) printf("IN_DELETE_SELF ");
    if(evt->mask & IN_MODIFY) printf("IN_MODIFY ");
    if(evt->mask & IN_MOVE_SELF) printf("IN_MOVE_SELF ");
    if(evt->mask & IN_MOVED_FROM) printf("IN_MOVED_FROM ");
    if(evt->mask & IN_MOVED_TO) printf("IN_MOVED_TO ");
    if(evt->mask & IN_OPEN) printf("IN_OPEN ");
    printf(")\n");
  } 
	
	return 0;
}
