# $Source: /usr/home/system/bin/dev/c/beeperd-0.01/RCS/Makefile,v $
# $Revision: 1.3 $
# $Date: 1999/09/21 03:05:52 $

CC=	gcc
STD=	_GNU_SOURCE
OBJS=	beeperd.o
#main.o beeperd.o

.c.o:
	$(CC)	-c	-Wall $(CFLAGS) -D$(STD) $<

all:	beeperd

beeperd: $(OBJS)
	$(CC) $(OBJS) -o beeperd

clean:
	rm -f *.o core

clobber: clean
	rm -f beeperd

# End Makefile
