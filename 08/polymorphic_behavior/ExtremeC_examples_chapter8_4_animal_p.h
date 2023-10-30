#ifndef EXTREME_C_EXAMPLES_CHAPTER_8_4_ANIMAL_P_H
#define EXTREME_C_EXAMPLES_CHAPTER_8_4_ANIMAL_P_H

// The function pointer type needed to point to
// different morphs of animanl_sound
typedef void (*sound_func_t)(void*);

// Forward declaration
typedef struct {
  char* name;
  // This member is a pointer to the function which
  // performs the actual sound behavior
  sound_func_t sound_func;
} animal_t;

#endif
