#include <unistd.h> // Needed for sleep function
#include <stdlib.h> // Needed for malloc function
#include <stdio.h>  // Needed for printf

int main(int argc, char** argv) {
  void* ptr = malloc(1024); // Allocate 1KB from heap
  printf("Address: %p\n", ptr);
  fflush(stdout); // To forse the print
  // Infinite loop
  while (1) {
    sleep(1); // Sleep 1 second
  };
  return 0;
}
