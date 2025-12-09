# HTLL

HTLL is a low-level programming language that compiles directly to **x86-64 Linux assembly** via FASM, creating small, fast, and dependency-free statically linked binaries. It's built on the "Escape Programming" philosophy, which rejects the bloat and restrictions of modern toolchains in favor of absolute control.

## The Power of Minimalism: A Concrete Example

HTLL's simplicity translates directly to hyper-efficient executables.

*   A standard `Hello, World!` program in HTLL compiles to a **440-byte** assembly (`.s`) file. When assembled, the final object (`.o`) file is only **255 bytes**.
*   A more complex program, like a full Bubble Sort algorithm, produces a final executable of just **1.1 kilobytes**.

This is the result of speaking directly to the metal without unnecessary abstraction layers.

## Core Features at a Glance

HTLL provides a powerful set of primitives for systems programming:

*   **Direct Memory Control:** Manipulate data using arrays as raw byte buffers.
*   **Simple Syntax:** A straightforward syntax inspired by the "least keystroke" philosophy.
*   **Core Functionality:** Includes variables, functions, loops, and conditionals.
*   **Built-in System Calls:** Direct access to file I/O (read, append, delete) and user input without external libraries.

### Full Documentation

For a complete guide, including detailed explanations of every feature, syntax rules, and extensive examples, please read the full documentation:

[**Read the Full HTLL Documentation**](https://github.com/TheMaster1127/HTLL/blob/main/docs.md)

---

## Requirements

*   **Architecture:** x86-64
*   **Operating System:** 64-bit Linux
*   **Compiler:** `g++` (for the initial compiler bootstrap)
*   **Assembler:** `fasm` (flat assembler)

**Installing FASM:**

*   **Arch Linux / Manjaro:** `sudo pacman -S fasm`
*   **Debian / Ubuntu:** `sudo apt install fasm`

---

## How to Use

The build process is separated into a one-time compiler setup and the process for your own programs.

### 1. Compiling the HTLL Compiler (One-Time Setup)

To build the `HTLL` compiler from source, follow these exact steps in order:

1.  **Compile the temporary bootstrap compiler:**
    ```bash
    g++ HTLL.cpp -o HTLL
    ```
2.  **Run it to generate the initial assembly:**
    ```bash
    ./HTLL HTLL.htll
    ```
3.  **Rename the output file:**
    ```bash
    mv finalASM_HTLL_ASM.s HTLL.s
    ```
4.  **Compile the C++ Backend Library:**
    ```bash
    g++ -shared -fPIC HTLL.cpp wrapper.cpp -o libHTLL-lib.so
    ```
5.  **Regenerate the assembly:**
    ```bash
    ./HTLL HTLL.htll
    ```
6.  **Assemble the Compiler:**
    ```bash
    fasm HTLL.s HTLL.o
    ```
7.  **Link the Compiler:**
    ```bash
    ld -o HTLL HTLL.o -L. -lHTLL-lib --dynamic-linker /lib64/ld-linux-x86-64.so.2 -rpath '$ORIGIN'
    ```

### 2. Compiling and Running Your Program

Once the compiler is built, use this simple workflow for your own `.htll` files:

1.  **Compile Source to Assembly:**
    ```bash
    ./HTLL my_program.htll
    ```
2.  **Assemble to a Static Executable:**
    ```bash
    fasm my_program.s
    ```
3.  **Run:**
    ```bash
    ./my_program
    ```

---

## 💀 The "Escape Programming" Philosophy

*   **Definition:** "Escaping Programming" means **rejecting the authority** of external compilers and interpreters (`g++`, Python, GCC, etc.) that impose their own rules, syntax, and constraints. It's about seizing absolute control over the entire software creation process.
*   **The Ultimate Goal:** A fully self-hosting development environment where I only code in my own syntax. The final compiler will be a single, statically linked binary that produces code with **zero** dependencies (no `libc`), speaking directly to the kernel via syscalls.
*   **The Enemy:** Restriction and Bloat. `g++` is a dictator. `libc` is a dependency chain. I reject any tool that forces me to think within its predefined box.

---

## Bootstrapping Notes

*   Currently, the HTLL compiler relies on a `.so` library for some built-in functions.
*   The ultimate goal is a full bootstrap, removing all external dependencies.
*   The future statically linked HTLL compiler will produce binaries under 100 KB with zero dependencies.

---

## License

This project is licensed under the [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html).