#include <stdio.h>

typedef struct {
  char first_name[32];
  char last_name[32];
  unsigned int birth_year;
} person_t;

typedef struct {
  person_t person;
  char student_number[16]; // Extra attribute
  unsigned int passed_credits; // Extra attribute
} student_t;

int main(int argc, char** argv) {
  student_t s = {{"James", "Bond", 1947}, "0123456789abcdef", 120};
  student_t* s_ptr = &s;
  person_t* p_ptr = (person_t*)&s;
  printf("Student pointer points to %p\n", (void*)s_ptr);
  printf("Person pointer points to %p\n", (void*)p_ptr);
  printf("First name: %s\n", p_ptr->first_name);
  return 0;
}
