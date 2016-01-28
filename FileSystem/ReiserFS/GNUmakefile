# ReiserFileSystem for Amiga OS 4.x
#
LNAME	= ReiserFileSystem

CC		= gcc -N -q
STRIP	= strip
RM		= Delete
MAKE	= Make

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/
CFLAGS	= -O2 $(INCLUDE) -fno-builtin -fPIC -shared $(CWFLAGS)

LDFLAGS	= -mcrt=newlib
LDLIBS	= -lc

OBJECTS	= 	main.o	\
			a.o	\
			z.o

$(LNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(LNAME).debug
	$(STRIP) $(LNAME).debug -o $(LNAME)

clean:
	$(RM) $(OBJECTS) $(LNAME).debug $(LNAME) ALL QUIET

%.o:	%.c
	$(CC) $(CFLAGS) -s -c -o $@ $<
