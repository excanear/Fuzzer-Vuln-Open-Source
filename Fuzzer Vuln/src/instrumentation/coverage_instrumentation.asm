; coverage_instrumentation.asm
; x86_64 Assembly for basic block instrumentation
; Inspired by AFL's instrumentation

section .data
    extern __afl_area_ptr  ; Pointer to the coverage map (shared memory)

section .text
    global __afl_instrument_basic_block

; Function to instrument a basic block
; This is called at the start of each basic block
; Increments the coverage counter for the current edge
__afl_instrument_basic_block:
    ; Save registers
    push rax
    push rbx

    ; Load the previous location (stored in a global or thread-local)
    ; For simplicity, assume __afl_prev_loc is a global variable
    extern __afl_prev_loc
    mov rax, [__afl_prev_loc]

    ; Calculate the current location (passed as argument or hardcoded)
    ; For this example, assume cur_loc is in rdi
    mov rbx, rdi  ; cur_loc

    ; Update previous location
    mov [__afl_prev_loc], rbx

    ; Compute hash: (prev_loc >> 1) ^ cur_loc
    shr rax, 1
    xor rax, rbx

    ; Get the coverage map pointer
    mov rbx, [__afl_area_ptr]
    test rbx, rbx
    jz .done  ; If null, skip

    ; Increment the counter (with saturation)
    movzx rcx, byte [rbx + rax]
    inc rcx
    cmp rcx, 255
    jle .no_sat
    mov rcx, 255
.no_sat:
    mov [rbx + rax], cl

.done:
    ; Restore registers
    pop rbx
    pop rax
    ret