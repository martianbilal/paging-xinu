typedef enum {pd=0, pt} type_t;

typedef struct{
	pid32 pid;
	type_t type;
	uint32 used_entries;
	uint32 address;
}dentry_t;

extern	dentry_t dtable[];