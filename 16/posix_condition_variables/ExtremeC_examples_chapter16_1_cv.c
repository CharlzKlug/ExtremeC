#include <stdio.h>
#include <stdlib.h>

// The POSIX standard header for using pthread library
#include <pthread.h>

#define TRUE 1
#define FALSE 0
typedef unsigned int bool_t;

// A structure for keeping all the variables related
// to a shared state
typedef struct {
  // The flag which indicates whether 'A' has been printed or not
  bool_t done;
  // The mutex object protecting the critical sections
  pthread_mutex_t mtx;
  // The condition variable used to synchronize two threads
  pthread_cond_t cv;
} shared_state_t;

// Initializes the members of a shared_state_t object
void shared_state_init(shared_state_t *shared_state) {
  shared_state->done= FALSE;
  pthread_mutex_init(&shared_state->mtx, NULL);
  pthread_cond_init(&shared_state->cv, NULL);
}

// Destroy the members of a shared_state_t object
void shared_state_destroy(shared_state_t *shared_state) {
  pthread_mutex_destroy(&shared_state->mtx);
  pthread_cond_destroy(&shared_state->cv);
}

void* thread_body_1(void* arg) {
  shared_state_t* ss= (shared_state_t*)arg;
  pthread_mutex_lock(&ss->mtx);
  printf("A\n");
  ss->done= TRUE;
  // Signal the threads waiting on the condition variable
  pthread_cond_signal(&ss->cv);
  pthread_mutex_unlock(&ss->mtx);
  return NULL;
}

void* thread_body_2(void* arg) {
  shared_state_t* ss= (shared_state_t*)arg;
  pthread_mutex_lock(&ss->mtx);
  // Wait until the flag becomes TRUE
  while (!ss->done) {
    // Wait on the condition variable
    pthread_cond_wait(&ss->cv, &ss->mtx);
  }
  printf("B\n");
  pthread_mutex_unlock(&ss->mtx);
  return NULL;
}

int main() {
  // The shared state
  shared_state_t shared_state;

  // Initialize the shared state
  shared_state_init(&shared_state);

  // The thread handlers
  pthread_t thread1;
  pthread_t thread2;

  // Create new threads
  int result1=
    pthread_create(&thread1, NULL, thread_body_1, &shared_state);
  int result2=
    pthread_create(&thread2, NULL, thread_body_2, &shared_state);

  if (result1 || result2) {
    printf("The threads could not be created.\n");
    exit(1);
  }

  // Wait for the threads to finish
  result1= pthread_join(thread1, NULL);
  result2= pthread_join(thread2, NULL);

  if (result1 || result2) {
    printf("The threads could not be joined.\n");
    exit(2);
  }

  // Destroy the shared state and release the mutex
  // and condition variable objects
  shared_state_destroy(&shared_state);

  return 0;
}
