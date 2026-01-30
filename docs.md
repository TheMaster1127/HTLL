# The HTLL Documentation: A Guide to Zero-Bloat Programming

### The Power of Minimalism: A Concrete Example

HTLL is not an academic exercise; it is a practical tool for creating hyper-efficient software. Its simplicity translates directly to size and performance.

*   A standard `Hello, World!` program in HTLL compiles to a **440-byte** x86-64 assembly (`.s`) file. When assembled, the final **statically linked** binary x86-64 file is only **255 bytes**.
*   A more complex program, like a full Bubble Sort algorithm, produces a final **statically linked** executable x86-64 of just **1.1 kilobytes**.

This is the result of rejecting unnecessary layers of abstraction.

---

## Table of Contents

0.  [Rules](#rules)
1.  [The 'Types Are a Lie' Doctrine](#1-the-types-are-a-lie-doctrine)
2.  [The `main` Entry Point: Rules of Execution](#2-the-main-entry-point-rules-of-execution)
3.  [Variables: 64-bit Integers](#3-variables-64-bit-integers)
4.  [Arrays: The Universal Byte Buffer](#4-arrays-the-universal-byte-buffer)
5.  [Terminal I/O: Printing and Input](#5-terminal-io-printing-and-input)
6.  [Control Flow: Loops](#6-control-flow-loops)
7.  [Control Flow: Conditionals](#7-control-flow-conditionals)
8.  [Control Flow: Goto](#8-control-flow-goto)
9.  [Functions: Scope, Parameters, and Variables](#9-functions-scope-parameters-and-variables)
10. [The Array Parameter Solution: The Buffer Pattern](#10-the-array-parameter-solution-the-buffer-pattern)
11. [File System Operations](#11-file-system-operations)
12. [System Commands & Features](#12-system-commands--features)
13. [Syntax Reference & Flexibility](#13-syntax-reference--flexibility)
14. [Comprehensive All-In-One Example](#14-comprehensive-all-in-one-example)
15. [**HTLL Quirks & Gotchas: A Mandatory Review**](#15-htll-quirks--gotchas-a-mandatory-review)

---

# Rules

### 1. Data Types (The "Types are a Lie" Doctrine)
*   **Integers (`int` / `nint`):**
    *   64-bit signed integers.
    *   `int` and `nint` are functionally identical.
    *   **Global Scope Rule:** All variables defined anywhere (except function params) are **global** and initialized only once.
*   **Arrays (`arr`):**
    *   Universal dynamic byte buffers.
    *   Can hold ASCII text, integers, or raw data.
    *   **NOT** passable as function parameters.
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

### 3. Math & Logic (NO EXPRESSIONS)
*   **Rule:** **Expressions do not exist.** You cannot perform math and assignment in the same step unless it is a compound assignment.
    *   *Illegal:* `z := x + y`
    *   *Illegal:* `if (x + 1 > 5)`
*   **The "Line-by-Line" Workflow:** To add two numbers into a third variable, you must do it in steps:
    ```ahk
    z := x
    z += y
    ```
*   **Allowed Math Operations:**
    *   `+=` (Add to variable)
    *   `-=` (Subtract from variable)
    *   `*=` (Multiply variable)
    *   `++` (Increment)
    *   `--` (Decrement)
*   **Assignment:** `:=`

### 4. Control Flow (Brackets & Limits)
*   **The 9-Level Limit:** You can only nest blocks (Loops/Ifs) up to **9 levels deep**.
    *   This is a hard limit of the parser stack.
*   **Loops:**
    *   **Syntax:** `Loop, <count> {` or `Loop, rax {`
    *   **Termination:** `}`
    *   **Iterator:** `A_Index` (Always refers to the *innermost* active loop).
    *   **`continue`:** Skips the rest of the current iteration and jumps to the next.
    *   **`break`:** Immediately terminates the loop.
*   **Conditionals:**
    *   **Syntax:** `if (<operand1> <operator> <operand2>) {`
    *   **Termination:** `}`
    *   **Operators:** ` = `, ` != `, ` > `, ` < `, ` >= `, ` <= ` (Spaces required!).
    *   **Logic:** NO `else`. NO `and`. NO `or`. You must use `goto` or nested `if` blocks to simulate this.
*   **Jumps:**
    *   `togo <label>`: Defines a jump target.
    *   `goto <label>`: Unconditional jump.

### 5. Array Methods & Manipulation
*   **`arradd <arr> <text>`:**
    *   Appends text to the array.
    *   **NO QUOTES ALLOWED.** (e.g., `arradd mybuf Hello World`).
    *   **TRIMS SPACES:** Trailing spaces are automatically removed.
*   **`.add <value>`:** Appends a raw 64-bit value or an ASCII code (e.g., `.add 32` for a space).
*   **`.pop`:** Removes the last element from the array.
*   **`.clear`:** Resets the array size to 0.
*   **`.size`:** Calculates the size and puts it into `rax`.
*   **`.index <i_val>`:** Retrieves value at index `i_val` and puts it into `rax`.
*   **`.set <idx>, <val>`:** Overwrites the value at a specific index.
*   **`.copy <src_arr>`:** Copies contents of source array to destination array.

### 6. Functions & Scope
*   **Definition:** `func <name>(param1, param2) {`
*   **Termination:** `}`
*   **Parameters:** These are the **ONLY** local variables in the entire language. They are pushed to the stack.
*   **No Nesting:** You cannot define a function inside another function.
*   **The Anti-Pattern:** Do **not** define variables (`int x := 0`) inside a function. They will be global and only initialize once.
*   **The Buffer Pattern:** Since you cannot pass arrays as parameters, you must agree on a global `arr` to use as a shared data buffer between the caller and the function.

### 7. Terminal I/O
*   **`print(<value>)`:** Prints integer value of a variable or register.
*   **`print("text")`:** Prints a double-quoted string literal.
*   **`print("")`:** Prints a newline (ASCII 10).
*   **`print_rax_as_char`:** Interprets the value in `rax` as an ASCII code and prints the corresponding character.
*   **`input <dest_arr>, <prompt_arr>`:** Displays the text in `prompt_arr`, then reads from standard input into `dest_arr`.

### 8. File System I/O (Direct Syscalls)
*   **Read:**
    *   `fileread <dest_arr>, "file.txt"` (Literal filename)
    *   `fileread_arr <dest_arr>, <filename_arr>` (Filename in an array)
*   **Append:**
    *   `fileappend "file.txt", <source_arr>`
    *   `fileappend_arr <filename_arr>, <source_arr>`
*   **Delete:**
    *   `filedelete "file.txt"`
    *   `filedelete_arr <filename_arr>`

### 9. System Internals
*   **`main` Label:**
    *   Mandatory if functions are used (must be placed after definitions).
    *   Optional if writing a simple top-down script.
*   **`args_array`:**
    *   A special global array automatically populated at startup.
    *   Contains command-line arguments.
    *   **Separator:** Each argument is separated by a newline character (ASCII 10) inside the array.
*   **`Sleep, <ms>`:** Pauses execution via a kernel sleep syscall.

### 10. Quirks & Gotchas (Mandatory Knowledge)
*   **No Booleans:** True/False must be handled via integers (1/0).
*   **Manual ASCII:** To add a space to a string builder, you must use `.add 32`. `arradd` will strip it.
*   **Stack Management:** The parser manages the stack for you, but you must respect the 9-level nesting limit.
*   **No Expressions:** Never try to do math inside an `if` statement or an assignment. One operation per line.


---

### 1. The 'Types Are a Lie' Doctrine

In low-level computing, distinct data types like "string" or "boolean" don't truly exist. There are only bytes of data. A "type" is simply how you decide to *interpret* those bytes. HTLL is built on this truth.

The `arr` type is a raw buffer of bytes. It can function as a "string" if you fill it with ASCII values and print them as characters. It can function as a list of "integers" if you fill it with numbers and perform arithmetic. You, the programmer, are in complete control of data representation.

**Example 1: Dual-purpose buffer**
```ahk
arr buffer
arradd buffer Some text ; Use as a "string"
buffer.clear
buffer.add 1991         ; Now use as a list of numbers
```

**Example 2: Populating with character literals**
You can also build up text using single-quoted character literals.
```ahk
arr greeting
greeting.add 'H'
greeting.add 'e'
greeting.add 'l'
greeting.add 'l'
greeting.add 'o'
; The array now holds the ASCII values [72, 101, 108, 108, 111]
```

---

### 2. The `main` Entry Point: Rules of Execution

The `main` label tells the compiler where your program's execution begins. Its usage has specific rules:

1.  **If you have no `func` definitions:** The `main` label is **optional**. The compiler will execute from the top of the file. You can still add it for clarity if you wish.
2.  **If you have `func` definitions:** The `main` label is **mandatory**. You must place it after all your global variable and function definitions to mark the start of execution.

---

### 3. Variables: 64-bit Integers

HTLL supports 64-bit integers. Following the global scope rule, they should be defined at the top of your file for clarity.

**Types:** `int` and `nint` (functionally identical).

**Syntax:** No spaces are allowed between a variable name and the `++` or `--` operators.

```ahk
; --- Declaration and Initialization ---
int counter := 0
int value := 50

; --- Operations ---
value += 10  ; value is now 60
value -= 5   ; value is now 55
value *= 2   ; value is now 110
value++      ; value is now 111
value--      ; value is now 110
counter := value ; Assignment
```

---

### 4. Arrays: The Universal Byte Buffer

Arrays are the core data structure in HTLL. They are dynamically-sized buffers that hold 64-bit values.

**Handling Spaces with `arradd`**
The `arradd` command is designed for efficiency and trims any trailing spaces from its input. This means you cannot add a space at the end of a line with it. To add a space or other specific characters, you must use the `.add` method with the character's ASCII value. The value for a space is `32`.

**Example: Creating a spaced prompt**
```ahk
arr prompt
; Correct way to create "Name: "
arradd prompt Name:
prompt.add 32 ; This adds the trailing space
```

---

### 5. Terminal I/O: Printing and Input

*   `print(<value>)`: Prints the numeric value of a variable or number.
*   `print("text")`: Prints a double-quoted string literal.
*   `print("")`: This is the standard method for printing a newline character.
*   `print_rax_as_char`: Interprets the numeric value in `rax` as an ASCII code and prints the corresponding character.
*   `input <dest_arr>, <prompt_arr>`: Displays the text in `prompt_arr`, then reads from standard input into `dest_arr`.

---

### 6. Control Flow: Loops

Loops are **zero-indexed** and can be nested up to 9 levels (`Loop`, `Loop2`, ..., `Loop9`). The number (`2`, `3`, etc.) is only for the parser to keep track of nesting; it does not change the behavior.

**Syntax:** `Loop, <count>`

**CRITICAL:** The current iteration index is **always** available in the special variable `A_Index`. There is no `A_Index2` or `A_Index3`. This means if you have nested loops, `A_Index` will refer to the index of the innermost loop currently executing.

**Example 1: Basic zero-indexed loop**
```ahk
; This will print 0, 1, 2, 3, 4
Loop, 5
    print(A_Index)
endloop
```

**Example 2: Basic counted loop**
```ahk
; Prints 0, 1, 2
Loop, 3
    print(A_Index)
endloop
```

**Example 3: Iterating over an array**
```ahk
arr data_arr
data_arr.add 11
data_arr.add 22
data_arr.add 33

data_arr.size ; Place size (3) into rax
Loop, rax
    data_arr.index A_Index
    print(rax)
endloop
```

**Example 4: Nested Loops and `A_Index`**
```ahk
Loop, 2 ; Outer loop from 0 to 1
    print("Outer:")
    print(A_Index) ; This A_Index belongs to the outer loop
    
    Loop2, 3 ; Inner loop from 0 to 2
        print("  Inner:")
        print(A_Index) ; This A_Index belongs to the inner loop
    endloop2
endloop
```

---

### 7. Control Flow: Conditionals

Conditional logic requires parentheses and can also be nested up to 9 levels (`if`, `if2`, ..., `if9`).

**Operators:** `=`, `!=`, `>`, `<`, `>=`, `<=`

**Example 1: Simple check**
```ahk
int val := 10
if (val = 10)
    print("Value is 10.")
ifend
```

**Example 2: Simulating an `else` block**
HTLL does not have an `else` keyword. You achieve this with `goto`.
```ahk
int access_level := 5
if (access_level > 8)
    print("Admin access granted.")
    goto end_check
ifend
print("Standard access.")
togo end_check
```

---

### 8. Control Flow: `goto`

`goto` provides raw control over the execution path by jumping to a defined label.

*   `togo <label_name>`: Defines a jump target.
*   `goto <label_name>`: Jumps execution to the label.

**Example: A `goto`-based loop**
```ahk
int counter := 0
togo loop_start
    counter++
    print(counter)
    if (counter < 3)
        goto loop_start
    ifend
print("Loop finished.")
```

---

### 9. Functions: Scope, Parameters, and Variables

#### Function Parameters: True Local Scope
Function parameters are the **only** local variables in HTLL. They are pushed onto the stack when the function is called and exist only for the duration of that function's execution.

**Shadowing:**
HTLL supports parameter shadowing. If you define a function parameter with the same name as a global variable, the compiler automatically isolates the parameter. Inside the function, that name refers to the local stack value. Outside, it refers to the global variable.

**Example: Safe Shadowing**
```ahk
int x := 10  ; Global variable

func my_func(x) ; This 'x' is a local parameter, shadowing the global 'x'
    x += 5     ; This modifies the local 'x' on the stack (20 + 5)
    print(x)   ; Prints 25
funcend

main
    my_func(20) ; We pass 20 to the function.
    print(x)    ; Prints 10. The global 'x' was never touched.
```

#### Defining Variables Inside Functions: The Anti-Pattern
**Do not define variables inside functions (`int i := 0`).** While the syntax is allowed, it does not behave as you might expect from other languages. Due to HTLL's global nature, the variable is created globally and initialized *only once* when the compiler first sees it. It will **not** be reset on subsequent function calls.

**The Wrong Way (Bugged Logic):**
```ahk
func counter_broken()
    int i := 0 ; This line only truly runs once during compilation!
    i++
    print(i)
funcend

main
    counter_broken() ; Prints 1
    counter_broken() ; Prints 2, because 'i' was not reset to 0
    counter_broken() ; Prints 3
```

**The Correct Way (Declare Globally, Reset Locally):**
```ahk
int i := 0 ; 1. Declare the variable in the global scope.

func counter_correct()
    i := 0 ; 2. Reset the variable to its starting value inside the function.
    i++
    print(i)
funcend

main
    counter_correct() ; Prints 1
    counter_correct() ; Prints 1
    counter_correct() ; Prints 1
```

---

### 10. The Array Parameter Solution: The Buffer Pattern

**CRITICAL RULE REVISITED:** You **cannot** pass an array as a parameter to a function.

This is a core design choice. The correct way to have a function process array data is to use the "Global Buffer Pattern." You and the function agree on a global array to use as a shared workspace. The caller places data into the buffer, and the function reads from it.

**Example: Correctly processing array data**
```ahk
arr my_data
arr shared_buffer ; This is the agreed-upon global buffer

; This function is designed to work ONLY with 'shared_buffer'
func process_data_in_buffer()
    print("Processing data...")
    shared_buffer.size
    Loop, rax
        shared_buffer.index A_Index
        rax++ ; Increment the value
        print(rax)
    endloop
funcend

main
    ; 1. Load your primary data
    my_data.add 100
    my_data.add 200
    
    ; 2. Copy the data to the shared buffer before calling
    shared_buffer.copy my_data
    
    ; 3. Call the function, which now operates on the buffer
    process_data_in_buffer()
```

---

### 11. File System Operations

HTLL provides direct syscalls for file I/O. For each operation, there are two versions: one that takes a double-quoted string literal for the filename, and a more flexible `_arr` version that takes an array containing the filename.

*   `fileread <dest_arr>, "file.txt"` / `fileread_arr <dest_arr>, <filename_arr>`
*   `fileappend "file.txt", <source_arr>` / `fileappend_arr <filename_arr>, <source_arr>`
*   `filedelete "file.txt"` / `filedelete_arr <filename_arr>`

**Example: Reading, Writing, and Deleting a file**
```ahk
arr content_buffer
arr filename_buffer

main
    arradd filename_buffer temp_file.txt
    arradd content_buffer This is a test.
    content_buffer.add 10 ; Add a newline

    ; Write the content to the file
    print("Writing to file...")
    fileappend_arr filename_buffer, content_buffer

    ; Clear the buffer and read the file back
    content_buffer.clear
    print("Reading from file...")
    fileread_arr content_buffer, filename_buffer

    ; Print the contents
    content_buffer.size
    Loop, rax
        content_buffer.index A_Index
        print_rax_as_char
    endloop

    ; Clean up
    print("Deleting file...")
    filedelete_arr filename_buffer
```

---

### 12. System Commands & Features

#### Command Line Arguments (`args_array`)
HTLL automatically populates the special global array `args_array` at startup with any command-line arguments you provide. **Do not declare this array yourself.**

Internally, HTLL loops through the arguments provided by the operating system, unpacks them, and appends them to the `args_array` one by one, adding a newline character (ASCII `10`) after each argument to act as a separator.

**Example: Displaying arguments**
```ahk
; To run from terminal: ./my_program arg1 "second arg" third

main
    print("--- Arguments Received ---")
    args_array.size
    if (rax = 0)
        print("None.")
        goto args_done
    ifend
        
    Loop, rax
        args_array.index A_Index
        if (rax = 10)
            print("") ; A newline separates each argument
            continue
        ifend
        print_rax_as_char
    endloop
    
    togo args_done
```

#### Sleep
The `Sleep, <ms>` command pauses execution via a kernel syscall for the specified number of milliseconds.

**Example: Timed execution**
```ahk
print("Step 1")
Sleep, 1000 ; Pause for 1 second (1000 ms)
print("Step 2")
```

---

### 13. Syntax Reference & Flexibility

*   **Block Enders:** Blocks can be closed with two styles. They are interchangeable.
    *   `if` -> `ifend` or `endif`
    *   `loop` -> `endloop` or `loopend`
    *   `func` -> `funcend` or `endfunc`
*   **Nesting Limit:** Loops and conditionals can be nested up to 9 levels deep.
*   **Loop Iterator:** The loop variable is **always** `A_Index`.

---

### 14. Comprehensive All-In-One Example

This single program demonstrates the correct usage of globals, functions, the buffer pattern, I/O, loops, and conditionals to perform a series of tasks.

```ahk
;#######################################################
;# HTLL All-In-One Demonstration
;#######################################################

; --- Global Data & Buffers ---
arr log_content_buffer  ; Shared buffer for logging functions
arr filename_buffer     ; Holds the filename for I/O operations
arr number_list         ; Primary data array for sorting
arr user_input          ; Buffer for stdin
arr prompt              ; Buffer for user prompt text

int swapped := 0
int i := 0
int n := 0
int item1 := 0
int item2 := 0

; --- Function to sort the global 'number_list' via Bubble Sort ---
func sort_number_list()
    print("--- Sorting List ---")
    togo sort_start_label
        swapped := 0
        number_list.size
        n := rax
        n-- ; Adjust for zero-based index

        Loop, n
            i := A_Index
            number_list.index i
            item1 := rax

            i++
            number_list.index i
            item2 := rax

            if (item1 > item2)
                number_list.set A_Index, item2
                number_list.set i, item1
                swapped := 1
            ifend
        endloop
        if (swapped = 1)
            goto sort_start_label
        ifend
    print("Sort complete.")
funcend

; --- Function to append the contents of 'log_content_buffer' to disk ---
func append_to_log()
    ; This function uses the agreed-upon global buffers
    fileappend_arr filename_buffer, log_content_buffer
    
    ; Also append a newline for readability
    log_content_buffer.clear
    log_content_buffer.add 10
    fileappend_arr filename_buffer, log_content_buffer
funcend

;#######################################################
;# MAIN EXECUTION BLOCK
;#######################################################
main
    ; --- 1. Setup ---
    arradd filename_buffer demo_output.log
    filedelete_arr filename_buffer ; Start with a clean file

    ; --- 2. Populate and Sort Data ---
    number_list.add 42
    number_list.add 8
    number_list.add 1991
    number_list.add 100
    sort_number_list()

    ; --- 3. Log the Sorted Data ---
    log_content_buffer.clear
    arradd log_content_buffer Sorted list written to disk.
    append_to_log()
    print("Log updated.")

    ; --- 4. Get User Input and Log It ---
    print("--- User Input ---")
    arradd prompt Enter a message:
    prompt.add 32 ; Add trailing space for prompt
    input user_input, prompt
    
    log_content_buffer.clear
    log_content_buffer.copy user_input
    append_to_log()
    print("User message logged.")

    ; --- 5. Read and Display the Final Log File ---
    print("--- Final Log Contents ---")
    log_content_buffer.clear ; Reuse buffer for reading
    fileread_arr log_content_buffer, filename_buffer
    
    log_content_buffer.size
    Loop, rax
        log_content_buffer.index A_Index
        print_rax_as_char
    endloop
    
    print("--- DEMO COMPLETE ---")
```

---

### 15. HTLL Quirks & Gotchas: A Mandatory Review

This is a summary of the most important, non-obvious rules of HTLL. Internalize them.

*   **`arradd` USES NO QUOTES:** The most common mistake. `arradd my_arr Some text` is correct. `arradd my_arr "Some text"` is **wrong**. To add a space at the end of text, you must append it manually with `.add 32`.
*   **ALL DECLARED VARIABLES ARE GLOBAL:** There is no local variable declaration. `int x := 0` inside a function creates a global `x` that is initialized only once. The only local scope is for *function parameters*. Doing so will cause big problems, so just DO NOT declare variables inside a function body.
*   **ARRAYS ARE NOT PARAMETERS:** You cannot pass an array to a function like `my_func(my_array)`. This is forbidden. You must use the Global Buffer Pattern described in section 10.
*   **`args_array` IS RESERVED:** Do not declare `arr args_array`. The system provides it.
*   **NO `else`, `and`, `or`:** These constructs do not exist. You must build equivalent logic using `goto` and nested `if` statements. This is intentional.
*   **`print("")` IS FOR NEWLINES:** This is the specific syntax trick to print a newline character.
*   **SPACE BEFORE END-OF-LINE COMMENTS:** A comment at the end of a line needs a space. `x++ ; comment` is correct. `x++;comment` is wrong.
*   **NESTING IS FINITE:** You have 9 levels (`Loop` to `Loop9`, `if` to `if9`). If you need more, your code is too complex and must be refactored into smaller functions.

---

In conclusion: **C is bloated.**

You have been told your whole life that C is the pinnacle of minimalism. This is a lie. The dependency on `libc` and the complexity of its toolchains introduce unnecessary bloat.

HTLL is an experiment in true minimalism. The trade-off is its specificity: it only targets only one architecture (x86-64) and one operating system (Linux). But within that domain, it offers unparalleled control and efficiency.
