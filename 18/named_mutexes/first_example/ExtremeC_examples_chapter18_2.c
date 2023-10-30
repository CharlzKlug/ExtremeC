/* How to compile: gcc ExtremeC_examples_chapter18_2.c -Wall -Wextra -Werror -lrt -lpthread */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h> // For using pthread_mutex_* functions

#define SHARED_MEM_SIZE 4

// Shared file descriptor used to refer to shared memory object
int shared_fd = -1;

// Shared file descriptor used to refer to the mutex's shared
// memory object
int mutex_shm_fd = -1;

// The pointer to the shared counter
int32_t* counter = NULL;

// The pointer to shared mutex
pthread_mutex_t* mutex = NULL;

void init_control_mechanism() {
  // Open the mutex shared memory
  mutex_shm_fd = shm_open("/mutex0", O_CREAT | O_RDWR, 0600);
  if (mutex_shm_fd < 0) {
    fprintf(stderr, "ERROR: Failed to create shared memory: %s\n",
	    strerror(errno));
    exit(1);
  }

  // Allocate and truncate the mutex's shared memory region
  if (ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) < 0) {
    fprintf(stderr, "ERROR: Truncation of mutex failed: %s\n",
	    strerror(errno));
    exit(1);
  }

  // Map the mutex's shared memory
  void* map = mmap(0, sizeof(pthread_mutex_t),
		   PROT_READ | PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
  if (map == MAP_FAILED) {
    fprintf(stderr, "ERROR: Mapping failed: %s\n",
	    strerror(errno));
    exit(1);
  }

  mutex = (pthread_mutex_t*)map;

  // Initialize the mutex object
  int ret = -1;
  pthread_mutexattr_t attr;

  if ((ret = pthread_mutexattr_init(&attr))) {
    fprintf(stderr, "ERROR: Failed to init mutex attrs: %s\n",
	    strerror(ret));
    exit(1);
  }

  if ((ret = pthread_mutexattr_setpshared(&attr,
					  PTHREAD_PROCESS_SHARED))) {
    fprintf(stderr, "ERROR: Failed to set the mutex attr: %s\n",
	    strerror(ret));
    exit(1);
  }

  if ((ret = pthread_mutexattr_destroy(&attr))) {
    fprintf(stderr, "ERROR: Failed to destroy mutex attrs: %s\n",
	    strerror(ret));
    exit(1);
  }
}

void shutdown_control_mechanism() {
  int ret = -1;
  if ((ret = pthread_mutex_destroy(mutex))) {
    fprintf(stderr, "ERROR: Failed to destroy mutex: %s\n",
	    strerror(ret));
    exit(1);
  }

  if (munmap(mutex, sizeof(pthread_mutex_t)) < 0) {
    fprintf(stderr, "ERROR: Unmapping the mutex failed: %s\n",
	    strerror(errno));
    exit(1);
  }

  if (close(mutex_shm_fd) < 0) {
    fprintf(stderr, "ERROR: Closing the mutex failed: %s\n",
	    strerror(errno));
    exit(1);
  }

  if (shm_unlink("/mutex0") < 0) {
    fprintf(stderr, "ERROR: Unlinking the mutex failed: %s\n",
	    strerror(errno));
    exit(1);
  }
}

void init_shared_resource() {
  // Open the shared memory object
  shared_fd= shm_open("/shm0", O_CREAT | O_RDWR, 0600);

  if (shared_fd < 0) {
    fprintf(stderr, "ERROR: Failed to create shared memory: %s\n",
	    strerror(errno));
    exit(1);
  }

  fprintf(stdout, "Shared memory is created with fd: %d\n",
	  shared_fd);
}

void shutdown_shared_resource() {
  if (shm_unlink("/shm0") < 0) {
    fprintf(stderr, "ERROR: Unlinking shared memory failed: %s\n",
	    strerror(errno));
    exit(1);
  }
}

void inc_counter() {
  usleep(1);
  pthread_mutex_lock(mutex); // Should check the return value.
  int32_t temp = *counter;
  usleep(1);
  temp++;
  usleep(1);
  *counter = temp;
  pthread_mutex_unlock(mutex); // Should check the return value.
  usleep(1);
}

int main() {

  // Parent process needs to initialize the shared resource
  init_shared_resource();

  // Parent process needs to initialize the control mechanism
  init_control_mechanism();

  // Allocate and truncate the shared memory region
  if (ftruncate(shared_fd, SHARED_MEM_SIZE * sizeof(char)) < 0) {
    fprintf(stderr, "ERROR: Truncation failed: %s\n",
	    strerror(errno));
    return 1;
  }
  fprintf(stdout, "The memory region is truncated.\n");

  // Map the shared memory and initialize the counter
  void* map= mmap(0, SHARED_MEM_SIZE, PROT_WRITE, MAP_SHARED, shared_fd, 0);
  if (map == MAP_FAILED) {
    fprintf(stderr, "ERROR: Mapping failed: %s\n",
	    strerror(errno));
    return 1;
  }
  counter= (int32_t*) map;
  *counter= 0;

  // Fork a new process
  pid_t pid= fork();
  if (pid) { // The parent process
    // Increment the counter
    inc_counter();
    fprintf(stdout, "The parent process sees the counter as %d.\n",
	    *counter);

    // Wait for the child process to exit
    int status= -1;
    wait(&status);
    fprintf(stdout, "The child process finished with status %d.\n",
	    status);
  } else { // The child process
    // Increment the counter
    inc_counter();
    fprintf(stdout, "The child process sees the counter as %d.\n",
	    *counter);
  }

  // Both processes should unmap shared memory region and close
  // its file descriptor
  fprintf(stdout, "Trying to unmap shared memory region...\n");
  if (munmap(counter, SHARED_MEM_SIZE) < 0) {
    fprintf(stderr, "ERROR: Unmapping failed: %s\n",
	    strerror(errno));
    return 1;
  }
  fprintf(stdout, "Unmapping finished.\n");

  fprintf(stdout, "Trying to close shared memory file...\n");
  if (close(shared_fd) < 0) {
    fprintf(stderr, "ERROR: Closing  shared memory fd filed: %s\n",
	    strerror(errno));
    return 1;
  }
  fprintf(stdout, "Closing shared memory file finished.\n");
  
  // Only parent process needs to shutdown the shared resource
  /* if (pid) { */
  /*   shutdown_shared_resource(); */
  /* } */

  // Only parent process needs to shut down the shared resource
  // and the employed control mechanism
  if (pid) {
    shutdown_shared_resource();
    shutdown_control_mechanism();
  }
  
  return 0;
}
