INCLUDE_DIRS =
LIB_DIRS =

CDEFS=
CFLAGS= -O -g $(INCLUDE_DIRS) $(CDEFS) -DLINUX
LIBS=-lpthread -lrt

HFILES=

CFILES1= deadlock.c
CFILES2= deadlock_fix.c
CFILES3= pthread3.c
CFILES4= pthread3ok.c



SRCS1= ${HFILES} ${CFILES1}
SRCS2= ${HFILES} ${CFILES2}
SRCS3= ${HFILES} ${CFILES3}
SRCS4= ${HFILES} ${CFILES4}



OBJS1= ${CFILES1:.c=.o}
OBJS2= ${CFILES2:.c=.o}
OBJS3= ${CFILES3:.c=.o}
OBJS4= ${CFILES4:.c=.o}


all:  deadlock deadlock_fix pthread3 pthread3ok

clean:
	-rm -f *.o *.d *.exe  deadlock deadlock_fix pthread3 pthread3ok


deadlock: deadlock.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS1) $(LIBS)

deadlock_fix: deadlock_fix.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS2) $(LIBS)
	
pthread3: pthread3.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS3) $(LIBS)
	
pthread3ok: pthread3ok.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(OBJS4) $(LIBS)



depend:

.c.o:
	$(CC) -MD $(CFLAGS) -c $<
