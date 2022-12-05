#define FRAME_SIZE 	(4096)
#define NCFRAME 	(1024)
#define NDFRAME 	(1000)
#define NPROCPAGE	(1024)
#define NE1FRAME 	(1024)
#define NE2FRAME 	(1048)
#define NGFRAME 	(1024)
#define NPTE 		(1024)
#define MEMLIMIT	(0x90400000)

#define pd_lsb12 	(0x3)
#define pt_lsb12 	(0x3)


extern uint32 a_addr, b_addr, c_addr, d_addr, e1_addr, e2_addr, f_addr, g_addr;
extern uint32 a_frames, b_frames, c_frames, d_frames, e1_frames, e2_frames, f_frames, g_frames;
extern uint32 pd_n_addr;
extern uint32 pt_a_addr, pt_g_addr, pt_m_addr;

extern int used_e1frames, used_e2frames;

extern qid16 framewait;