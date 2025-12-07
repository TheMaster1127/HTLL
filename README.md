# HTLL

HTLL is a low-level language written in HTVM that compiles directly to x86-64 Linux  assembly via FASM. It allows creating statically linked binaries and provides a set of built-in features including arrays, loops, functions, I/O, and string manipulation.

---

## Table of Contents

1. [Overview](#overview)
2. [File Types](#file-types)
3. [Example Program](#example-program)
4. [Features](#features)

   * Variables
   * Functions
   * Loops
   * Conditionals
   * Arrays
   * String-like Operations
   * File I/O
   * User Input
5. [Comprehensive Feature Test](#comprehensive-feature-test)
6. [Debugging](#debugging)
7. [Compilation Workflow](#compilation-workflow)
8. [Bootstrapping Notes](#bootstrapping-notes)

---

## Overview

HTLL compiles high-level instructions directly to assembly using FASM. Programs are typically statically linked, lightweight, and fully independent. The compiler itself is currently dynamically linked but will eventually become fully bootstrapped.

---

## File Types

| File         | Description                                                     |
| ------------ | --------------------------------------------------------------- |
| `HTLL`       | Compiler executable (dynamically linked, ELF 64-bit).           |
| `my_program` | Example compiled program (statically linked, ELF 64-bit).       |
| `fasm`       | Assembler used for compilation (statically linked, ELF 64-bit). |
| `ld`         | GNU linker (dynamically linked, ELF 64-bit PIE).                |

---

## Example Program

**Source (`my_program.htll`):**

```htll
Loop, 5
    print("Hello HTLL")
endloop
```

**Output when compiled and run:**

```
Hello HTLL
Hello HTLL
Hello HTLL
Hello HTLL
Hello HTLL
```

---

## Features

### Variables

* Supports integer declaration (`int x := 0`) and arithmetic (`+=`, `-=`, `*=`, `++`, `--`).
* Example:

```htll
nint var1234 := 5
var1234 += 5
var1234--
```

### Functions

* Define with `func name(args) ... funcend`.
* Return values via `rax`.
* Example:

```htll
func add_numbers(num1, num2)
    int local_sum := 0
    local_sum += num1
    local_sum += num2
    return local_sum
funcend
```

### Loops

* `Loop, n ... endloop` syntax.
* Supports dynamic iteration based on variables or array sizes.

### Conditional Logic

* `if(condition) ... ifend` for branching logic.

### Arrays

* Declare with `arr name`.
* Methods:

  * `.add` – append an element
  * `.size` – get array size
  * `.index` – get element by index
  * `.set` – set element value
  * `.copy` – copy array
  * `.clear` – clear array
* Example:

```htll
numbers_arr.add i
numbers_arr.set 3, 99
```

### String-like Operations

* Use arrays to store character data.
* Methods:

  * `arradd` – append text
  * `print_rax_as_char` – print array element as character

### File I/O

* `fileappend` – append data to a file
* `fileread` – read file contents into array
* `filedelete` – delete file

### User Input

* `input array_name, prompt_array` reads input into an array.

---

## Comprehensive Feature Test

The following script tests all major HTLL features in one run. It covers arrays, loops, functions, conditionals, I/O, and more:

```htll
; Global variable definitions
arr numbers_arr
arr temp_arr
arr message_arr
arr user_input_arr
arr prompt_arr

int i := 0
int loop_counter := 0
int sum_result := 0

; Function example
func add_numbers(num1, num2)
    int local_sum := 0
    local_sum += num1
    local_sum += num2
    return local_sum
funcend

; MAIN EXECUTION
main

; Function call & printing
add_numbers(100, 200)
sum_result := rax
print(sum_result) ; Expected output: 300

; Populate array with loop
i := 0
loop_counter := 5
Loop, loop_counter
    numbers_arr.add i
    i++
loopend

; Array reading & conditionals
numbers_arr.size
Loop, rax
    numbers_arr.index A_Index
    if (rax != 1)
        if2 (rax != 3)
            print(rax)
        ifend2
    ifend
loopend

; Modify array
numbers_arr.set 3, 99

; Copy & clear arrays
temp_arr.copy numbers_arr
numbers_arr.clear

; User input & string operations
prompt_arr.clear
arradd prompt_arr Please enter your name:
prompt_arr.add 32
input user_input_arr, prompt_arr
message_arr.clear
arradd message_arr You entered:
message_arr.add 32

user_input_arr.size
Loop, rax
    user_input_arr.index A_Index
    message_arr.add rax
loopend
message_arr.add 10

message_arr.size
Loop, rax
    message_arr.index A_Index
    print_rax_as_char
loopend

; File I/O test
temp_arr.clear
arradd temp_arr This is a test of the file system.
temp_arr.add 10
fileappend "test_output.txt", temp_arr
fileappend "test_output.txt", message_arr
temp_arr.clear
fileread temp_arr, "test_output.txt"
i := 0
temp_arr.size
Loop, rax
    temp_arr.index i
    print_rax_as_char
    i++
loopend
filedelete "test_output.txt"

; Final test
print("Test Done")
```

---

## Compilation Workflow

1. Compile HTLL compiler:

```bash
g++ -shared -fPIC HTLL.cpp wrapper.cpp -o libHTLL-lib.so
```

2. Compile HTLL source to `.s`:

```bash
./HTLL my_program.htll && chmod 644 my_program.s
```

3. Assemble `.s` file:

```bash
fasm my_program.s my_program.o
```

4. Link object to executable:

```bash
ld my_program.o -o my_program
```

5. Run executable:

```bash
./my_program
```

---

## Bootstrapping Notes

* Currently, HTLL relies on a `.so` library for some built-in functions.
* Goal: full bootstrap, removing all external dependencies.
* Future statically linked HTLL compiler will produce binaries under 100 KB with zero dependencies.

---
