# HTLL

HTLL is a low-level programming language that compiles directly to **x86-64 Linux assembly** via FASM, creating small, fast, and dependency-free statically linked binaries. It's built on the "Escape Programming" philosophy, which rejects the bloat and restrictions of modern toolchains in favor of absolute control.

## The Power of Minimalism: A Concrete Example

HTLL's simplicity translates directly to hyper-efficient executables.

*   A standard `Hello, World!` program in HTLL compiles to a **440-byte** assembly (`.s`) file. When assembled, the final **statically linked** binary file is only **255 bytes**.
*   A more complex program, like a full Bubble Sort algorithm, produces a final **statically linked** executable of just **1.1 kilobytes**.

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

## License

This project is licensed under the [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html).
