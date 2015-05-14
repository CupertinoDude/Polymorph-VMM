#
#	'Polymorph.Library' Build Scripting
#
MAKE	=	SDK:C/make
MKDIR	=	C:Makedir
COPY	=	C:Copy
RM		=	C:Delete ALL FORCE QUIET

# Preparations for a release
world: prepack all
	${COPY}		Library/#?.library#?			Release/Libs
	${COPY}		Plugins/#?/#?.polymorph#?		Release/Plugins

release: world
	@ ${MAKE}	-C Release		-f GNUmakefile

all:
	@ ${MAKE} -C Devkit 	-f GNUmakefile
	@ ${MAKE} -C Library	-f GNUmakefile
	@ ${MAKE} -C Plugins	-f GNUmakefile -j 2

autodocs: all
	@ ${MAKE} -C Library	-f GNUmakefile autodoc

prepack:
	@ ${MAKE} -C Release	-f GNUmakefile clean

clean:
	@ ${MAKE} -C Plugins	-f GNUmakefile clean
	@ ${MAKE} -C Library	-f GNUmakefile clean
	@ ${MAKE} -C Devkit 	-f GNUmakefile clean

install: release
	@ ${MAKE}	-C Library	-f GNUmakefile install
	@ ${MAKE}	-C Plugins	-f GNUmakefile install
