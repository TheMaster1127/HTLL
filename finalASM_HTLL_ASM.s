; This defines a "struct" to make our code readable.
struc DynamicArray
    .pointer:   resq 1  ; Offset 0: Holds the pointer to the heap memory
    .size:      resq 1  ; Offset 8: Holds the current number of elements
    .capacity:  resq 1  ; Offset 16: Holds how many elements the block can store
endstruc

section .data
    SCALE_FACTOR:   dq 1000000
    INITIAL_CAPACITY equ 2  ; Let's use a smaller capacity to test resizing sooner
    print_buffer:   times 21 db 0
    dot:            db '.'
    minus_sign:     db '-'
    nl:   db 10


ASM_STR_TEMP_PRINT_1: db "hello man", 10
ASM_STR_TEMP_PRINT_1_len: equ $-ASM_STR_TEMP_PRINT_1


var1: dq 5



section .bss
    print_buffer_n: resb 20


var2: resb 1024


section .text
global _start

; =============================================================================
; UTILITY FUNCTIONS (Correct and Properly Formatted)
; =============================================================================

;-----------------------------------------------------------------------------
; divide_and_store: Performs fixed-point division and stores results in memory.
; [In] rdi: dividend_int, rsi: dividend_dec
;      rdx: divisor_int,  rcx: divisor_dec
;      r8:  ptr to result_int, r9: ptr to result_dec
;-----------------------------------------------------------------------------
divide_and_store:
    ; Prologue
    push rbx
    push r12
    push r13
    push r14
    push r15

    ; --- THE FIX: Save all volatile input arguments immediately ---
    mov r12, rdi            ; Save dividend_int
    mov r13, rsi            ; Save dividend_dec
    mov r14, rdx            ; Save divisor_int (THIS IS THE CRITICAL FIX)
    mov r15, rcx            ; Save divisor_dec
    
    ; --- Step 1: Scale the Dividend ---
    mov rax, r12            ; Use the saved dividend_int
    mov rbx, [SCALE_FACTOR]
    mul rbx
    add rax, r13            ; Add the saved dividend_dec
    mov r12, rax            ; r12 now holds the final scaled dividend

    ; --- Step 2: Scale the Divisor ---
    mov rax, r14            ; Use the saved divisor_int
    mov rbx, [SCALE_FACTOR]
    mul rbx
    add rax, r15            ; Add the saved divisor_dec
    mov r13, rax            ; r13 now holds the final scaled divisor

    ; --- Step 3: Perform the main 128-bit division ---
    mov rax, r12            ; Load the scaled dividend
    mov rbx, [SCALE_FACTOR]
    mul rbx
    div r13                 ; Divide by the scaled divisor
    
    ; --- Step 4: De-scale the final result ---
    mov rbx, [SCALE_FACTOR]
    xor rdx, rdx
    div rbx

    ; --- Step 5: Store the results using the pointers from r8 and r9 ---
    mov [r8], rax           ; Store the integer result
    mov [r9], rdx           ; Store the decimal result

    ; Epilogue
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

;-----------------------------------------------------------------------------
; print_div: (Unchanged logic, but now properly formatted)
; [In] rdi: integer part, rsi: decimal part
;-----------------------------------------------------------------------------
print_div:
    ; Prologue
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push rcx
    push rsi

    ; --- Print integer part ---
    mov rax, rdi
    lea r12, [print_buffer + 19]
    mov byte [r12], 0
    dec r12
    mov rbx, 10
.int_convert_loop:
    xor rdx, rdx
    div rbx
    add rdx, '0'
    mov [r12], dl
    dec r12
    cmp rax, 0
    jne .int_convert_loop

    inc r12
    mov rsi, r12
    lea rdx, [print_buffer + 19]
    sub rdx, rsi
    mov rax, 1
    mov rdi, 1
    syscall

    ; --- Print decimal point ---
    mov rax, 1
    mov rdi, 1
    mov rsi, dot
    mov rdx, 1
    syscall

    ; --- Print decimal part ---
    pop rax
    lea r12, [print_buffer + 5]
    mov rbx, 10
    mov rcx, 6
.dec_convert_loop:
    xor rdx, rdx
    div rbx
    add rdx, '0'
    mov [r12], dl
    dec r12
    loop .dec_convert_loop

    mov rax, 1
    mov rdi, 1
    lea rsi, [print_buffer]
    mov rdx, 6
    syscall

    ; Print a final newline.
    mov rax, 1
    mov rdi, 1
    mov rsi, nl
    mov rdx, 1
    syscall

    ; Epilogue
    pop rcx
    pop r12
    pop rbx
    pop rbp
    ret


;-----------------------------------------------------------------------------
; print_number: Prints a number, prepending a '-' sign if requested.
;
; [In]
;   rdi - The number to print (as an unsigned, positive magnitude).
;   rsi - The "is_negative" flag. 0 for positive, 1 for negative.
;-----------------------------------------------------------------------------
print_number:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push rdi        ; Save the number itself (the value from rdi) on the stack.

    ; Check the is_negative flag passed in rsi
    cmp rsi, 0
    je .skip_sign_print

    ; Flag is 1, so print a minus sign.
    ; This is safe because the number is preserved on the stack.
    mov rax, 1
    mov rdi, 1
    mov rsi, minus_sign
    mov rdx, 1
    syscall

.skip_sign_print:
    ; Restore the saved number from the stack directly into RAX,
    ; which is the register we need for the division loop.
    pop rax

    ; The conversion loop, which now operates on the correct number in rax.
    lea r12, [print_buffer + 19]
    mov byte [r12], 0
    dec r12
    mov rbx, 10
.conversion_loop:
    xor rdx, rdx
    div rbx
    add rdx, '0'
    mov [r12], dl
    dec r12
    cmp rax, 0
    jne .conversion_loop

    inc r12
    mov rsi, r12
    lea rdx, [print_buffer + 20]
    sub rdx, rsi
    mov rax, 1
    mov rdi, 1
    syscall

    ; Print a final newline.
    mov rax, 1
    mov rdi, 1
    mov rsi, nl
    mov rdx, 1
    syscall

    ; The stack is balanced because we pushed RDI and popped it into RAX.
    pop r12
    pop rbx
    pop rbp
    ret

print_str:
    push rbp
    mov rbp, rsp
    mov rax, 1
    mov rdi, 1
    ; rsi and rdx are expected to be set by the caller
    syscall
    pop rbp
    ret

;-----------------------------------------------------------------------------
; input: Reads a line from stdin, handles empty input, removes the trailing
;        newline, and stores the raw string and its length.
;
; [In]
;   rdi - Pointer to the buffer where the string should be stored.
;   rsi - Pointer to a qword where the length of the clean string will be stored.
;-----------------------------------------------------------------------------
input:
    push rbp
    mov rbp, rsp
    push r12
    push r13

    ; Save the pointers passed as arguments immediately
    mov r12, rdi ; r12 holds pointer to the buffer
    mov r13, rsi ; r13 holds pointer to the length variable

    ; Correctly set up all registers for the sys_read syscall
    mov rax, 0
    mov rdi, 0
    mov rsi, r12
    mov rdx, 100
    syscall
    ; The program waits here. rax will return with bytes read.

    ; Check if the user entered anything. If they just press Enter, rax will be 1.
    ; If they press Ctrl+D, rax will be 0. Both cases mean empty input.
    cmp rax, 1
    jle .read_empty     ; Jump if Less or Equal (handles 0 and 1)

    ; This code only runs for valid input (rax > 1).
    ; The length of the clean string is (bytes_read - 1).
    dec rax
    jmp .store_length   ; Jump to the end to store the calculated length

.read_empty:
    ; === THE FIX IS HERE ===
    ; If the input was empty, explicitly set the length to 0.
    xor rax, rax

.store_length:
    ; Store the final calculated length (either from dec rax or xor rax)
    ; into the memory location pointed to by r13.
    mov [r13], rax

    pop r13
    pop r12
    pop rbp
    ret


; =============================================================================
; DYNAMIC ARRAY LIBRARY FUNCTIONS (Final, Corrected Version)
; =============================================================================
array_append:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14

    mov r12, rdi             ; R12 = pointer to our array struct
    mov r13, rsi             ; R13 = value we want to add

    mov rcx, [r12 + DynamicArray.size]
    cmp rcx, [r12 + DynamicArray.capacity]
    jl .has_space

.resize:
    mov rbx, [r12 + DynamicArray.capacity]
    cmp rbx, 0
    jne .double_it
    mov rbx, INITIAL_CAPACITY
    jmp .capacity_calculated
.double_it:
    shl rbx, 1
.capacity_calculated:
    mov [r12 + DynamicArray.capacity], rbx

    push r12
    push r13

    mov r14, rbx
    imul r14, 8

    mov rax, 9
    mov rdi, 0
    mov rsi, r14
    mov rdx, 3
    mov r10, 34
    mov r8, -1
    mov r9, 0
    syscall

    cmp rax, 0
    jl .mmap_failed

    mov r14, rax

    pop r13
    pop r12

    mov rsi, [r12 + DynamicArray.pointer]
    cmp rsi, 0
    je .skip_copy

    mov rcx, [r12 + DynamicArray.size]
    mov rdi, r14
    rep movsq

.skip_copy:
    mov [r12 + DynamicArray.pointer], r14
    jmp .has_space

.mmap_failed:
    pop r13
    pop r12
    jmp .append_epilogue

.has_space:
    mov rbx, [r12 + DynamicArray.pointer]
    mov rcx, [r12 + DynamicArray.size]

    mov [rbx + rcx * 8], r13
    inc qword [r12 + DynamicArray.size]

.append_epilogue:
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

array_pop:
    push rbp
    mov rbp, rsp
    push rbx
    push rcx
    push r8

    mov r8, rdi
    mov rcx, [r8 + DynamicArray.size]
    cmp rcx, 0
    je .pop_empty

    dec rcx
    mov [r8 + DynamicArray.size], rcx
    mov rbx, [r8 + DynamicArray.pointer]
    mov rax, [rbx + rcx * 8]
    jmp .pop_done

.pop_empty:
    xor rax, rax

.pop_done:
    pop r8
    pop rcx
    pop rbx
    pop rbp
    ret

;-----------------------------------------------------------------------------
; array_clear: Clears a dynamic array by setting its size to 0.
; [In] rdi: Pointer to the DynamicArray struct.
;-----------------------------------------------------------------------------
array_clear:
    push rbp
    mov rbp, rsp

    ; Set the .size field of the struct pointed to by rdi to 0.
    mov qword [rdi + DynamicArray.size], 0

    pop rbp
    ret

;-----------------------------------------------------------------------------

print_char:
    push rbp
    mov rbp, rsp
    sub rsp, 1                  ; reserve 1 byte for char storage

    mov byte [rsp], dil         ; store char at rsp (safe because reserved)

    mov rax, 1                 ; sys_write
    mov rdi, 1                 ; stdout
    mov rsi, rsp               ; pointer to char on stack
    mov rdx, 1                 ; length = 1
    syscall

    add rsp, 1                 ; deallocate local stack space
    pop rbp
    ret




; =============================================================================
; MAIN PROGRAM ENTRY POINT
; =============================================================================
_start:

mov rdi, 5
mov rsi, 0
call print_number
mov rsi, ASM_STR_TEMP_PRINT_1
mov rdx, ASM_STR_TEMP_PRINT_1_len
call print_str
mov rdi, [var1]
mov rsi, 0
call print_number


    ; --- Exit cleanly ---
    mov rax, 60
    xor rdi, rdi
    syscall
