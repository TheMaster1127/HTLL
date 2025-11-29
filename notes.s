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
    msg1: db "--- Array 1 Contents ---", 10
    msg1_len: equ $-msg1
    msg2: db "--- Array 2 Contents ---", 10
    msg2_len: equ $-msg2
    msg3: db "--- Popping from Array 1 ---", 10
    msg3_len: equ $-msg3
    
    someInt1: dq 25
    someInt2: dq 26
    someIntOut: dq 0
    
    big_string_addr: dq 0
    big_string_size: dq 0

    ; --------------------------------------------------------------------
    prompt:         db "Please enter your name: "
    prompt_len:     equ $-prompt

    greeting:       db "Hello, "
    greeting_len:   equ $-greeting
    userName_len:   dq 0

    ; -------------------------------------------------------------------

    ; -- Inputs for our calculation: 10.5 / 3.7
    dividend_int:   dq 10
    dividend_dec:   dq 500000      ; .5 as 6 decimal digits

    divisor_int:    dq 3
    divisor_dec:    dq 700000       ; .7 as 6 decimal digits

    ; -- Outputs from our calculation
    result_digit:   dq 0
    result_decimal: dq 0

	test_int_neg: dq 6
	test_int_neg_isNegative: dq 0



section .bss
    print_buffer_n: resb 20
    ; These are our three "Array objects". They start completely zeroed out.
    array1: resb DynamicArray_size
    array2: resb DynamicArray_size
    array3: resb DynamicArray_size ; We'll leave array3 empty to test that case

    array69: resb DynamicArray_size

    big_string: resb 10  ; reserve a buffer

    userName: resb 100

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
; input: Corrected version.
; [In] rdi: Pointer to buffer, rsi: Pointer to length variable
;-----------------------------------------------------------------------------
input:
    push rbp
    mov rbp, rsp
    push r12 ; To save the buffer pointer
    push r13 ; To save the length pointer

    ; Save the pointers passed as arguments immediately
    mov r12, rdi ; r12 now holds the pointer to the userName buffer
    mov r13, rsi ; r13 now holds the pointer to the userName_len variable

    ; --- Correctly set up all registers for the sys_read syscall ---
    mov rax, 0              ; syscall number for sys_read
    mov rdi, 0              ; file descriptor for stdin (THIS WAS THE MISSING PART)
    mov rsi, r12            ; pointer to our buffer (from r12)
    mov rdx, 100            ; maximum number of bytes to read
    syscall
    ; The program will now correctly wait here for user input.

    ; Check if the user entered anything
    cmp rax, 1
    jle .read_empty

    ; Input is valid, length is (bytes_read - 1)
    dec rax
    
.read_empty:
    ; Store the calculated length into the memory location pointed to by r13
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



; ========================================================================
; test func call

func1:
    push rbp
    mov rbp, rsp
    sub rsp, 16
	mov qword [rbp - 8], 5 ; var1


	mov rax, [rbp + 16]
    add rax, [rbp + 24]
	add rax, [rbp + 32]
	add rax, [rbp - 8]

.func1_return:
    add rsp, 16
    pop rbp
	ret

; =============================================================================
; MAIN PROGRAM ENTRY POINT
; =============================================================================
_start:

	push 5
	push 10
	push 15
	call func1
	add rsp, 24
	mov rdi, rax 
	mov rsi, 0
    call print_number


    ; --- Step 1: Populate array1 with 3 elements ---
    mov rsi, 100
    mov rdi, array1
    call array_append

    mov rsi, 101
    mov rdi, array1
    call array_append

    mov rsi, 102
    mov rdi, array1
    call array_append

    ; --- Step 2: Populate array2 with 2 elements ---
    mov rsi, 999
    mov rdi, array2
    call array_append

    mov rsi, 888
    mov rdi, array2
    call array_append

    ; --- Step 3: Print the contents of array1 ---
    mov rsi, msg1
    mov rdx, msg1_len
    call print_str

    mov r13, [array1 + DynamicArray.size] ; **YOUR FIX: Use a callee-saved register for the loop counter**
    xor r12, r12
.print_loop1:
    cmp r13, 0
    je .print_loop1_end
    mov rbx, [array1 + DynamicArray.pointer]
    mov rdi, [rbx + r12 * 8]
    mov rsi, 0
    call print_number
    inc r12
    dec r13
    jmp .print_loop1
.print_loop1_end:

    ; --- Step 3: Print the contents of array2 ---
    mov rsi, msg2; mov rdx, msg2_len; call print_str

    mov r13, [array2 + DynamicArray.size] ; **YOUR FIX: Use a callee-saved register**
    xor r12, r12
.print_loop2:
    cmp r13, 0
    je .print_loop2_end
    mov rbx, [array2 + DynamicArray.pointer]
    mov rdi, [rbx + r12 * 8]
    mov rsi, 0
    call print_number
    inc r12
    dec r13
    jmp .print_loop2
.print_loop2_end:

    ; --- Step 5: Pop elements and prove independence ---
    mov rsi, msg3
    mov rdx, msg3_len
    call print_str

    mov rdi, array1
    call array_pop          ; Pop 102 from array1. Result is in RAX.
    mov rdi, rax            ; **THE FIX: Move the result from RAX to RDI.**
    mov rsi, 0
    call print_number

    mov rdi, array2
    call array_pop          ; Pop 888 from array2. Result is in RAX.
    mov rdi, rax            ; **THE FIX: Move the result from RAX to RDI.**
    mov rsi, 0
    call print_number

    mov rdi, array3
    call array_pop          ; Pop from empty array3. Result (0) is in RAX.
    mov rdi, rax            ; **THE FIX: Move the result from RAX to RDI.**
    mov rsi, 0
    call print_number



    mov rdi, [someInt1]
    add rdi, [someInt2] 
    add [someIntOut], rdi        
    inc qword [someIntOut]
    mov rdi, [someIntOut]       
    mov rsi, 0     
    call print_number



    mov rsi, 69
    mov rdi, array69
    call array_append

    mov rsi, 691
    mov rdi, array69
    call array_append

	mov r12, 0 ; its my index
	mov rbx, [array69 + DynamicArray.pointer] ; RBX now holds the starting address of the data [69, 691].
	mov rdi, [rbx + r12 * 8]
    mov rsi, 0
    call print_number

	mov rdi, 11111111111 ; a sepertor to see better
    mov rsi, 0
    call print_number
    
	mov rdi, [array69 + DynamicArray.size] 
    mov rsi, 0
    call print_number

        ; --- Now, pop an element from array69 ---
    mov rdi, array69        ; Arg 1 for array_pop is the pointer to the struct.
    call array_pop          ; Pop 691 from array69. The popped value is returned in RAX.


	;mov rdi, array69
	;call array_clear

	mov rdi, [array69 + DynamicArray.size] 
    mov rsi, 0
    call print_number



    ; --- Step 1: Print the prompt to the user ---
    mov rsi, prompt
    mov rdx, prompt_len
    call print_str

    ; --- Step 2: Call our input function ---
    lea rdi, [userName]
    lea rsi, [userName_len]
    call input

    ; --- Step 3: Print the greeting back ---
    mov rsi, greeting
    mov rdx, greeting_len
    call print_str

    mov rsi, userName
    mov rdx, [userName_len]
    call print_str

    mov rsi, nl
    mov rdx, 1
    call print_str

    ; --- Step 4: Print the len ---

    mov rdi, [userName_len]
    mov rsi, 0
    call print_number


    ; --- One-time setup: Store the runtime address in our named pointer variable ---
    lea rax, [rel big_string]
    mov [rel big_string_addr], rax
    
 	mov rax, [rel big_string_addr]	
    mov byte [rax + 0], 'H'
    inc qword [big_string_size]
 	mov rax, [rel big_string_addr]
    mov byte [rax + 1], 'e'
    inc qword [big_string_size]
    mov rax, [rel big_string_addr]
    mov byte [rax + 2], 'l'
    inc qword [big_string_size]
	mov rax, [rel big_string_addr]
    mov byte [rax + 3], 'l'
    inc qword [big_string_size]
    mov rax, [rel big_string_addr]
    mov byte [rax + 4], 'o'
    inc qword [big_string_size]
    mov rax, [rel big_string_addr]
    mov byte [rax + 5], 10
    inc qword [big_string_size]
    mov rax, [rel big_string_addr]


	mov rdx, [big_string_size]
	mov rsi, [rel big_string_addr]

	call print_str

	push 5
	push 10
	push 15
	call func1
	mov rdi, rax 
    mov rsi, 0
    call print_number
	add rsp, 24

	mov rdi, 999999 ; ---------- kinda like --------- im lazy
    mov rsi, 0
    call print_number	

	; last learning the loop
	; it must loop 10 times and print each index and break at index 5

	xor r13, r13
	mov r12, 10
.myLoop:
	cmp r12, 0
	je .myLoop_end
	; body
	; r13 is the index
	mov rdi, r13
        mov rsi, 0
	call print_number
	cmp r13, 5
	je .myLoop_end
	; body
	inc r13
	dec r12
	jmp .myLoop
.myLoop_end:


    ; 1. Load all 6 arguments and call the calculation function.
    mov rdi, [dividend_int]
    mov rsi, [dividend_dec]
    mov rdx, [divisor_int]
    mov rcx, [divisor_dec]
    lea r8,  [result_digit]
    lea r9,  [result_decimal]
    call divide_and_store

    ; 2. Load the results and call the print function.
    mov rdi, [result_digit]
    mov rsi, [result_decimal]
    call print_div



	sub qword [test_int_neg], 20
	js .is_negative1     ; Jump if the Sign Flag is set
	
.is_positive1:
    ; The number is positive or zero, just print it.
    mov qword [test_int_neg_isNegative], 0
    jmp .is_negative_done1       ; Skip the negative-handling code

.is_negative1:
    mov qword [test_int_neg_isNegative], 1

    ; === THE FIX IS HERE ===
    ; We must convert the negative bit pattern back to its positive magnitude.
    mov rax, [test_int_neg]  ; Load the negative bit pattern (e.g., for -14)
    neg rax                  ; Negate it. rax now holds the positive magnitude (14)
    mov [test_int_neg], rax  ; Store the positive magnitude back into the variable.
    

.is_negative_done1:

    mov rdi, [test_int_neg]
    mov rsi, [test_int_neg_isNegative]	
	call print_number
	
    ; --- Exit cleanly ---
    mov rax, 60
    xor rdi, rdi
    syscall

