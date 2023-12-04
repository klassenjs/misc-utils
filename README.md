# misc-utils

A collection of small utilities I wrote to solve some problem I was having
at the time.

`args`: Outputs it's arguments one per line (for troubleshooting quoting and escaping in scripts).

`fs_readonly_watchdog`: Reboot if filesystem is remounted read-only

`httpwatchdog.py`: Reboot kiosk if don't receive message over HTTP.

`pa_devs`: List available PortAudio devices.

`strerror`: Expose strerror(3) to the shell prompt.

`wait-for-hr`: Like `sleep 3600` but always waits for the top of the hour.

# misc-examples

A collection of example code snippets to explore synatax or library behavior.

`array-of-structs-init`: Initialize an array of structs in C.

`big_vm`: Explore core dump size vs memory allocations.

`desktop_notify`: Test the libnotify API for creating desktop notifications in Gnome.

`inotify_watch`: Test the Linux inotify API.

`mmap`: Test using mmap to create a large file-backed VM space.

`mmap-test`: Test reading and writing to a file via mmap.

`socket`: Test IPC via UNIX sockets.

`thread-test`: Test pthreads thread scheduling with contended mutexes.

`year-days-to-unix-time`: Calculate a `time_t` from a year and day-of-year.

`zeros`: Calculates blocks of all zeros in a file.  (Estimate potential sparseness).

`zombie`: Create 'zombie' processes by not cleaning up after children right away.

