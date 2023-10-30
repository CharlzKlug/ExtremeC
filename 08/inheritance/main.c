#include <stdio.h> // For printf

typedef struct {
  char first_name[32];
  char last_name[32];
  unsigned int birth_year;
} person_t;

typedef struct {
  char first_name[32];
  char last_name[32];
  unsigned int birth_year;
  char student_number[16]; // Extra attribute
  unsigned int passed_credits; // Extra attribute
} student_t;

int main(void) {
  person_t james_bond= {"James", "Bond", 1946};
  printf("Name: %s, last name: %s, birth year: %d\n",
	 james_bond.first_name, james_bond.last_name, james_bond.birth_year);

  student_t albert_einstein = {"Albert", "Einstein", 1920, "11234", 344};
  printf("Student name: %s, last name: %s, birth year: %d, number: %s, credits: %d\n",
	 albert_einstein.first_name, albert_einstein.last_name, albert_einstein.birth_year,
	 albert_einstein.student_number, albert_einstein.passed_credits);
  return 0;
}
