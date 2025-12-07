format ELF64
public _start

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


ASM_STR_TEMP_PRINT_1 db "Hello Man", 10
ASM_STR_TEMP_PRINT_1_len = $-ASM_STR_TEMP_PRINT_1



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



section '.text' executable

print_str:
    push rbp
    mov rbp, rsp
    mov rax, 1
    mov rdi, 1
    ; rsi and rdx are expected to be set by the caller
    syscall
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

mov rsi, ASM_STR_TEMP_PRINT_1
mov rdx, ASM_STR_TEMP_PRINT_1_len
call print_str

    ; --- Exit cleanly ---
    mov rsp, rbp
    pop rbp
    mov rax, 60
    xor rdi, rdi
    syscall
