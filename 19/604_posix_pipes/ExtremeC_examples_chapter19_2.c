#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char** argv) {
  int fds[2];
  pipe(fds);

  int childpid = fork();
  if (childpid == -1) {
    fprintf(stderr, "fork error!\n");
    exit(1);
  }

  if (childpid == 0) {
    // Child closes the write file descriptor
    close(fds[1]);
    char buff[32];

    // Child reads from the read file descriptor
    fprintf(stdout, "CHILD: Reading from parent...\n");
    int num_of_read_bytes = read(fds[0], buff, 32);
    fprintf(stdout, "CHILD: Received from parent: %s\n", buff);
    
  } else {
    // Parent closes the read file descriptor
    close(fds[0]);
    char str[] = "Hello Son!";

    // Parent writes to the write file descriptor
    fprintf(stdout, "PARENT: Waiting for 2 seconds...\n");
    sleep(2);
    fprintf(stdout, "PARENT: Writing to son...\n");
    write(fds[1], str, strlen(str) + 1);

  }
  return 0;
}
