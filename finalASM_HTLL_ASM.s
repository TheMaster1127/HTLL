format ELF64
public _start
extrn compiler_c
extrn free_string_c

    DynamicArray.pointer  = 0
    DynamicArray.size     = 8
    DynamicArray.capacity = 16
    sizeof.DynamicArray   = 24

section '.data' writeable
    SCALE_FACTOR   dq 1000000
    INITIAL_CAPACITY = 2
    print_buffer   rb 21
    dot            db '.'
    minus_sign     db '-'
    nl   db 10


ASM_STR_TEMP_PRINT_1 db "DEBUG: Array size is...", 10
ASM_STR_TEMP_PRINT_1_len = $-ASM_STR_TEMP_PRINT_1
ASM_STR_TEMP_PRINT_2 db "DEBUG: Array content is...", 10
ASM_STR_TEMP_PRINT_2_len = $-ASM_STR_TEMP_PRINT_2
ASM_STR_TEMP_PRINT_3 db "--------------------", 10
ASM_STR_TEMP_PRINT_3_len = $-ASM_STR_TEMP_PRINT_3
ASM_STR_TEMP_PRINT_4 db ">>>> STAGE 1: Initial state of args_array <<<<", 10
ASM_STR_TEMP_PRINT_4_len = $-ASM_STR_TEMP_PRINT_4
ASM_STR_TEMP_PRINT_5 db ">>>> STAGE 2: State after 4 pops <<<<", 10
ASM_STR_TEMP_PRINT_5_len = $-ASM_STR_TEMP_PRINT_5
ASM_STR_TEMP_PRINT_6 db ">>>> STAGE 3: State after adding 's' <<<<", 10
ASM_STR_TEMP_PRINT_6_len = $-ASM_STR_TEMP_PRINT_6


i dq 0

section '.bss' writeable
    input_buffer rb 256
    file_read_buffer rb 4096
    input_len    rq 1
    filename_ptr_size  rq 1
    source_ptr      rq 1
    source_ptr_size rq 1
    args_array rq 3
    filename_ptr      rq 1
    asm_code_ptr    rq 1
    print_buffer_n rb 20
    debug_buffer rq 3
    my_source_code rq 3
    my_asm_output rq 3



section '.text' executable

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
; =============================================================================
; TRULY BULLETPROOF array_copy
; =============================================================================
array_copy:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    mov r12, rdi             ; r12 = pointer to destination struct
    mov r13, rsi             ; r13 = pointer to source struct
    ; --- Step 1: Free the destination's old memory block ---
    mov rdi, [r12 + DynamicArray.pointer]
    cmp rdi, 0
    je .copy_skip_munmap_fix 
    mov rsi, [r12 + DynamicArray.capacity]
    imul rsi, 8
    mov rax, 11
    syscall
.copy_skip_munmap_fix:
    ; --- Step 2: Set new size and capacity ---
    mov rbx, [r13 + DynamicArray.size] 
    mov [r12 + DynamicArray.size], rbx
    mov [r12 + DynamicArray.capacity], rbx 
    ; --- Step 3: Allocate new memory ---
    cmp rbx, 0
    je .copy_epilogue_fix      
    
    mov r14, rbx             ; Save the size in a safe register
    imul r14, 8              ; r14 holds bytes needed
    ; ======================= THE CRITICAL FIX IS HERE =========================
    ; Syscalls clobber registers. We need r12 and r13 after. Save them.
    push r12
    push r13
    
    mov rax, 9               ; mmap syscall
    mov rdi, 0
    mov rsi, r14             ; Use the size from our safe register
    mov rdx, 3
    mov r10, 34
    mov r8, -1
    mov r9, 0
    syscall
    
    ; Restore our precious struct pointers immediately after the syscall.
    pop r13
    pop r12
    ; ========================================================================
    
    cmp rax, 0
    jl .copy_epilogue_fix    
    mov [r12 + DynamicArray.pointer], rax ; Update destination pointer
    ; --- Step 4: Copy the data ---
    mov rcx, [r13 + DynamicArray.size] 
    mov rdi, [r12 + DynamicArray.pointer] 
    mov rsi, [r13 + DynamicArray.pointer] 
    rep movsq                
.copy_epilogue_fix:
    pop r15
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
; =============================================================================
; HTLL DYNAMIC FILE I/O LIBRARY - POINTER-BASED VERSIONS
; These functions are the key to building a truly dynamic compiler.
; =============================================================================
; =============================================================================
; =============================================================================
; file_read_from_ptr: Appends a file's contents to a dynamic array.
; [In]
;   rdi - Pointer to the destination DynamicArray struct.
;   rsi - A POINTER to a null-terminated string containing the file path.
; =============================================================================
fileread_from_ptr:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    mov r12, rdi             ; r12 = pointer to dest array
    mov r13, rsi             ; r13 = A POINTER to the file path
    ; --- Open the file ---
    mov rax, 2               ; sys_open
    mov rdi, r13
    xor rsi, rsi             ; O_RDONLY
    syscall
    
    ; ======================= THE ARMOR PLATE =========================
    ; A valid file descriptor is a small positive number. Errors are negative.
    cmp rax, 0
    jl .read_error_ptr       ; If rax is Less than 0, ABORT.
    ; ===================================================================
    mov r14, rax             ; r14 = file descriptor
.read_loop_ptr:
    mov rax, 0               ; sys_read
    mov rdi, r14
    mov rsi, file_read_buffer
    mov rdx, 4096
    syscall
    cmp rax, 0
    jle .close_and_exit_ptr      
    mov r15, rax             
    mov rsi, file_read_buffer
.append_byte_loop_ptr:
    cmp r15, 0
    je .read_loop_ptr            
    movzx rbx, byte [rsi]    
    
    push rsi                 
    mov rdi, r12             
    mov rsi, rbx             
    call array_append
    pop rsi                  
    inc rsi                  
    dec r15                  
    jmp .append_byte_loop_ptr
.close_and_exit_ptr:
    mov rax, 3               ; sys_close
    mov rdi, r14             
    syscall
    jmp .read_done_ptr
.read_error_ptr:
    ; The open failed. We do nothing. The destination array remains empty.
    ; A real-world version would print an error to stderr.
.read_done_ptr:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret
; =============================================================================
; file_append_from_ptr: Appends an array of character codes to a file.
; [In]
;   rdi - A POINTER to a null-terminated string containing the file path.
;   rsi - Pointer to the source DynamicArray struct.
; =============================================================================
fileappend_from_ptr:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15
    mov r12, rdi             ; r12 = A POINTER to the file path
    mov r13, rsi             ; r13 = pointer to array struct
    mov rcx, [r13 + DynamicArray.size]
    cmp rcx, 0
    je .append_epilogue_ptr  ; If there's nothing to write, just exit
    ; --- Pack the array into a byte buffer on the stack ---
    mov r14, rcx
    mov rbx, rcx
    add rbx, 15
    and rbx, -16
    sub rsp, rbx
    
    mov r15, rsp
    mov rsi, [r13 + DynamicArray.pointer]
    xor rcx, rcx
.pack_loop_ptr:
    cmp rcx, r14
    jge .pack_done_ptr
    mov al, [rsi + rcx*8]
    mov [r15 + rcx], al
    inc rcx
    jmp .pack_loop_ptr
.pack_done_ptr:
    ; --- Open the file ---
    mov rax, 2
    mov rdi, r12
    mov rsi, 1089            ; O_WRONLY | O_CREAT | O_APPEND
    mov rdx, 0x644
    syscall
    ; ======================= THE ARMOR PLATE =========================
    cmp rax, 0
    jl .append_error_ptr     ; If open failed, jump to cleanup.
    ; ===================================================================
    mov r14, rax             ; Save FD
    ; --- Write the packed data ---
    mov rax, 1
    mov rdi, r14
    mov rsi, r15
    mov rdx, [r13 + DynamicArray.size]
    syscall
    ; --- Close the file ---
    mov rax, 3
    mov rdi, r14
    syscall
.append_error_ptr:
    ; This label is jumped to if open fails. We just clean up the stack.
.append_cleanup_ptr:
    add rsp, rbx ; Deallocate stack space
.append_epilogue_ptr:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    pop rbp
    ret
; =============================================================================
; file_delete_from_ptr: Deletes a file from the filesystem.
; [In]
;   rdi - A POINTER to a null-terminated string containing the file path.
; =============================================================================
filedelete_from_ptr:
    push rbp
    mov rbp, rsp
    
    mov rax, 87              ; syscall for sys_unlink
    syscall
    ; For robustness, you could check if rax is 0 (success) or -1 (failure)
    
    pop rbp
    ret
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
; free_packed_string: Frees memory allocated by sys_mmap.
; [In]
;   rdi - Pointer to the memory block.
;   rsi - The size of the memory block in bytes.
; =============================================================================
free_packed_string:
    push rbp
    mov rbp, rsp
    
    ; Robustness check: Don't try to free a null pointer.
    cmp rdi, 0
    je .free_done
    mov rax, 11         ; syscall for sys_munmap
    ; rdi (pointer) and rsi (size) are already set by the caller.
    syscall
.free_done:
    pop rbp
    ret


debug_print_array:
push rbp
mov rbp, rsp
sub rsp, 16
mov rsi, ASM_STR_TEMP_PRINT_1
mov rdx, ASM_STR_TEMP_PRINT_1_len
call print_str
mov rax, [debug_buffer + DynamicArray.size]
mov rdi, rax
mov rsi, 0
call print_number
mov rsi, ASM_STR_TEMP_PRINT_2
mov rdx, ASM_STR_TEMP_PRINT_2_len
call print_str
push r12
push r13
xor r13, r13
mov r12, rax
.loop1_0:
cmp r12, 0
je .loop1_end0
mov rcx, r13

mov rbx, [debug_buffer + DynamicArray.pointer]
mov rax, [rbx + rcx*8]
mov rdi, rax
push rcx
call print_char
pop rcx
.cloop1_end0:
inc r13
dec r12
jmp .loop1_0
.loop1_end0:
pop r13
pop r12
mov rsi, ASM_STR_TEMP_PRINT_3
mov rdx, ASM_STR_TEMP_PRINT_3_len
call print_str
.debug_print_array_return:
add rsp, 16
pop rbp
ret
; =============================================================================
; MAIN PROGRAM ENTRY POINT
; =============================================================================
_start:
push rbp
mov rbp, rsp
and rsp, -16 
mov rdi, args_array
call array_clear
mov r12, [rbp+8]
lea r13, [rbp+16]
mov r14, 1
.arg_loop:
cmp r14, r12
jge .args_done
mov rsi, [r13 + r14*8]
mov rdi, args_array
call array_unpack_from_bytes
mov rdi, args_array
mov rsi, 10
call array_append
inc r14
jmp .arg_loop
.args_done:
mov rsi, ASM_STR_TEMP_PRINT_4
mov rdx, ASM_STR_TEMP_PRINT_4_len
call print_str
mov rdi, debug_buffer
call array_clear
mov rdi, debug_buffer
mov rsi, args_array
call array_copy

call debug_print_array
mov rdi, args_array

call array_pop
mov rdi, args_array
call array_pack_to_bytes
mov [filename_ptr], rax
mov [filename_ptr_size], rdx
mov rdi, my_source_code
mov rsi, [filename_ptr]
call fileread_from_ptr
mov rdi, [filename_ptr]
mov rsi, [filename_ptr_size]
call free_packed_string
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
mov rdi, args_array

call array_pop
mov rdi, args_array

call array_pop
mov rdi, args_array

call array_pop
mov rdi, args_array

call array_pop
mov rsi, ASM_STR_TEMP_PRINT_5
mov rdx, ASM_STR_TEMP_PRINT_5_len
call print_str
mov rdi, debug_buffer
call array_clear
mov rdi, debug_buffer
mov rsi, args_array
call array_copy

call debug_print_array
mov rsi, 's'
mov rdi, args_array

call array_append
mov rsi, ASM_STR_TEMP_PRINT_6
mov rdx, ASM_STR_TEMP_PRINT_6_len
call print_str
mov rdi, debug_buffer
call array_clear
mov rdi, debug_buffer
mov rsi, args_array
call array_copy

call debug_print_array
mov rdi, args_array
call array_pack_to_bytes
mov [filename_ptr], rax
mov [filename_ptr_size], rdx
mov rdi, [filename_ptr]
call filedelete_from_ptr
mov rdi, [filename_ptr]
mov rsi, [filename_ptr_size]
call free_packed_string
mov rdi, args_array
call array_pack_to_bytes
mov [filename_ptr], rax
mov [filename_ptr_size], rdx
mov rdi, [filename_ptr]
mov rsi, my_asm_output
call fileappend_from_ptr
mov rdi, [filename_ptr]
mov rsi, [filename_ptr_size]
call free_packed_string

    ; --- Exit cleanly ---
    mov rsp, rbp
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall
