# HTLL - A Low-Level Language and Emulator  

HTLL is a project where I created my own assembly language and a compiled language that converts to that assembly. The assembly code is then interpreted and executed by an emulator. The purpose of HTLL is to explore low-level programming concepts and create a fun, minimal architecture with strict limitations.  

## How It Works  

1. Write HTLL code in a `.htll` file (e.g., `test.htll`).  
2. Use `HTLL.exe` to compile the `.htll` file into `.htasm` (HTLL assembly).  
3. Run the `.htasm` file through `CPU-ASM-EMULATOR.exe` to execute the program.  

## Features  

- **Custom Assembly Language**: HTLL compiles to `.htasm`, which follows its own instruction set.  
- **Basic Operations**: Supports addition, multiplication, and subtraction (no division).  
- **Limited Data Types**: Only positive integers—no negative numbers or floating points.  
- **Loops**: Includes `Loop` with a value and an index.  
- **Conditionals**: Supports `if` statements but no `else` or `else if`.  
- **Subroutines**: Functions don’t exist, but subroutines can be called.  
- **Instruction Limit**: Programs are limited to 2048 instructions.  

## Example HTLL Code (`test.htll`)  

```plaintext
func code() {
    var1 := 5
    Loop, i := 0, 0 {
        var1--
        if (var1 = 0) {
            break
        }
    }
    if (var1 = 0) {
        var1 := 5 ; Comment
        print(var1); Comment
    }
}
code()
Loop, i2 := 1, 7 {
    print(i2)
    if (i2 = 5) {
        break
    }
    if (i2 != 5) {
        code()
    }
}
```

## Compilation and Execution  

1. Compile the HTLL file:  
   ```shell
   HTLL.exe test.htll
   ```  
   This generates `test.htasm`.  

2. Run the compiled assembly code using the emulator:  
   ```shell
   CPU-ASM-EMULATOR.exe test.htasm
   ```  

## Limitations  

- **No `continue` statement**  
- **No `else` or `else if`**  
- **Only positive integers** (no negatives or floats)  
- **No division (only `+`, `-`, `*`)**  
- **Global variables only** (no local scoping)  
- **Max 2048 instructions per program**  

## Purpose  

HTLL was built purely for fun as an experiment in low-level programming and creating an emulator-based architecture. It is not meant for practical use but serves as an interesting look into assembly-like language design and execution.  

---
