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

*   **architectures:** x86-64, AArch64
*   **Operating System:** 64-bit Linux
*   **Compiler:** `g++` (for the initial compiler bootstrap)
*   **Assembler:** `fasm` (flat assembler) and for AArch64 use `as` and `ld`

**Installing FASM:**

*   **Arch Linux / Manjaro:** `sudo pacman -S fasm`
*   **Debian / Ubuntu:** `sudo apt install fasm`

---

## How to Use

The build process is separated into a one-time compiler setup and the process for your own programs.

### 1. Compiling the HTLL Compiler (One-Time Setup)

To build the `HTLL` compiler from source, follow these exact steps in order:

1.  **Compile the compiler:**
    ```bash
    g++ HTLL.cpp -o HTLL
    ```

### 2. Compiling and Running Your Program

Once the compiler is built, use this simple workflow for your own `.htll` files:

## x86-64 Example

1.  **Compile Source to Assembly:**
    ```bash
    ./HTLL my_program.htll x86-64
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

## ARM (AArch64) Example

### HTLL source (`my_program.htll`)

```bash
./HTLL my_program.htll arm
```

This generates:

```
my_program.s
```

### Assemble (ARM)

```bash
as my_program.s -o my_program.o
```

### Link

```bash
ld my_program.o -o my_program
```

### Run

```bash
./my_program
```
---

## ORYX: Interpreted Assembly for HTLL

ORYX is an **assembly-like interpreted language** that HTLL can **compile your `.htll` code into**.

### Workflow

1. **Compile HTLL to ORYX assembly** (`.oryxir`):

```bash
./HTLL my_program.htll oryx
```

2. **Compile the ORYX runtime** (`oryxir.cpp`):

```bash
g++ oryxir.cpp -o oryxir
```

3. **Run the ORYX file**:

```bash
./oryxir my_program.oryxir
```

4. **Debug using the browser**:

* Open `index.html`
* Load your `.oryxir` file
* Step forward/backward, inspect and modify registers or memory

> ORYX is **interpreted**, not a binary. `oryxir` executes it, and `index.html` is the debugger.

---

## HT-Kernel: Ring 0 Execution for HTLL

HT-Kernel is a **minimal x86-64 kernel written in FASM assembly** that allows **HTLL-generated code to run directly in ring 0**.

It is designed for **bare-metal experimentation**, giving HTLL programs direct access to hardware without an operating system.

### Overview

* **Architecture:** x86-64 only  
* **Privilege Level:** Ring 0  
* **Language:** FASM assembly  
* **Purpose:** Execute HTLL-generated kernel-safe assembly  
* **OS:** None (bare metal) x86-64 ONLY

HT-Kernel is intentionally simple and experimental.

### HTLL → HT-Kernel Workflow

1. **Compile HTLL to kernel assembly**:
```bash
./HTLL ring0_test.htll x86-64-ring0
````

2. **Use the generated assembly output**:

* Copy the contents of the generated `.s` file
* Paste it into the HT-Kernel folder in main_draw.s and then run `./build.sh`

3. **Build the kernel**:

* Assemble the kernel using FASM
* Boot it (QEMU or real hardware)

### Example Programs

HTLL programs such as `ring0_test.htll` demonstrate:

* Direct screen drawing
* 80×50 text-mode graphics
* Simple game and visual demos
* A clock in UTC+2

### Repository

HT-Kernel is hosted here:

[https://github.com/TheMaster1127/HT-Kernel](https://github.com/TheMaster1127/HT-Kernel)

> HT-Kernel is experimental. No memory protection, no safety guarantees.

---

## License

This project is licensed under the [GNU General Public License v3.0 (GPLv3)](https://www.gnu.org/licenses/gpl-3.0.html).
