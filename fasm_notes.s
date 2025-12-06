format ELF64
public _start
extrn compiler_c
extrn free_string_c

; =================================================================
; 1. DEFINE CONSTANTS FOR STRUCT OFFSETS
; We do this so we don't have to deal with FASM's complex struct macros.
; =================================================================
DynamicArray.pointer  = 0
DynamicArray.size     = 8
DynamicArray.capacity = 16
sizeof.DynamicArray   = 24

; =================================================================
; 2. DATA SECTION
; =================================================================
section '.data' writeable
    SCALE_FACTOR   dq 1000000
    INITIAL_CAPACITY = 2
    print_buffer   rb 21
    dot            db '.'
    minus_sign     db '-'
    nl             db 10

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

; =================================================================
; 3. BSS SECTION (THE FIX)
; We use 'rq 3' for structs so they are treated as 64-bit (Quadword) data.
; This fixes the "Operand sizes do not match" error.
; =================================================================
section '.bss' writeable
    input_buffer       rb 256
    file_read_buffer   rb 4096
    input_len          rq 1
    filename_ptr_size  rq 1
    source_ptr         rq 1
    source_ptr_size    rq 1
    
    ; FIXED: Changed from 'rb 24' to 'rq 3'
    args_array         rq 3       
    
    filename_ptr       rq 1
    asm_code_ptr       rq 1
    print_buffer_n     rb 20
    
    ; FIXED: Changed from 'rb 24' to 'rq 3'
    debug_buffer       rq 3       
    my_source_code     rq 3       
    my_asm_output      rq 3       

section '.text' executable
