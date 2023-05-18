CC ?= $(CROSS_COMPILE)gcc

HFILES = 
CFILES = aesdsocket.c

SRCS = ${HFILES} ${CFILES}
OBJS = ${CFILES:.c=.o}

CFLAGS ?= -g -Wall -Werror
LDFLAGS ?= -pthread -lrt

all: aesdsocket

clean:
	-rm -f *.o *.d
	-rm -f aesdsocket

aesdsocket: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(INCLUDES) $(LDFLAGS)
