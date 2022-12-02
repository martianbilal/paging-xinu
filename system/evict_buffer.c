#include <xinu.h>

int buf_length = 0;
int buf_head = 0;
int buf_tail = 0;
eentry_t *evict_buf[BUFF_SIZE];

status evict_buf_put(eentry_t *e1entry){

	if (buf_length == BUFF_SIZE) return SYSERR;
	evict_buf[(buf_tail++)%BUFF_SIZE] = e1entry;
	buf_length++;
	return OK;
}

eentry_t *evict_buf_get(void){

	if (buf_length == 0) return NULL;
	eentry_t *e1entry = evict_buf[(buf_head++)%BUFF_SIZE];
	buf_length--;
	return e1entry;
}

void reset_evict_buf(void){

	int i, tmp_len;
	eentry_t *tmp_buf[BUFF_SIZE];
	for (i = buf_head, tmp_len = 0; i < buf_tail; i++){
		if (evict_buf[i%BUFF_SIZE]->pid != -1)
			tmp_buf[tmp_len++] = evict_buf[i%BUFF_SIZE];
	}
	for (i = 0; i < tmp_len; i++){
		evict_buf[i] = tmp_buf[i];
	}
	buf_head = 0;
	buf_tail = tmp_len;
	buf_length = tmp_len;
}