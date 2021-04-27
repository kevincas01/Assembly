//----------------------------------------------------------------------
// bigintaddopt.s
// Author: Kevin Castro and Valeria Torres-Olivares
//----------------------------------------------------------------------

        .equ FALSE, 0 

        .equ TRUE, 1


//----------------------------------------------------------------------

        .section .rodata


//----------------------------------------------------------------------

        .section .data



//----------------------------------------------------------------------

        .section .bss



//----------------------------------------------------------------------

        .section .text

        //--------------------------------------------------------------
        // Write to stdout counts of how many lines, words, and characters
        // are in stdin. A word is a sequence of non-whitespace characters.
        // Whitespace is defined by the isspace() function. Return 0. 
        //--------------------------------------------------------------

        // Must be a multiple of 16

        // Stack offsets 
        .equ INT_ADD_STACK_BYTECOUNT, 80

        // Struct offset 

        .equ lLength, 0

        .equ aulDigits, 8

        // Max Digits
        .equ maxDigits, 32768

        // Registers for parameters 
        OADDEND1 .req x19
        OADDEND2 .req x20
        OSUM .req x21

        // Registers for local variables
        
        ULSUM  .req x22
        LINDEX .req x23
        LSUMLENGTH .req x24
        LLARGER .req x25

        .global BigInt_add 

BigInt_add:
        sub sp, sp, INT_ADD_STACK_BYTECOUNT

        str x30, [sp] //return address
        str x19,[sp, 8] // Save x19
        str x20,[sp, 16] // Save x20
        str x21,[sp, 24] // Save x21
        str x22,[sp, 32] // Save x22
        str x23,[sp, 40] // Save x23
        str x24,[sp, 48] // Save x24
        str x25,[sp, 56] // Save x25\
        mov OADDEND1,  x0
        mov OADDEND2, x1
        mov OSUM, x2

        // lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength);

        // if (lLength1 <= lLength2) goto lLength2Larger;
        ldr x0,  [OADDEND1, lLength] // lLength1
        ldr x1, [OADDEND2, lLength] // lLength2
        cmp x0, x1
        ble lLength2Larger

        // lLarger = lLength1;
        ldr x0,  [OADDEND1, lLength] // lLength1
        mov LLARGER, x0

        b endIfbil

lLength2Larger:
        // lLarger = lLength2;
        ldr x1, [OADDEND2, lLength] // lLength2
        mov LLARGER, x1

endIfbil:
        mov LSUMLENGTH, LLARGER
        
        // if (oSum->lLength <=lSumLength) goto else1bia;
        ldr x1, [OSUM,lLength]
        cmp x1, LSUMLENGTH
        ble else1bia

        // memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));
        add x0,  OSUM, aulDigits
        mov w1, 0
        mov x2, maxDigits
        lsl x2, x2, 3
        bl memset

else1bia:

        // ulCarry = 0;
        clc

        // lIndex=0;
        mov LINDEX, 0
        

        // if (lIndex >= lSumLength) goto forLoopbia;
        cmp LINDEX, LSUMLENGTH
        bge endForLoopbia


forLoopbia:
        // ulSum = ulCarry;
        bcc carry0
        
        //if carry flag is set, move 1  into  ULSUM
        mov ULSUM, 1
        b end
carry0:
        //if carry flag is not set,move 0  into  ULSUM
        mov ULSUM, 0

end:

        // ulCarry = 0;
        clc

        // ulSum += oAddend1->aulDigits[lIndex];
        add x1,  OADDEND1, aulDigits
        ldr x2, [x1, LINDEX, lsl 3]
        adcs ULSUM, ULSUM, x2

        // ulSum += oAddend2->aulDigits[lIndex];
        add x1,  OADDEND2, aulDigits
        ldr x2, [x1, LINDEX, lsl 3]
        adcs ULSUM, ULSUM, x2

        // oSum->aulDigits[lIndex] = ulSum;
        add x1, OSUM, aulDigits
        str ULSUM, [x1, LINDEX, lsl 3]
        
        // lIndex++
        add LINDEX, LINDEX, 1

        // if (lIndex < lSumLength) goto forLoopbia;
        cmp LINDEX, LSUMLENGTH
        blt forLoopbia

endForLoopbia:

        // if (ulCarry != 1) goto else2bia;
        bcc else2bia

        // if (lSumLength != MAX_DIGITS) goto else3bia;
        cmp LSUMLENGTH, maxDigits
        bne else3bia

        // Epilogue & return false
        mov w0, FALSE
        ldr x30, [sp]
        ldr x19,[sp, 8] // Restore x19
        ldr x20,[sp, 16] // Restore x20
        ldr x21,[sp, 24] // Restore x21
        ldr x22,[sp, 32] // Restore x22
        ldr x23,[sp, 40] // Restore x23
        ldr x24,[sp, 48] // Restore x24
        ldr x25,[sp, 56] // Restore x25
        add sp, sp, INT_ADD_STACK_BYTECOUNT
        ret 

else3bia:

        // oSum->aulDigits[lIndex] = 1;
        add x1, OSUM, aulDigits
        mov x2, 1
        str x2, [x1, LINDEX, lsl 3]

        // lSumLength++
        add LSUMLENGTH, LSUMLENGTH, 1

else2bia:

        // oSum->lLength = lSumLength;
        str LSUMLENGTH, [OSUM, lLength]

        // Epilogue & return TRUE
        mov w0, TRUE
        ldr x30, [sp] // Restore return address
        ldr x19,[sp, 8] // Restore x19
        ldr x20,[sp, 16] // Restore x20
        ldr x21,[sp, 24] // Restore x21
        ldr x22,[sp, 32] // Restore x22
        ldr x23,[sp, 40] // Restore x23
        ldr x24,[sp, 48] // Restore x24
        ldr x25,[sp, 56] // Restore x25
        add sp, sp, INT_ADD_STACK_BYTECOUNT
        ret 