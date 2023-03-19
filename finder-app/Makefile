#CROSS_COMPILE = aarch64-none-linux-gnu-
CC = $(CROSS_COMPILE)gcc

HFILES = 
CFILES = writer.c

SRCS = ${HFILES} ${CFILES}
OBJS = ${CFILES:.c=.o}

all: writer

clean:
	-rm -f *.o *.d
	-rm -f writer

writer: $(OBJS)
	$(CC) $(OBJS) -o writer

