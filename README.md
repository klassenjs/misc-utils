# misc-utils

A collection of small utilities I wrote to solve some problem I was having
at the time.

`fs_readonly_watchdog`: Reboot if filesystem is remounted read-only

`httpwatchdog.py`: Reboot kiosk if don't receive message over HTTP.

`strerror`: Expose strerror(3) to the shell prompt.

`wait-for-hr`: Like `sleep 3600` but always waits for the top of the hour.

# misc-examples

A collection of example code snippets to explore synatax or library behavior.

`array-of-structs-init`: Initialize an array of structs in C.

`thread-test`: Test pthreads thread scheduling with contended mutexes.

`year-days-to-unix-time`: Calculate a `time_t` from a year and day-of-year.
