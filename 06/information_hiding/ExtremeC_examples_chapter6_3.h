#ifndef EXTREME_C_EXAMPLES_CHAPTER_6_3_H
#define EXTREME_C_EXAMPLES_CHAPTER_6_3_H

#include <unistd.h>

// The attribute structure with no disclosed attribute
struct list_t;

// Allocation function
struct list_t* list_malloc();

// Constructor and destructor functions
void list_init(struct list_t*);
void list_destroy(struct list_t*);

// Public behavior functions
int list_add(struct list_t*, int);
int list_get(struct list_t*, int, int*);
void list_clear(struct list_t*);
size_t list_size(struct list_t*);
void list_print(struct list_t*);

#endif
