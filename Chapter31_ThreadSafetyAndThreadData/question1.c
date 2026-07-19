#include<stdio.h>
#include<pthread.h>
typedef struct {
	bool called;
	pthread_mutex_t mutex;
}one_time_t;
void one_time_init( one_time_t *control,void (*init)(void)) {
	pthread_mutex_lock(&control->mutex);
	if (!control->called) {
		init();
		control->called=true;
	}
	pthread_mutex_unlock(&control->mutex);
	return;
}
