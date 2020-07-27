MB2_ARCH  EQU 0                                 ; 0 = x86/x86-64
MB2_LEN   EQU (mbend-mbhead)
MB2_MAGIC EQU 0xe85250d6

section .multiboot align=4096
mbhead:
        dd MB2_MAGIC                            ; Multiboot2 magic number
        dd MB2_ARCH                             ; Architecture
        dd MB2_LEN                              ; Multiboot header length
        dd 0x100000000 - MB2_LEN - MB2_ARCH - MB2_MAGIC
                                                ; Checksum

        align 8
mb2_tag_end_start:
        dw 0                                    ; last tag
        dw 0
        dd mb2_tag_end_end - mb2_tag_end_start
mb2_tag_end_end:

mbend: