#include <stdio.h>

typedef struct {
  union {
    struct {
      int x;
      int y;
    };
    int data[2];
  };
} point_t;

int main() {
  point_t p;
  p.x= 10;
  p.data[1]= -5;
  printf("Point (%d, %d) using an anonymous structure inside an anonymous union.\n", p.x, p.y);
  printf("Point (%d, %d) using byte array inside an anonymous union.\n", p.data[0], p.data[1]);
  return 0;
}
