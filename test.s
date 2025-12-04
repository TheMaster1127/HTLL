section .data
    ; --- Strings for testing ---
    test_str_trim:      db "   some spaces   ", 0
    test_str_replace:   db "one two one three", 0
    find_str_replace:   db "one", 0
    replace_str_replace:db "NEO", 0
    test_str_substr:    db "abcdefghij", 0
    test_file_name:     db "my_test_file.txt", 0
    test_file_content:  db "This content was written from assembly!", 0
    htll_source_code:   db "main", 10, "print(123)", 10, "int x := 5", 10, "x++", 10, "print(x)", 10, 0
    
    ; --- Formatting labels for output ---
    header_trim:        db "--- Testing Trim ---", 0
    header_replace:     db "--- Testing StrReplace ---", 0
    header_substr:      db "--- Testing SubStrLastChars ---", 0
    header_file:        db "--- Testing File I/O ---", 0
    header_compiler:    db "--- Testing Compiler ---", 0
    newline:            db 10, 0

section .text
global _start

extern puts, Trim_c, StrReplace_c, SubStrLastChars_c, FileAppend_c, FileRead_c, FileDelete_c, compiler_c, free_string_c

_start:
    push rbp
    mov rbp, rsp
    and rsp, -16 ; Force 16-byte stack alignment

    ; --- Test 1: Trim_c ---
    mov rdi, header_trim
    call puts
    mov rdi, test_str_trim
    call Trim_c
    push rax
    mov rdi, rax
    call puts
    pop rdi
    call free_string_c
    mov rdi, newline
    call puts
    
    ; --- Test 2: StrReplace_c (FIXED) ---
    mov rdi, header_replace
    call puts
    mov rdi, test_str_replace
    mov rsi, find_str_replace   ; <-- CORRECT: Load address from label
    mov rdx, replace_str_replace; <-- CORRECT: Load address from label
    call StrReplace_c
    push rax
    mov rdi, rax
    call puts
    pop rdi
    call free_string_c
    mov rdi, newline
    call puts

    ; --- Test 3: SubStrLastChars_c ---
    mov rdi, header_substr
    call puts
    mov rdi, test_str_substr
    mov rsi, 4
    call SubStrLastChars_c
    push rax
    mov rdi, rax
    call puts
    pop rdi
    call free_string_c
    mov rdi, newline
    call puts

    ; --- Test 4: File I/O ---
    mov rdi, header_file
    call puts
    mov rdi, test_file_name
    mov rsi, test_file_content
    call FileAppend_c
    mov rdi, test_file_name
    call FileRead_c
    push rax
    mov rdi, rax
    call puts
    pop rdi
    call free_string_c
    mov rdi, test_file_name
    call FileDelete_c
    mov rdi, newline
    call puts
    
    ; --- Test 5: The Grand Finale - Calling compiler_c ---
    mov rdi, header_compiler
    call puts
    mov rdi, htll_source_code
    call compiler_c
    push rax
    mov rdi, rax
    call puts
    pop rdi
    call free_string_c
    
    ; --- Exit cleanly ---
    mov rsp, rbp
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall
