#define NCFRAME 	1024
#define NDFRAME 	1000
#define NE1FRAME 	1024
#define NE2FRAME 	1048
#define NGFRAME 	1024

#define NPTE 		1024

#define MEMLIMIT	0x90400000

typedef enum {pd=0, pt} type_t;

typedef struct{
	pid32 pid;
	type_t type;
	uint32 used_entries;
	uint32 address;
}d_info_t;


extern	d_info_t dtable[];