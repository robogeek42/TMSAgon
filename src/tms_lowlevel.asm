; ASM

    .global _load_mode


PC_DR:		equ 	0x9E
PC_DDR:		equ 	0x9F
PD_DR:		equ 	0xA2
PD_DDR:		equ 	0xA3

    .assume ADL=1
    .text

; -------------------------------------------------

_load_mode: 
        ; takes mode number
	push ix 			; push ix onto the stack and allocate local frame
	ld ix, 0 			;	
	add ix, sp 			; set ix to sp

        ld de, (ix+6)                   ; load mode variable into de
       
        ; ignore mode for now, assume 0
        ;ld a,e 
        ;sla a
        ;sla a
        ;sla a                           ; Multiply by 8 to get offset
        
        ld hl, mode_data                ; load address of mode-data
        ;ld de, 0
        ;ld e, a
        ;add hl, de                      ; move by offset

        ld b,8                          ; b is counter
        ld c, 0b10000000                ; c register has second byte of command (increments)
        ld a, 0b00000000                ; set DDR portC = 0 (write)
        out0 (PC_DDR), a

        ld d, 0b10000000                ; d is write reg signal value (MODE=1, EN=0, RWB=0)
        ld e, 0b01110000                ; e is signal reset value
loop:
        ld a, (hl)                      ; get the data to put in VDP regsiter

        out0 (PC_DR), a                 ; write out data

        ; toggle the signals
        out0 (PD_DR), d                 ; set signals
        call delay2us
        out0 (PD_DR), e                 ; reset signals
        call delay2us

        out0 (PC_DR), c                 ; write out second byte = 0x80 | reg-num

        ; toggle the signals
        out0 (PD_DR), d                 ; set signals
        call delay2us
        out0 (PD_DR), e                 ; reset signals
        call delay2us

        inc hl
        inc c
        djnz loop
       
        ld sp,ix
        pop ix

        ret 

delay2us:
        push de
        ld e, 10
dloop:
        nop
        nop
        dec e
        jr nz,dloop
        pop de
        ret

mode_data:
        .db $00, $D0, $02, $00, $00, $00, $00, $F4
        .db $00, $C0, $02, $30, $00, $28, $02, $CF 
        .db $02, $C0, $06, $FF, $03, $78, $07, $F4 
        .db $00, $C8, $02, $00, $00, $16, $02, $F0 
    
