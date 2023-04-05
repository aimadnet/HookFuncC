#include <dlfcn.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

typedef int (*original_function_t)(int);

// Pointer to the original function
static original_function_t original_function_ptr = NULL;

int hooked_function(int x) {
  printf("Hooked function called with x: %d\n", x);
  return 69;
}

void setup_hook(uintptr_t original_function_address) {
  original_function_ptr = (original_function_t)original_function_address;

  // Calculate the page boundaries and adjust the memory protection
  uintptr_t page_start =
      original_function_address & ~(sysconf(_SC_PAGESIZE) - 1);
  if (mprotect((void *)page_start, sysconf(_SC_PAGESIZE),
               PROT_READ | PROT_WRITE | PROT_EXEC) != 0) {
    perror("mprotect");
    return;
  }

  // Generate a trampoline to call the hooked_function (x86_64)
  uint64_t hooked_function_address = (uint64_t)(uintptr_t)hooked_function;
  uint8_t trampoline[] = {
      0x48,
      0xB8, // movabs rax, hooked_function_address
      (hooked_function_address >> 0) & 0xFF,
      (hooked_function_address >> 8) & 0xFF,
      (hooked_function_address >> 16) & 0xFF,
      (hooked_function_address >> 24) & 0xFF,
      (hooked_function_address >> 32) & 0xFF,
      (hooked_function_address >> 40) & 0xFF,
      (hooked_function_address >> 48) & 0xFF,
      (hooked_function_address >> 56) & 0xFF,
      0xFF,
      0xE0 // jmp rax
  };

  // Replace the original function's entry point with the trampoline
  memcpy((void *)original_function_address, trampoline, sizeof(trampoline));

  // Restore the memory protection
  if (mprotect((void *)page_start, sysconf(_SC_PAGESIZE),
               PROT_READ | PROT_EXEC) != 0) {
    perror("mprotect");
    return;
  }
}