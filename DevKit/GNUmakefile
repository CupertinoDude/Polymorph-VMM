#
#	'Polymorph.Library' Build Scripting
#
CC		= gcc -N
IDLTOOL	=	SDK:C/idltool -p -n -i
MKDIR	=	C:Makedir
RM		=	C:Delete ALL FORCE QUIET

CWFLAGS	= -Wall -Wundef -Wshadow -Wpointer-arith -Wbad-function-cast -Wconversion -Waggregate-return
INCLUDE	= -I/SDK/includes/include_h/ -I/SDK/Workspace/Polymorph-VM/DevKit/Include/
CFLAGS	= -O2 $(INCLUDE) -fno-builtin -fPIC -shared $(CWFLAGS)

LDFLAGS	= -nostartfiles -nostdlib -mcrt=newlib
LDLIBS	= -lc

main:   Includes
	$(CC) $(CFLAGS) -c -o emucall.o emucall.s
	@ Echo "Automagic Updates applied..."

Includes:
	@ ${IDLTOOL} polymorph.xml
	@ ${IDLTOOL} polymorphclass.xml

clean:
	${RM}	Include/proto Include/inline Include/inline4 Include/interfaces
	@ ${MKDIR} Include/proto Include/inline Include/inline4 Include/interfaces
	${RM}	emucall.o
