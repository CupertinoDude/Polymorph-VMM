/*
**  FileSystem.h
**
**  ZetaFileSystem
*/
#include	<exec/types.h>

/*  FileSystem Metadata Definitions
*/
struct ZetaVirtualAddress
{
	uint32_t vdev;
	uint32_t asize;
	uint64_t offset;
};

typedef struct ZetaBlockPtr
{
	struct ZetaVirtualAddress bpDVA0;
	struct ZetaVirtualAddress bpDVA1;
	struct ZetaVirtualAddress bpDVA2;
	uint32_t bpFlags;
	uint16_t bpPsize;
	uint16_t bpLsize;
	uint64_t bpPad0;
	uint64_t bpPad1;
	uint64_t bpPad2;
	uint64_t bpBirthTXG;
	uint64_t bpFillCount;
	uint64_t bpChecksum0;
	uint64_t bpChecksum1;
	uint64_t bpChecksum2;
	uint64_t bpChecksum3;
}blkptr_t;

struct ZetaUberBlock
{
	uint64_t ub_Magic;
	uint64_t ub_Version;
	uint64_t ub_TXG;
	uint64_t ub_GUID_sum;
	uint64_t ub_TimeStamp;
	blkptr_t ub_BlockPtr;
};
#define	ZETA_UBERMAGIC_BE	0x00BAB10C
#define	ZETA_UBERMAGIC_LE	0x0cB1BA00

#define	ZETA_UBER_VERSION	0x1

struct ZetaPartition
{
	Reservation[8192];
	ZetaBootBlock[8192];
	NameValueStore[114688];
	struct ZetaUberBlock UberBlock[256];
};

#define	ZETA_METABLOCK		0x400000