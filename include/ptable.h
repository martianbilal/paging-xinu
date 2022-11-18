typedef enum {vmem=-1, empty, e1, e2} loc_t;

typedef struct{
	loc_t loc;
	eentry_t* eentry;
}pentry_t;