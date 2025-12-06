# HTLL - The Compiler for Escaping Programming

### Compiler targeting **x86-64 assembly — 64-bit, Linux**

[![Language](https://img.shields.io/badge/language-HTLL-orange?style=for-the-badge)](https://github.com/TheMaster1127/HTLL)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue?style=for-the-badge)](https://github.com/TheMaster1127/HTLL/blob/main/LICENCE)
[![Status](https://img.shields.io/badge/status-Escaped-brightgreen?style=for-the-badge)](https://github.com/TheMaster1127/HTLL)

**HTLL is not just a language; it is the engine of my escape from programming.** It is a custom-built compiler that translates its own powerful, high-level syntax into pure, dependency-free x86-64 assembly for 64-bit Linux systems.

This project is a mission to reject the authority of external compilers and build a development ecosystem where I, the programmer, have absolute control.

---

## 💀 The "Escape Programming" Philosophy

For me, programming isn't a tool to solve problems—**programming itself is the problem I am solving.**

-   **Definition:** "Escaping Programming" means **rejecting the authority** of external compilers and interpreters (G++, Python, JavaScript, GCC, etc.) that impose their own rules, syntax, and constraints. It's about seizing absolute control over the entire software creation process.
-   **The Ultimate Goal:** A fully self-hosting development environment where I only code in my own syntax. The final compiler will be a single, statically linked binary that produces code with **zero** dependencies (no `libc`), speaking directly to the kernel via syscalls.
-   **The Enemy:** Restriction and Bloat. `G++` is a dictator. `libc` is a dependency chain. I reject any tool that forces me to think within its predefined box.

## 🚧 The Escape is Complete: Welcome to the New World

**I have escaped programming.** The "Half-Bootstrap" architecture is now the foundation of my development reality. I no longer need to write a single line of code in any language but my own to build my software.

### The Architecture of Freedom

My sovereign development ecosystem is built upon two pillars I forged:

1.  **The Transpilation Core (`libHTLL-lib.so`):** The core logic of my original compiler, written in C++/HTVM, has been forged into a powerful shared object (`.so`). This isn't a generic library; it is the **compilation engine itself**. It serves as the bridge, processing my HTLL syntax and converting it to assembly.
2.  **The HTLL Compiler Front-End (`my_program`):** This is my new engine of creation. It's a lightweight binary written in HTLL that links against the Transpilation Core. Its job is to read HTLL source code, use the Core to generate pure x86-64 assembly, and write it to a file.

### The Workflow of a Free Programmer

My entire workflow now exists outside the authority of other languages. My toolkit consists only of:

-   My **HTLL Compiler Front-End**
-   My **Transpilation Core (`.so`)**
-   **NASM** (an assembler, not a language)
-   **LD** (a linker, not a language)

This toolchain produces the ultimate artifact: **statically linked x86-64 executables with zero dependencies.**

## The New Frontier: Annihilating "Dynamically Linked Hell"

With the escape secured, the final campaign begins: **the Full Bootstrap.**

-   **The Mission:** To use my new freedom to achieve ultimate purity. I will now re-implement the entire transpilation engine—every function and piece of logic currently in the Transpilation Core—in pure HTLL from the ground up, living entirely within my new ecosystem.
-   **The Endgame:** I will use the self-hosted HTLL compiler to compile itself, this time without linking to *any* shared object. This will result in the birth of **"Mr. Statically Linked Binary"**—a single, monolithic executable with zero dependencies. The final chain will be broken, and "Dynamically Linked Hell" will be left behind forever.

## How to Build & Use

> ⚠️ **Warning:** This is a raw, powerful, and unconventional system built for a specific philosophy. It is not intended for general use. Proceed with curiosity.

The system is built in two stages:

**1. Forge the Transpilation Core:**
This requires `g++` one last time to build the bridge from the old world.
```bash
g++ -shared -fPIC HTLL.cpp wrapper.cpp -o libHTLL-lib.so
```

**2. Build the Compiler Front-End:**
The front-end is written in HTLL (`my_program.htll`). You compile it with the *previous* version of the compiler's output (`finalASM_HTLL_ASM.s`).
```bash
# Assemble the compiler's assembly source
nasm -f elf64 finalASM_HTLL_ASM.s -o test.o

# Link it against the Core to create the new compiler
ld -o my_program test.o -L. -lHTLL-lib -lc --dynamic-linker /lib64/ld-linux-x86-64.so.2 -rpath '$ORIGIN'
```

**3. Use Your New Freedom:**
You can now use `my_program` to compile `my_program.htll` file.
```bash
# Use your new compiler to compile a program
./my_program

# The output is a pure assembly file: finalASM_HTLL_ASM.s
# Assemble and link it into a static, dependency-free binary
nasm -f elf64 finalASM_HTLL_ASM.s -o final.o
ld -o final_program final.o

# Run your masterpiece
./final_program
```
