#include <pthread.h>
#include <stdlib.h> // For malloc

/* struct shared_mutex_t; */

/* typedef struct { */
/*   struct shared_mem_t* shm; */
/*   pthread_mutex_t* ptr; */
/* } shared_mutex_t_l; */
struct shared_mutex_t_l;
typedef shared_mutex_t_l shared_mutex_t;

shared_mutex_t* shared_mutex_new();
void shared_mutex_delete(shared_mutex_t* obj);

void shared_mutex_ctor(shared_mutex_t* obj,
		       const char* name);
void shared_mutex_dtor(shared_mutex_t* obj);

pthread_mutex_t* shared_mutex_getptr(shared_mutex_t* obj);

void shared_mutex_lock(shared_mutex_t* obj);
void shared_mutex_unlock(shared_mutex_t* obj);

#if !defined(__APPLE__)
void shared_mutex_make_consistent(shared_mutex_t* obj);
#endif
