# Polymorph.Library for Amiga OS 4.x
#
LNAME	= template.polymorph

CC		= gcc -N -q
STRIP	= strip
RM		= Delete
AD		= Autodoc
MAKE	= Make

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/ -I../../DevKit/Include/
CFLAGS	= -O2 $(INCLUDE) -fno-builtin -fPIC -shared $(CWFLAGS)

LDFLAGS	= -nostartfiles -nostdlib -mcrt=newlib
LDLIBS	= -lc

OBJECTS	=	main.o		apicalls.o		emucalls.o	\
			segments.o	messages.o

$(LNAME): devkit $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(LNAME).debug
	$(STRIP) $(LNAME).debug -o $(LNAME)

devkit:
	$(MAKE) -C ../../DevKit -f GNUmakefile

clean:
	$(RM) $(OBJECTS) $(LNAME).debug $(LNAME) ALL QUIET

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<
