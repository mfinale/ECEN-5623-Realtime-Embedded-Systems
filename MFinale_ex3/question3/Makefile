INCLUDE_DIRS = 
LIB_DIRS = 

CDEFS=
CFLAGS= -O -g $(INCLUDE_DIRS) $(CDEFS) -DLINUX
LIBS=-lpthread -lrt

HFILES=

CFILES1= deadlock.c
CFILES2= deadlock_fix.c

SRCS1= ${HFILES} ${CFILES1}
SRCS2= ${HFILES} ${CFILES2}


OBJS1= ${CFILES1:.c=.o}
OBJS2= ${CFILES2:.c=.o}

all:  deadlock deadlock_fix

clean:
	-rm -f *.o *.d *.exe  deadlock deadlock_fix


deadlock: deadlock.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS1) $(LIBS)

deadlock_fix: deadlock_timeout.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS2) $(LIBS)

depend:

.c.o:
	$(CC) -MD $(CFLAGS) -c $<
