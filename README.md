# Function Hooking with Trampolines (x86_64)
This project demonstrates how to hook a function in C using trampolines on the x86_64 architecture.

## Description
The project consists of a simple C program that defines an original function and hooks it with a new implementation using a trampoline. The hooked function simply return 69.

## Requirements
* C compiler (e.g., GCC)

## How to Use
To use the project, simply clone the repository and compile the main.c file:

```bash
git clone https://github.com/aimadnet/HookFuncC.git
cd HookFuncC
make
```

Then, run the executable:

```bash
./main
```

Output without hook:

```bash
Result: 10
```

Output with hook:

```bash
Hooked function called with x: 5
Result: 69
```
