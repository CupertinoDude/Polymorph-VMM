##
##		"Polymorph"
##
##	THIS IS THE EMULATOR SUPPORT INTERFACE FOR AMIGAOS4 ONLY!!!
##		AmigaOS 3.x/AROS/MorphOS use different sources for this!
##
.machine	"440"

##	extern ** function(**) calls
.xdef	ExecShort
.xdef	ExecOctet
.xdef	InterpretVector
.xdef	DynamicVector
.xdef	ObtainTableSegment
.xdef	ReleaseTableSegment
.xdef	GetTableSegmentAttr
.xdef	SetTableSegmentAttr
.xdef	ReadOctet
.xdef	WriteOctet
.xdef	ReadShortBE
.xdef	WriteShortBE
.xdef	ReadShortLE
.xdef	WriteShortLE
.xdef	ReadLongBE
.xdef	WriteLongBE
.xdef	ReadLongLE
.xdef	WriteLongLE

##
##  This is the plugin side bindings for C to use the EPROC interface to Polymorph.Library
##

ExecShort:
	blr;

ExecOctet:
	blr;

InterpretVector:
	blr;

DynamicVector:
	blr;

ObtainTableSegment:
	blr;

ReleaseTableSegment:
	blr;

GetTableSegmentAttr:
	blr;

SetTableSegmentAttr:
	blr;

ReadOctet:
	blr;

WriteOctet:
	blr;

ReadShortBE:
	blr;

WriteShortBE:
	blr;

ReadShortLE:
	blr;

WriteShortLE:
	blr;

ReadLongBE:
	blr;

WriteLongBE:
	blr;

ReadLongLE:
	blr;

WriteLongLE:
	blr;

##
