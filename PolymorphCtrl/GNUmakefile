# Polymorph.Library for Amiga OS 4.x
#
LNAME	= polymorphctrl

CC		= gcc -N -q
STRIP	= strip
RM		= Delete

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/
CFLAGS	= -O2 $(INCLUDE) -fno-builtin -fPIC -shared $(CWFLAGS)

LDFLAGS	= -mcrt=newlib
LDLIBS	= -lc -lm

OBJECTS	= main.o \
          rsrc.o

$(LNAME): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(LNAME).debug
	$(STRIP) $(LNAME).debug -o $(LNAME)

clean:
	$(RM) $(OBJECTS) $(LNAME).debug $(LNAME) ALL QUIET

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<
