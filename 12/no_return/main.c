#include <stdio.h>

_Noreturn void main_loop() {
  int i= 0;
  while (1) {
    printf("%d\n", i);
    i+= 1;
  }
}

int main() {
  main_loop();
  return 0;
}
