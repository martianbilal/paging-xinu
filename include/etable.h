typedef struct{
	pid32 pid;
	uint32 page_number;
	uint32 address;
}eentry_t;

extern	eentry_t e1table[];
extern	eentry_t e2table[];