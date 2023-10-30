#include <unistd.h> // Needed for sleep function
#include <stdlib.h> // Needed for EXIT_SUCCESS

int main(int argc, char** argv) {
  // Infinite loop
  while (1) {
    sleep(1); // Sleep 1 second
  };
  return EXIT_SUCCESS;
}
