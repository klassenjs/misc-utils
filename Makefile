CFLAGS=-std=c99 -Os -pedantic -Wall

APPS=ext4-find-superblocks
APPS+=args
APPS+=fs_readonly_watchdog
APPS+=strerror
APPS+=thread-test
APPS+=wait-for-hr
APPS+=xor
APPS+=year-days-to-unix-time
APPS+=zeros

all: ${APPS}

thread-test: CFLAGS += -pthread
thread-test: LDFLAGS += -pthread

fs_readonly_watchdog: CFLAGS += -std=gnu99

clean:
	rm -f ${APPS}

