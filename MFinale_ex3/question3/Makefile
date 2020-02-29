INCLUDE_DIRS = 
LIB_DIRS = 

CDEFS=
CFLAGS= -O -g $(INCLUDE_DIRS) $(CDEFS) -DLINUX
LIBS=-lpthread -lrt

HFILES=

CFILES1= deadlock.c
CFILES2= deadlock_fix.c
CFILES3= deadlock_8.c

SRCS1= ${HFILES} ${CFILES1}
SRCS2= ${HFILES} ${CFILES2}
SRCS3= ${HFILES} ${CFILES3}


OBJS1= ${CFILES1:.c=.o}
OBJS2= ${CFILES2:.c=.o}
OBJS3= ${CFILES3:.c=.o}

all:  deadlock deadlock_fix

clean:
	-rm -f *.o *.d *.exe  deadlock deadlock_fix deadlock_8


deadlock: deadlock.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS1) $(LIBS)

deadlock_fix: deadlock_fix.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS2) $(LIBS)
	
deadlock_8: deadlock_8.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS3) $(LIBS)

depend:

.c.o:
	$(CC) -MD $(CFLAGS) -c $<
