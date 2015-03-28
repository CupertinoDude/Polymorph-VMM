# Template Device for Amiga OS 4.x
#
DNAME	= template.device
AUTODOC	= template.doc

CC		= gcc -N -q
STRIP	= strip
RM		= Delete
AD		= Autodoc
MAKE	= make

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/ -I../../DevKit/Include/
CFLAGS	= -O2 $(CWFLAGS) -gstabs $(INCLUDE) -fno-builtin -fPIC -shared

LDFLAGS	= -nostartfiles -nostdlib -mcrt=newlib
LDLIBS	= -lc

OBJECTS	= main.o	apicalls.o

$(DNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(DNAME).debug
	$(STRIP) $(DNAME).debug -o $(DNAME)

clean:
	$(RM) $(OBJECTS) $(DNAME).debug $(DNAME) ALL QUIET

autodoc:
	$(AD) apicalls.c >$(AUTODOC)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
