#define NDFRAME 1000
#define NPTE 1024

typedef enum {pd=0, pt} type_t;

typedef struct {
	
	pid32 pid;
	type_t type;
	uint32 used_entries; //counter of occupied entries
	uint32 *address;

} d_info_t;


extern	d_info_t dtable[];