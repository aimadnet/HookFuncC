#include "func.h"
#include "hooked.h"
#include <stdint.h>
#include <stdio.h>

int main() {
  setup_hook((uintptr_t)original_function);

  int x = 5;
  int result = original_function(x);
  printf("Result: %d\n", result);

  return 0;
}