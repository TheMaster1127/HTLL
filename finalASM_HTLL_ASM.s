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


FILE_PATH_1: db "my_program.htll", 0
FILE_PATH_2: db "test_output.s", 0





section .bss
    input_buffer: resb 256 ; A buffer to temporarily hold user's raw input
file_read_buffer: resb 4096
input_len:    resq 1   ; A variable to hold the length of the input
    source_ptr:      resq 1
source_ptr_size: resq 1
asm_code_ptr:    resq 1
    print_buffer_n: resb 20

my_source_code: resb DynamicArray_size
my_asm_output: resb DynamicArray_size





section .text
global _start

; =============================================================================

; =============================================================================
; UTILITY FUNCTIONS (Correct and Properly Formatted)
; =============================================================================

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
; is_nint_negative: Reads a signed number, updates its corresponding sign
;                   flag, AND converts the original number to its positive
;                   magnitude if it was negative.
;
; [In]
;   rdi - Pointer to the nint variable (which holds a signed value).
;   rsi - Pointer to the is_negative flag variable for that nint.
;
; [Out]
;   The memory at [rsi] will be overwritten with 0 or 1.
;   The memory at [rdi] will be overwritten with its positive magnitude.
;-----------------------------------------------------------------------------
is_nint_negative:
    push rbp
    mov rbp, rsp
    push rax

    ; Load the actual signed value from the first pointer
    mov rax, [rdi]

    ; Check its sign using the CPU's Sign Flag
    test rax, rax
    jns .is_positive_or_zero    ; Jump if Not Sign

.is_negative:
    ; The number is negative.
    ; 1. Update the flag variable to 1.
    mov qword [rsi], 1
    
    ; 2. Negate the value to get its positive magnitude.
    neg rax
    
    ; 3. Store the positive magnitude BACK into the original variable.
    mov [rdi], rax
    jmp .done

.is_positive_or_zero:
    ; The number is positive or zero.
    ; 1. Update the flag variable to 0.
    mov qword [rsi], 0
    ; 2. The value at [rdi] is already its own positive magnitude, so we do nothing to it.

.done:
    pop rax
    pop rbp
    ret


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
; array_clear: Clears a dynamic array by setting its size to 0.

; =============================================================================
; array_copy: Copies all elements from a source array to a destination array.
; The destination array will be resized if necessary. Any existing data in
; the destination array will be overwritten.
; [In]
;   rdi - Pointer to the destination DynamicArray struct.
;   rsi - Pointer to the source DynamicArray struct.
; =============================================================================
array_copy:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14

    mov r12, rdi             ; r12 = pointer to destination struct
    mov r13, rsi             ; r13 = pointer to source struct

    ; --- Step 1: Check if the destination has enough capacity ---
    mov rbx, [r13 + DynamicArray.size] ; Get the number of elements we need to copy
    cmp [r12 + DynamicArray.capacity], rbx
    jge .copy_has_capacity   ; Jump if Greater or Equal, skip resize

.copy_resize_needed:
    ; Free the old memory block to prevent leaks, if it exists
    mov rdi, [r12 + DynamicArray.pointer]
    cmp rdi, 0
    je .copy_skip_munmap     ; If pointer is null, nothing to free
    mov rsi, [r12 + DynamicArray.capacity]
    imul rsi, 8              ; Calculate old block size in bytes
    mov rax, 11              ; munmap syscall number
    syscall

.copy_skip_munmap:
    ; The required new size is already in rbx from the check above
    mov [r12 + DynamicArray.capacity], rbx ; Set new capacity

    push r12 ; Save registers before mmap syscall
    push r13

    mov r14, rbx
    imul r14, 8              ; Calculate bytes needed for new block

    ; mmap syscall to get new memory
    mov rax, 9
    mov rdi, 0
    mov rsi, r14
    mov rdx, 3
    mov r10, 34
    mov r8, -1
    mov r9, 0
    syscall
    ; rax now holds the new pointer

    pop r13 ; Restore registers
    pop r12

    cmp rax, 0
    jl .copy_epilogue        ; If mmap failed, exit function

    mov [r12 + DynamicArray.pointer], rax ; Update destination to use the new memory

.copy_has_capacity:
    ; --- Step 2: Copy the data ---
    mov rcx, [r13 + DynamicArray.size] ; Number of qwords to copy
    cmp rcx, 0
    je .copy_update_size     ; If source is empty, just update size and finish

    mov rdi, [r12 + DynamicArray.pointer] ; Destination address
    mov rsi, [r13 + DynamicArray.pointer] ; Source address
    rep movsq                ; THE MAGIC: Copy RCX qwords from [RSI] to [RDI]

.copy_update_size:
    ; --- Step 3: Update destination's size ---
    mov rbx, [r13 + DynamicArray.size]
    mov [r12 + DynamicArray.size], rbx

.copy_epilogue:
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

; [In] rdi: Pointer to the DynamicArray struct.
;-----------------------------------------------------------------------------
array_clear:
    push rbp
    mov rbp, rsp
    ; Set the .size field of the struct pointed to by rdi to 0.
    mov qword [rdi + DynamicArray.size], 0
    pop rbp
    ret

; =============================================================================
; array_append: (Final, Robust Version with Error Checking)
; Appends a qword value to a dynamic array, resizing if necessary.
; =============================================================================
array_append:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov r12, rdi             ; r12 = pointer to the array struct
    mov r13, rsi             ; r13 = the qword value to add

    mov rcx, [r12 + DynamicArray.size]
    cmp rcx, [r12 + DynamicArray.capacity]
    jl .has_space

; --- Resize Logic ---
.resize:
    mov r14, [r12 + DynamicArray.pointer]   ; r14 = old pointer
    mov r15, [r12 + DynamicArray.capacity]  ; r15 = old capacity
    
    mov rbx, r15
    cmp rbx, 0
    jne .double_it
    mov rbx, INITIAL_CAPACITY
    jmp .capacity_calculated
.double_it:
    shl rbx, 1
.capacity_calculated:
    mov [r12 + DynamicArray.capacity], rbx

    ; --- Allocate new memory ---
    push r12
    push r13
    push r14
    push r15

    imul rbx, 8                             ; Convert new capacity to bytes
    mov rax, 9                              ; sys_mmap
    mov rdi, 0
    mov rsi, rbx
    mov rdx, 3
    mov r10, 34
    mov r8, -1
    mov r9, 0
    syscall

    pop r15
    pop r14
    pop r13
    pop r12

    ; ========================= THE CRITICAL FIX =========================
    ; Syscalls return a negative value on error. We must check for this.
    ; A valid pointer will be a small positive number. An error is a large
    ; number close to -1 (0xFFFFFFFFFFFFFFFF).
    cmp rax, 0
    jl .mmap_failed                         ; If rax is Less than 0, mmap failed.
    ; ====================================================================

    ; --- Copy old data to new block ---
    cmp r14, 0                              ; Was there an old block?
    je .skip_copy                           

    mov rdi, rax                            ; Destination is new block
    mov rsi, r14                            ; Source is old block
    mov rcx, [r12 + DynamicArray.size]      
    rep movsq                               

    ; --- Free the OLD memory block ---
    push rax
    push r12
    push r13

    imul r15, 8                             ; Old capacity in bytes
    mov rdi, r14                            ; Pointer to the old block
    mov rsi, r15                            ; Size of the old block
    mov rax, 11                             ; sys_munmap
    syscall

    pop r13
    pop r12
    pop rax

.skip_copy:
    mov [r12 + DynamicArray.pointer], rax

.has_space:
    mov rbx, [r12 + DynamicArray.pointer]
    mov rcx, [r12 + DynamicArray.size]
    mov [rbx + rcx * 8], r13
    inc qword [r12 + DynamicArray.size]
    jmp .append_epilogue

.mmap_failed:
    ; If mmap fails, we can't continue. Exit with an error code (e.g., 1).
    mov rax, 60
    mov rdi, 1
    syscall

.append_epilogue:
    pop r15
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

print_char:
    push rbp
    mov rbp, rsp

    ; Push the entire RDI register. This subtracts 8 bytes from RSP,
    ; which keeps the stack correctly aligned. The character is in DIL,
    ; the lowest byte of RDI, so this works perfectly.
    push rdi

    mov rax, 1                 ; sys_write
    mov rdi, 1                 ; stdout
    mov rsi, rsp               ; The pointer to the character is now the top of the stack
    mov rdx, 1                 ; length = 1
    syscall

    ; Clean up the stack by popping the value we pushed.
    ; This adds 8 back to RSP, restoring it.
    pop rdi

    pop rbp
    ret

; =============================================================================
; sys_read_line: A low-level wrapper around the sys_read syscall for stdin.
; [In]
;   rdi - Pointer to the buffer where the string should be stored.
;   rsi - Max number of bytes to read.
; [Out]
;   rax - The number of bytes actually read (including newline).
; =============================================================================
sys_read_line:
    push rbp
    mov rbp, rsp

    ; Prepare arguments for the sys_read syscall
    mov rax, 0      ; syscall number for sys_read
    mov rdx, rsi    ; 3rd arg: max number of bytes to read
    mov rsi, rdi    ; 2nd arg: the destination buffer pointer
    mov rdi, 0      ; 1st arg: file descriptor for stdin (0)
    
    syscall         ; Execute the read. rax will contain bytes read.

    pop rbp
    ret



; =============================================================================
; get_user_input: (Final, Clean, Production Version)
; Correctly prompts, reads, and appends user input.
; =============================================================================
get_user_input:
    ; --- Prologue: Save all callee-saved registers we will use. ---
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    ; Correctly align the entire stack frame for all subsequent calls.
    sub rsp, 8

    ; Save arguments immediately into safe callee-saved registers.
    mov r12, rdi
    mov r13, rsi

    ; --- Part 1: Print the prompt string ---
    mov rcx, [r13 + DynamicArray.size]
    cmp rcx, 0
    je .read_from_user

    mov r14, rcx             ; Save original size.
    add rcx, 15              ; Round up for alignment.
    and rcx, -16
    mov rbx, rcx             ; Save aligned size in safe RBX (syscall clobbers RCX).
    sub rsp, rbx             ; Allocate space using the safe value from RBX.

    ; Pack the prompt into the stack buffer.
    mov rdi, rsp
    mov rsi, [r13 + DynamicArray.pointer]
    push rbx                 ; Save rbx (aligned size) before the loop uses it.
    xor rbx, rbx
.pack_loop:
    cmp rbx, r14
    jge .do_prompt_print
    mov al, [rsi + rbx*8]
    mov [rdi + rbx], al
    inc rbx
    jmp .pack_loop
.do_prompt_print:
    pop rbx                  ; Restore rbx (aligned size).
    
    mov rax, 1
    mov rdi, 1
    mov rsi, rsp
    mov rdx, r14             ; Use the original size for the write length.
    syscall

    add rsp, rbx             ; Clean up the stack using the SAFE value from RBX.

    ; --- Part 2: Read user's input ---
.read_from_user:
    mov rax, 0
    mov rdi, 0
    mov rsi, input_buffer
    mov rdx, 255
    syscall

    ; --- Part 3: Append the input to the dynamic array ---
    cmp rax, 1
    jle .done

    dec rax
    mov r14, rax
    mov r15, input_buffer
.append_char_loop:
    cmp r14, 0
    je .done

    mov rdi, r12
    movzx rsi, byte [r15]
    call array_append

    inc r15
    dec r14
    jmp .append_char_loop

.done:
    ; --- Epilogue: Restore the stack and all saved registers. ---
    add rsp, 8
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

    
; =============================================================================
; file_read: Appends a file's contents to a dynamic array.
; =============================================================================
file_read:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov r12, rdi             ; r12 = pointer to dest array
    mov r13, rsi             ; r13 = pointer to file path

    mov rax, 2               ; sys_open
    mov rdi, r13
    xor rsi, rsi             ; O_RDONLY
    syscall
    cmp rax, 0
    jl .read_error           
    mov r14, rax             ; r14 = file descriptor

.read_loop:
    mov rax, 0               ; sys_read
    mov rdi, r14
    mov rsi, file_read_buffer
    mov rdx, 4096
    syscall
    cmp rax, 0
    jle .close_and_exit      

    mov r15, rax             ; Use safe r15 for the byte counter
    mov rsi, file_read_buffer
.append_byte_loop:
    cmp r15, 0
    je .read_loop            

    movzx rbx, byte [rsi]    
    
    push rsi                 
    mov rdi, r12             
    mov rsi, rbx             
    call array_append
    pop rsi                  

    inc rsi                  
    dec r15                  
    jmp .append_byte_loop

.close_and_exit:
    mov rax, 3               ; sys_close
    mov rdi, r14             
    syscall
    xor rax, rax             
    jmp .read_done

.read_error:
.read_done:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret


; =============================================================================
; file_append: Appends an array of character codes to a file. (Corrected)
; =============================================================================
file_append:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    mov r12, rdi             ; r12 = pointer to file path
    mov r13, rsi             ; r13 = pointer to array struct

    mov rcx, [r13 + DynamicArray.size]
    cmp rcx, 0
    je .append_epilogue      ; If there's nothing to write, just exit

    ; --- THE ROBUST FIX IS HERE ---
    mov r14, rcx             ; Save original size in a safe register
    mov rbx, rcx             ; Use RBX (a safe register) for the calculation
    add rbx, 15              ; Round up
    and rbx, -16             ; Align to a 16-byte boundary
    sub rsp, rbx             ; Allocate space using the SAFE value from RBX
    ; --- END FIX ---

    mov r15, rsp             ; r15 = pointer to our packed string buffer

    mov rsi, [r13 + DynamicArray.pointer]
    xor rcx, rcx             ; Use rcx as the loop counter, it's fine for it to be clobbered
.pack_loop:
    cmp rcx, r14             ; Loop using the ORIGINAL size saved in r14
    jge .pack_done
    mov al, [rsi + rcx*8]
    mov [r15 + rcx], al
    inc rcx
    jmp .pack_loop
.pack_done:
    ; --- Open the file ---
    mov rax, 2
    mov rdi, r12
    mov rsi, 1089            ; O_WRONLY | O_CREAT | O_APPEND
    mov rdx, 0o644
    syscall
    cmp rax, 0
    jl .append_error
    mov r14, rax             ; Save FD in a safe register (r14 is now overwritten)

    ; --- Write the packed data ---
    mov rax, 1
    mov rdi, r14             ; Write to the file descriptor
    mov rsi, r15             ; Source is our packed buffer on the stack
    mov rdx, [r13 + DynamicArray.size] ; Use the original size for the write
    syscall

    ; --- Close the file ---
    mov rax, 3
    mov rdi, r14
    syscall

    xor rax, rax
    jmp .append_cleanup

.append_error:
    ; Error occurred, set return code or handle appropriately if needed
    
.append_cleanup:
    ; --- THE SECOND PART OF THE ROBUST FIX ---
    ; Deallocate the exact amount of stack space we allocated using the saved value in RBX.
    add rsp, rbx
    ; --- END FIX ---

.append_epilogue:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

; =============================================================================
; file_delete: Deletes a file from the filesystem.
; =============================================================================
file_delete:
    push rbp
    mov rbp, rsp
    mov rax, 87              ; syscall for sys_unlink
    syscall
    pop rbp
    ret







; --- Functions from your C++ .so file ---
extern compiler_c, free_string_c

; =============================================================================
; ADAPTER FUNCTION 1: HTLL arr -> C-style string (char*)
; =============================================================================
; Takes an HTLL array and creates a C-style string from it.
; [In]  rdi - Pointer to the source DynamicArray struct.
; [Out] rax - Pointer to the new C-style string (or 0 on error).
;==============================================================================
; =============================================================================
; ADAPTER FUNCTION 1: HTLL arr -> C-style string (char*) (Robust Version)
; =============================================================================
array_pack_to_bytes:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14

    ; --- ROBUSTNESS CHECK 1 ---
    ; Check if the struct pointer itself is null
    cmp rdi, 0
    je .pack_fail

    mov r14, [rdi + DynamicArray.pointer] ; r14 is our source pointer (the qwords)
    mov r12, [rdi + DynamicArray.size]    ; r12 = number of elements

    ; --- ROBUSTNESS CHECK 2 ---
    ; Check if the array's internal data pointer is null or if size is zero
    cmp r14, 0
    je .pack_fail
    cmp r12, 0
    je .pack_fail 

    ; === Step A: Ask the kernel for memory ===
    mov r13, r12
    inc r13                             ; Space for all chars + one '0' byte
    mov rax, 9                          ; sys_mmap
    mov rdi, 0
    mov rsi, r13
    mov rdx, 3
    mov r10, 34
    mov r8, -1
    mov r9, 0
    syscall
    
    cmp rax, 0
    jl .pack_fail

    ; === Step B: Copy the bytes, one by one ===
    mov r13, rax                        ; r13 is our destination pointer
    xor rbx, rbx                        ; rbx is our loop counter, i=0

.pack_loop:
    cmp rbx, r12
    jge .pack_add_null

    mov rcx, [r14 + rbx*8]
    mov [r13 + rbx], cl
    
    inc rbx
    jmp .pack_loop

.pack_add_null:
    mov byte [r13 + rbx], 0
    mov rax, r13
    mov rdx, r12                        ; Get original size (number of chars)
    inc rdx                             ; Add 1 for the null terminato
    jmp .pack_done

.pack_fail:
    xor rax, rax

.pack_done:
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret

; =============================================================================
; ADAPTER FUNCTION 2: C-style string (char*) -> HTLL arr
; =============================================================================
; Takes a C-style string and populates an HTLL DynamicArray with its characters.
; [In]
;   rdi - Pointer to the destination DynamicArray struct.
;   rsi - Pointer to the source, null-terminated C-style string.
;==============================================================================
; =============================================================================
; ADAPTER FUNCTION 2: C-style string (char*) -> HTLL arr (Robust Version)
; =============================================================================
array_unpack_from_bytes:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13

    mov r12, rdi  ; r12 = pointer to destination array struct
    mov r13, rsi  ; r13 = pointer to source C-string

    ; --- THE FIX IS HERE ---
    ; Before we do anything, check if the C-string pointer is NULL.
    cmp r13, 0
    je .unpack_done       ; If it's NULL, there's nothing to do. Exit safely.
    ; --- END FIX ---

    ; Clear the destination array to be safe.
    mov rdi, r12
    call array_clear

.unpack_loop:
    movzx rbx, byte [r13] ; Load ONE byte. This is now safe.
    cmp rbx, 0            ; Is this the null terminator?
    je .unpack_done       ; If yes, we are finished.

    ; Append the character to the HTLL array.
    mov rdi, r12
    mov rsi, rbx
    call array_append
    
    inc r13               ; Move to the next character in the C-string.
    jmp .unpack_loop

.unpack_done:
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret
    
    

; =============================================================================
; free_packed_string: Frees memory allocated by array_pack_to_bytes (mmap).
; [In]
;   rdi - Pointer to the memory block.
;   rsi - The size of the memory block in bytes.
; =============================================================================
free_packed_string:
    push rbp
    mov rbp, rsp
    
    cmp rdi, 0          ; Don't try to free a null pointer
    je .done

    mov rax, 11         ; syscall for sys_munmap
    ; rdi and rsi are already set by the caller
    syscall

.done:
    pop rbp
    ret



; =============================================================================
; MAIN PROGRAM ENTRY POINT
; =============================================================================
_start:
push rbp
mov rbp, rsp
and rsp, -16 

mov rdi, my_source_code
mov rsi, FILE_PATH_1
call file_read
mov rdi, my_source_code
call array_pack_to_bytes
mov [source_ptr], rax
mov rdi, [source_ptr]
call compiler_c
mov [asm_code_ptr], rax
mov rdi, my_asm_output
mov rsi, [asm_code_ptr]
call array_unpack_from_bytes
mov rdi, [source_ptr]
mov rsi, [source_ptr_size]
call free_packed_string
mov rdi, [asm_code_ptr]
call free_string_c
mov rdi, FILE_PATH_2
mov rsi, my_asm_output
call file_append


    ; --- Exit cleanly ---
    mov rsp, rbp
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall
