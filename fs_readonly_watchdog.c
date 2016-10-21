/**************************************************************************
 * fs_readonly_watchdog.c
 *
 * This is a small utility to restart a machine if a filesystem is
 * remounted read-only.  This is made to work around an iSCSI SAN
 * intermittently dropping connection.
 *
 * It works by watching /proc/self/mounts for changes via pselect(2).
 * When it changes it looks for any filesystems of type ext? that are
 * read-only.  If it finds a read-only filesystem, it then waits 60
 * seconds and then if there are still read-only filesystems it
 * reboots the system by sending SIGINT to PID 1.
 *
 * Build with:
 *    make fs_readonly_watchdog
 * 
 * Run (as root):
 *    ./fs_readonly_watchdog &
 *
 **************************************************************************
 * Copyright (c) 2015 James Klassen
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
 **************************************************************************/

#include<stdio.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<signal.h>
#include<sys/select.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

#define LINE_MAXLEN 4096

const char* mounts_path = "/proc/self/mounts";

int read_mounts(int fd) {
  char line[LINE_MAXLEN];
  char *pos;
  char *fs_spec;
  char *fs_file;
  char *vfs_type;
  char *fs_mntops;
  char *fs_freq;
  char *fs_passno;
  char *mntop;
  char *saveptr;
  int bytes = 0;
  int err;

  int read_only_count = 0;

  /* Read file from beginning */
  if(lseek(fd, 0, SEEK_SET) == -1) {
    perror("can't seek");
    return(1);
  }
 
  for(;;) {
    memset(line, 0, LINE_MAXLEN);
    for( bytes = 0, pos = line; bytes < LINE_MAXLEN; bytes++, pos++ ) {
      err = read(fd, pos, 1);
      if(err == 0) {
	/* Reached end of file */
	return(read_only_count);
      }
      if(err == -1) {
	perror("Can't read mounts");
	return(-1);
      }
      if(*pos == '\n') {
	/* Got end of line, start parsing */
	fs_spec = strtok_r(line, " ", &saveptr);
	fs_file = strtok_r(NULL, " ", &saveptr);
	vfs_type = strtok_r(NULL, " ", &saveptr);
	fs_mntops = strtok_r(NULL, " ", &saveptr);
	fs_freq = strtok_r(NULL, " ", &saveptr);
	fs_passno = strtok_r(NULL, " ", &saveptr);

	/* Look for ext[234] filesystems, ignore the rest */
	if( vfs_type != NULL && strncmp( vfs_type, "ext", 3) == 0 ) {
	  /* Parse options */
	  for( mntop = strtok_r(fs_mntops, ",", &saveptr); 
	       mntop != NULL;
	       mntop = strtok_r(NULL, ",", &saveptr)) {
            if( strcmp("ro", mntop) == 0 ) {
	      printf("FOUND %s IS READ ONLY!\n", fs_file);
	      read_only_count++;
	    }
	  }
	}
	break; /* Drop out of for loop and get the next line */
      }
      if(bytes == LINE_MAXLEN) {
	printf("Line too long, skipping...\n");
      }
    }
  }
  return(-1); /* Can't get here */
}

int wait_for_update(int fd) {
  int ready;
  fd_set fds;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  ready = pselect(fd + 1, NULL, NULL, &fds, NULL, NULL);

  if (ready == 1) {
    printf("Ready\n");
    return ready;
  } else if(ready == -1) {
    perror("pselect failed");
  }
  return -1;
}

int main() {
  int mounts_fd = 0;
  int read_only_count;
  struct timeval failure_time;

  mounts_fd = open(mounts_path, O_RDONLY);
  if(mounts_fd == -1) {
    perror("Couldn't open mounts");
    return(1);
  }

  for(;;) {
    read_only_count = read_mounts(mounts_fd);  
    if(read_only_count == -1)
      return(1);
    else if(read_only_count > 0) {
      /* We have a problem */
      if( gettimeofday(&failure_time, NULL) == -1 ) {
	perror("Can't get time of day");
	return 1;
      }

      /* Wait 60 seconds */
      sleep(60);
 
      /* If still read only, reboot */
      if( read_mounts(mounts_fd) > 0 ) {
	/* Reboot the system */	
	if( kill((pid_t)1, SIGINT) == -1 ) {
	  perror("Can't reboot");
	  return 1;
	} 
      }
    }

    /* Wait for update to mounts_path */
    if( wait_for_update(mounts_fd) == -1)
	return 1;
  }

  return 0;
}
