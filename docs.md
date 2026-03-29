# The HTLL Documentation: A Guide to Zero-Bloat Systems Programming

## The Power of Minimalism

HTLL is not an academic exercise; it is a brutal, practical tool for creating hyper-efficient, multi-architecture software. It strips away the heavy abstractions of modern languages (like C's dependency on `libc`) and forces the programmer to interact intimately with memory, registers, and execution flow.

The result is absolute minimal footprint:
* A standard `Hello, World!` program in HTLL compiles to a **440-byte** x86-64 assembly (`.s`) file.
* When assembled, the final **statically linked** binary x86-64 file is only **255 bytes**.
* A complex program, like a full Bubble Sort algorithm, produces a final statically linked executable of just **1.1 kilobytes**.

HTLL targets **x86-64**, **AArch64 (ARM)**, and the **Oryx VM**.

---

## Table of Contents
0. [Rules](#rules)
1. [The "Types Are a Lie" Doctrine](#1-the-types-are-a-lie-doctrine)
2. [File Structure: Includes, Globals, and `main`](#2-file-structure-includes-globals-and-main)
3. [Variables, Scope, & The Double-Init Law](#3-variables-scope--the-double-init-law)
4. [Expressions & Bitwise Math (Strict Left-to-Right)](#4-expressions--bitwise-math-strict-left-to-right)
5. [Arrays: The Universal Byte Buffer](#5-arrays-the-universal-byte-buffer)
6. [Control Flow: Conditionals & Jumps](#6-control-flow-conditionals--jumps)
7. [Control Flow: Loops (Standard, Array, Infinite)](#7-control-flow-loops-standard-array-infinite)
8. [Functions: Declarations, Returns, & The Parameter Copy Law](#8-functions-declarations-returns--the-parameter-copy-law)
9. [Terminal & File I/O](#9-terminal--file-io)
10. [System Features: Command Line Args & Syscalls](#10-system-features-command-line-args--syscalls)
11. [Inline Assembly & Target Architecture Blocks](#11-inline-assembly--target-architecture-blocks)
12. [Function Parameters, Return Values & The Loop Return Ban](#13-function-parameters-return-values--the-loop-return-ban)
13. [Example: Bubble Sort](#13-example-bubble-sort)
14. [**HTLL Quirks & Gotchas: The Survival Guide**](#12-htll-quirks--gotchas-the-survival-guide)

---

# Rules

### 1. Data Types (The "Types are a Lie" Doctrine)
*   **Integers (`int`):**
    *   64-bit signed integer.
    *   **Global Scope Rule:** All variables defined anywhere (except function params) are **global** and initialized only once.
*   **Arrays (`arr`):**
    *   Universal dynamic byte buffers.
    *   Can hold ASCII text, integers, or raw data.
*   **The `rax` Register:**
    *   This is the volatile "working memory" of the compiler.
    *   **Behavior:** It is overwritten by almost every operation (`.size`, `.index`, math, etc.).
    *   **Rule:** Use the value in `rax` *immediately* or store it. Do not expect it to persist.

### 2. The Strict Syntax & Space Mandate
*   **Operator Spacing:** You **must** put a space *around* every single binary operator.
    *   *Correct:* `int x := 0`
    *   *Incorrect:* `int x:=0`
    *   *Correct:* `x += 10`
    *   *Incorrect:* `x+=10`
*   **Control Flow Spacing:** You **must** put a space between the keyword and the opening parenthesis/brace.
    *   *Correct:* `if (x = 1) {`
    *   *Incorrect:* `if(x=1){`
*   **Unary Exception:** `++` and `--` are the *only* operators that stick to the variable.
    *   *Correct:* `x++`
*   **Comments:** Require a preceding space if on the same line (e.g., `code ; comment`).

### 3. Math & Logic (LEFT TO RIGHT EXPRESSIONS)
*   **Rule:** **Expressions are limited. ONLY After assignment operators (`:=`, `+=`, etc.) and the `return` keyword.**
    *   *Here are all the operators:* `+`, `-`, `*`, `//`, `%`, `<<`, `>>`, `&`, `|`, `^`
    *   *Legal:* `z := x + y`
    *   *Legal:* `return x + y`
    *   *Illegal:* `z := x + (y * 5 + 7)`
    *   *Illegal:* `if (x + 1 > 5)`
    *   *Illegal:* `funcName(x, y + 5)`
    *   *Illegal:* `arrName.index i + 5`
*   **Allowed Math Operations:**
    *   `+=`, `-=`, `*=`, `//=`, `%=`, `<<=`, `>>=`, `&=`, `|=`, `^=`
    *   `++` (Increment), `--` (Decrement)
    *   **Assignment:** `:=`

### 4. Control Flow (Brackets & Limits)
*   **The 9-Level Limit:** You can only nest blocks (Loops/Ifs) up to **9 levels deep**.
*   **Loops:**
    *   **Syntax:** `Loop, <count> {`, `Loop, arrName.size {`, `Loop, rax {`, or `Loop {` (infinite).
    *   **Iterator:** `A_Index` (Always refers to the *innermost* active loop).
    *   **`continue`:** Skips to the next iteration of the innermost loop.
    *   **`break`:** Terminates the innermost loop.
*   **Conditionals:**
    *   **Syntax:** `if (<operand1> <operator> <operand2>) {`
    *   **Operators:** ` = `, ` != `, ` > `, ` < `, ` >= `, ` <= ` (Spaces required!).
    *   **Logic:** NO `else`. NO `and`. NO `or`. Use `goto` or nested `if` blocks.
*   **Jumps:**
    *   `togo <label>`: Defines a jump target. The label name must be unique.
    *   `goto <label>`: Unconditional jump to a previously defined `togo` label.

### 5. Array Methods & Manipulation
*   **`arradd <arr> <text>`:** Appends text. **NO QUOTES ALLOWED.** Trims trailing spaces.
*   **`.add <value>`:** Appends a raw 64-bit value or an ASCII code.
*   **`.pop`:** Removes the last element.
*   **`.clear`:** Resets array size to 0.
*   **`.size`:** Puts size into `rax`.
*   **`.index <i_val>`:** Puts value at index `i_val` into `rax`.
*   **`.set <idx>, <val>`:** Overwrites value at index.
*   **`.copy <src_arr>`:** Copies contents of source array.

### 6. Terminal I/O
*   **`print(<value>)`:** Prints integer value of a variable or register.
*   **`print("text")`:** Prints a double-quoted string literal.
*   **`print("")`:** Prints a newline (ASCII 10).
*   **`print_rax_as_char`:** Interprets `rax` as an ASCII code and prints the character.
*   **`input <dest_arr>, <prompt_arr>`:** Displays prompt, reads stdin into `dest_arr`.

### 7. File System I/O (Direct Syscalls)
*   **Read:** `fileread <dest_arr>, "file.txt"` or `fileread_arr <dest_arr>, <filename_arr>`
*   **Append:** `fileappend "file.txt", <source_arr>` or `fileappend_arr <filename_arr>, <source_arr>`
*   **Delete:** `filedelete "file.txt"` or `filedelete_arr <filename_arr>`

### 8. System Internals
*   **`main` Label:** Mandatory if functions are used (must be placed after definitions).
*   **`args_array`:** A special global array populated at startup with command-line arguments. Each argument is separated by a newline character (ASCII 10).
*   **`Sleep, <ms>`:** Pauses execution via a kernel sleep syscall.

### 9. Quirks & Gotchas (Mandatory Knowledge)
*   **No Booleans:** Use integers (1/0).
*   **Manual ASCII:** `arradd` strips spaces. Use `.add 32` to add a space to an array.
*   **Limited Expressions:** Never do math inside an `if` statement or a function call.
*   **The Parameter Copy Law:** When passing an array parameter to *another* function, you MUST copy it to a local array first.
*   **The Double-Init Law:** Local variables in functions retain their values. You MUST re-initialize them on every call if you need a clean state.
*   **Array Assignment:** The `:=` operator is ONLY used on an array to capture a function's return value. To copy one array to another, you MUST use `.copy`.

---

## 1. The "Types Are a Lie" Doctrine

In low-level computing, distinct data types like "string" or "boolean" don't truly exist. There are only bytes of data in memory. A "type" is simply how the programmer decides to interpret those bytes at runtime. HTLL enforces this truth.

* **Integers (`int`)**: 64-bit signed integers. Booleans do not exist; use `1` and `0`.
* **Arrays (`arr`)**: Universal, 0-indexed, dynamic byte buffers. You can fill them with ASCII values to act as "strings," or raw numbers to act as "lists." Each element is 64 bits.
* **No Negative Indices**: HTLL does not support negative numbers in its core logic design. Indices and lengths are strictly positive.

---

## 2. File Structure: Includes, Globals, and `main`

An HTLL file has a strict hierarchical structure.

1.  **Includes**: Must be at the very top.
2.  **Global Variables**: Must be declared before any functions or `main`.
3.  **Functions**: Defined next.
4.  **`main`**: The mandatory entry point if functions are present.

### Example 1: Basic Structure

```ahk
include "HTLL_Lib.htll"

; 1. Globals
int g_system_status := 1

; 2. Functions
func do_nothing() {
    return 0
}

; 3. Main Entry
main
    print("System starting...")
```

---

## 3. Variables, Scope, & The Double-Init Law

Variables in HTLL are strictly managed. Understanding scope and memory allocation is critical.

### Rule 1: The Global Prefix
Any variable declared outside a function or `main` is global. To prevent compiler errors, **all global variables MUST be prefixed with `g_` or `global_`**.

### Rule 2: Strict Declarations
You **cannot** evaluate expressions on the same line as a declaration. Declarations only accept a single numeric literal.
* **Illegal**: `int x := 5 + 5`
* **Legal**: `int x := 5` then on a new line: `x += 5`

### Rule 3: The Double-Initialization Law (CRITICAL)

Local variables inside `main` or a `func` are allocated statically. They are only zeroed out *once* when the program launches. If a function is called multiple times, its local variables will retain their values from the previous call. **To ensure a clean state, you MUST declare the variable and immediately re‑assign it.**

**❌ Without double‑init (unexpected behavior):**

```ahk
func increment_test() {
    int counter := 0   ; allocated once, initialised to 0 only at program start
    counter += 1
    print(counter)
}

main
    increment_test()   ; Prints 1
    increment_test()   ; Prints 2 (counter kept its value from last call)
```

**✅ With double‑init (correct behavior):**

```ahk
func increment_test() {
    int counter := 0   ; allocation (occurs once)
    counter := 0       ; DOUBLE-INIT: forces reset on every call
    counter += 1
    print(counter)
}

main
    increment_test()   ; Prints 1
    increment_test()   ; Prints 1 (because counter was reset)
```

---

## 4. Expressions & Bitwise Math (Strict Left-to-Right)

HTLL supports robust, multi-operand mathematical and bitwise expressions on assignments and return statements.

**CRITICAL RULE: No Parentheses Allowed.**
Expressions are evaluated strictly from **Left to Right**. Standard order of operations (PEMDAS) does not apply. `10 + 5 * 2` is evaluated as `(10 + 5) * 2 = 30`.

### Operators Available:
* **Math**: `+`, `-`, `*`, `//` (Floor Division), `%` (Modulo)
* **Bitwise**: `&` (AND), `|` (OR), `^` (XOR), `<<` (Left Shift), `>>` (Right Shift)
* **Compound**: `+=`, `-=`, `*=`, `//=`, `%=`, `&=`, `|=`, `^=`, `<<=`, `>>=`
* **Unary**: `++`, `--` (Must be directly attached: `x++`)

### Example: Left-to-Right Evaluation

```ahk
int calc := 0
calc := 0

; Evaluated as: (((10 + 5) - 2) * 3) // 2
calc := 10 + 5 - 2 * 3 // 2
print(calc) ; Prints 19
```

---

## 5. Arrays: The Universal Byte Buffer

Arrays are 0-indexed dynamic buffers.

*   **`arradd <arr> <text>`**: Appends text directly. **NO QUOTES ALLOWED.**
*   **`.add <val>`**: Appends a 64-bit integer or ASCII code (`.add 32` for space).
*   **`.pop`**: Removes the last element.
*   **`.clear`**: Empties the array.
*   **`.size`**: Calculates length and puts it into `rax`.
*   **`.index <idx>`**: Retrieves value at index and puts it into `rax`.
*   **`.set <idx>, <val>`**: Overwrites value at index.
*   **`.copy <src>`**: Deep copies from another array.

### Array Assignment Rule (CRITICAL)
The `:=` operator is **only** used to assign an array when capturing the return value of a function. To copy the contents of one existing array to another, you **must** use the `.copy` method. Direct assignment `arr1 := arr2` is illegal.

### Example: Array Math (Retrieving from RAX)
**Rule:** You must cache `rax` into a variable immediately after calling `.index` or `.size` before using it.

```ahk
arr numbers
numbers.add 50
numbers.add 150

numbers.index 1
int val := 0
val := rax ; Cache RAX immediately!

val += 50
print(val) ; Prints 200
```

---

## 6. Control Flow: Conditionals & Jumps

Conditionals require strict spacing around operators: ` = `, ` != `, ` > `, ` < `, ` >= `, ` <= `.
There is **no** `else`, `and`, or `or` keyword. You must use nested `if` statements or jumps.

**CRITICAL RULE:** You **cannot** use `rax` or array methods directly inside an `if ( )` statement.

### Jumps and Labels
HTLL uses `togo` and `goto` for non-linear control flow.
*   **`togo <label>`**: Defines a jump target. A label is a unique identifier that marks a line of code.
*   **`goto <label>`**: Performs an unconditional jump to the specified `togo` label.

### Example: Simulating "ELSE" (Goto/Togo)

```ahk
int status := 0
status := 0

if (status = 1) {
    print("Status is 1")
    goto skip_else
}

; This code runs only if the 'if' is false
print("Status is NOT 1") ; The 'else' block

togo skip_else
print("Done.")
```

---

## 7. Control Flow: Loops (Standard, Array, Infinite)

HTLL has a maximum nesting limit of 9 levels. The current iterator is always `A_Index` (0-indexed).
*   `Loop, <count> {` - Fixed iterations.
*   `Loop, <arr>.size {` - Automatic array iteration.
*   `Loop {` - Infinite loop (requires manual `break`).

The `break` and `continue` keywords only affect the **innermost** active loop.

### Example: Nested Loops & `continue`

```ahk
Loop, 3 {
    if (A_Index = 1) {
        continue ; Skips index 1 of the outer loop
    }
    print("Outer:")
    print(A_Index)
}
```

---

## 8. Functions: Declarations, Returns, & The Parameter Copy Law

Functions establish true local scope.

### Function Declaration and Parameters
The syntax for defining a function is:
`func [return_type] <name>([type] <param1>, [type] <param2>, ...)`

*   **Return Type**: `int` or `arr`. If omitted, the function cannot use the `return` keyword.
*   **Parameters**: Comma-separated list. Parameter type is optional; if omitted, it defaults to `int`.

### The `return` Statement
The `return` keyword sends a value back to the caller.
*   **Syntax**: `return <value_or_expression>`
*   **Rule**: Expressions are allowed after `return` and are evaluated strictly Left-to-Right.
*   A function declared with a return type (`int` or `arr`) **must** end with a `return` statement.

### THE PARAMETER COPY LAW (CRITICAL)
If your function receives an array as a parameter, and you want to pass that data into *another* function, you **must not** pass the parameter directly. You must `.copy` it to a newly declared local array, and pass the copy. This prevents the compiler from destroying the reference.

### Example 1: The Parameter Copy Law in Action

```ahk
func process_text(arr text) {
    print("Processing...")
}

func pass_text_down(arr input_param) {
    arr safe_copy
    safe_copy.copy input_param ; MUST BE COPIED!
    
    process_text(safe_copy)    ; SAFE TO PASS
}

main
    arr my_str
    arradd my_str Data
    pass_text_down(my_str)
```

### Example 2: Returning an Array

```ahk
func arr create_buffer() {
    arr result
    result.clear
    result.add 99
    return result
}

main
    arr my_res
    my_res := create_buffer() ; Legal use of := with an array
    my_res.index 0
    int v := 0
    v := rax
    print(v) ; Prints 99
```

---

## 9. Terminal & File I/O

* `print(<val>)`: Prints integer.
* `print("<literal>")`: Prints quoted string.
* `print("")`: Prints newline (ASCII 10).
* `input <dest>, <prompt>`: Reads stdin.

**File Syscalls:** `fileread`, `fileappend`, `filedelete` (append `_arr` to use array names instead of literals).

---

## 10. System Features: Command Line Args & Syscalls

* `args_array`: A reserved global array automatically populated with OS arguments separated by ASCII 10.
* `Sleep, <ms>`: Kernel pause.

---

## 11. Inline Assembly & Target Architecture Blocks

Embed native assembly or language bytecode. The compiler intelligently strips blocks that don't match the build target.
**Rule:** Delimiters (`___start <target>` / `___end <target>`) must be on their own completely empty lines.

### Example: OS-Specific Branches

```ahk
___start x86-64
    mov rdi, 1
___end x86-64

___start arm
    mov x0, #1
___end arm
```

---

## 12. Function Parameters, Return Values & The Loop Return Ban

### Parameters
- **`arr`** may appear as a type keyword in the parameter list to indicate an array parameter.  
- Integer parameters have **no type keyword**; they are just names.  
- **Examples:**
  ```
  func process(arr data)          ; array parameter
  func calc(a, b, arr buffer)     ; two ints, one array
  ```

### Return Values
- A function may be prefixed with **`arr`** to indicate it returns an array.
  - If no `arr` prefix is used, the function returns an integer via `rax`.
- To return a value, use `return <expr>` (simple expression or variable name).
- The returned value is placed in the `rax` register.
- **Capturing integer return:**  
  Call the function, then immediately assign `rax` to an integer variable.  
  *Declaration and assignment must be separate steps.*

  ```ahk
  func add(a, b) {
      return a + b
  }
  main
      int sum       ; declare first
      add(5, 3)
      sum := rax    ; assign after call
  ```

- **Capturing array return:**  
  Use the `arr` prefix in the function signature. Call the function and use the `:=` operator to assign the result to an array variable.  
  *Again, declare the array first, then assign separately.*

  ```ahk
  func arr make_buffer() {
      arr buf
      buf.add 42
      return buf
  }
  main
      arr my_buf       ; declare first
      my_buf := make_buffer()   ; assign after call
  ```

### THE LOOP RETURN BAN
- **Do not** place a `return` statement inside any loop (`Loop`, `Loop, <count>`, `Loop, <arr>.size`, infinite loop).
- **Why:** It compiles but will cause a **segmentation fault** at runtime.
- **Correct pattern:** Use a flag variable, break out of the loop, then return after the loop.

  **❌ Illegal (segfault):**

  ```ahk
  func find_zero(arr data) {
      Loop, data.size {
          data.index A_Index
          int v := 0
          v := rax
          if (v = 0) {
              return 1   ; FORBIDDEN You are going to get a segmentation fault most likely.
          }
      }
      return 0
  }
  ```

  **✅ Correct:**

  ```ahk
  func find_zero(arr data) {
      int found := 0
      found := 0
      int v := 0
      v := 0
      Loop, data.size {
          data.index A_Index
          v := rax
          if (v = 0) {
              found := 1
              break
          }
      }
      return found
  }
  ```

### Quick Reminder
- **The Parameter Copy Law** still applies: if you pass an array parameter to another function, you **must** copy it first with `.copy`.
- `rax` is clobbered by many operations; capture it immediately after a function call or array method.
- All variable declarations (`int`, `arr`) must be done on their own line before any assignment to them.

---

## 13. Example: Bubble Sort

This example demonstrates a complete bubble sort implementation that sorts a small array of integers. When compiled for x86‑64 Linux using FASM, the resulting statically linked executable is just **1.1 KiB** (1109 bytes).

```ahk
arr nums
nums.add 90
nums.add 11
nums.add 64

int n := 3 ; size

int i := 0
int temp := 0
int a := 0
int j := 0
int jp1 := 0
int limit := 0

Loop, n {
    limit := n - i - 1
    Loop, limit {
        j := A_Index
        jp1 := j + 1
        nums.index j
        temp := rax
        nums.index jp1
        a := rax
        if (temp > a) {
            nums.set j, a
            nums.set jp1, temp
        }
    }
    i += 1
}

Loop, n {
    nums.index A_Index
    print(rax)
}
```

This program creates an array with three integers, sorts them in ascending order, and prints each value on a new line. The binary size is a testament to HTLL’s zero‑bloat philosophy.

---

## 14. HTLL Quirks & Gotchas: The Survival Guide

If your code is failing to compile or causing a segmentation fault, you have violated one of these core laws:

*   **THE PARAMETER COPY LAW**: You cannot pass an array parameter directly into another function (`func1(arr p) { func2(p) }`). You MUST `.copy` it to a local array first.
*   **THE DOUBLE-INIT LAW**: You must re-initialize local variables (`x := 0`) inside functions if you need a clean state, otherwise they retain data from previous calls.
*   **STRICT SPACING**: You MUST put spaces around binary operators (`x += 5`) and after control keywords (`if (`, `Loop {`).
*   **NO EXPRESSIONS IN ARGS/CONDITIONALS**: `MyFunc(x + 1)` and `if (x + 1 > 5)` are illegal. Calculate the value into a variable first.
*   **NO PARENTHESES IN MATH**: Expressions are strictly Left-to-Right. `()` does not exist.
*   **CACHE `rax` IMMEDIATELY**: After `.size` or `.index`, store `rax` in a variable. Do not use `rax` directly in an `if` statement.
*   **`arradd` USES NO QUOTES**: `arradd my_arr Hello` is correct. To append a space, use `.add 32`.
*   **`togo` DEFINES A LABEL**: `togo my_label` creates a jump target. `goto my_label` jumps to it.
*   **ARRAY ASSIGNMENT IS FORBIDDEN**: You cannot write `arr1 := arr2`. Use `arr1.copy arr2`. The only time `:=` is used with an array is to receive a function's return value: `my_arr := func_that_returns_arr()`.
*   **GLOBALS NEED A PREFIX**: All top-level variables must start with `g_` or `global_`.
*   **NO `else`, `and`, `or`:** These constructs do not exist. Build equivalent logic using `goto` and nested `if` statements. This is intentional.
*   **NESTING IS FINITE:** You have 9 levels. If you need more, your code is too complex and must be refactored.
*   **int size is forbidden:** You cannot declare an integer or an array with the name size. This is going to break everything. Size is a reserved keyword. Do not use it as a variable name or an array name. Same with the str. That's also another keyword and a name and you must not use it.

---

In conclusion: **C is bloated.**

You have been told your whole life that C is the pinnacle of minimalism. This is a lie. The dependency on `libc` and the complexity of its toolchains introduce unnecessary bloat.

HTLL is an experiment in true minimalism. The trade-off is its specificity: it only targets a few architectures (x86-64 and AArch64) and one operating system (Linux). But within that domain, it offers unparalleled control and efficiency.

