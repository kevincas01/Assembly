//----------------------------------------------------------------------
// bigintadd.s
// Author: Kevin Castro and Valeria Torres-Olivares
// NOTE THIS IS NOT IT EXCEPT FOR THE DECLARATIONS
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
        .equ INT_LARGER_STACK_BYTECOUNT, 32

        // Argument offsets
        .equ lLength1, 8

        .equ lLength2, 16

        // Local variable offset
        .equ lLarger, 24


BigInt_larger:
        sub sp, sp, INT_LARGER_STACK_BYTECOUNT

        str x30, [sp] //return address
        str x0,[sp,lLength1] //Save lLength1
        str x1,[sp,lLength2] // Save lLength2

        // if (lLength1 <= lLength2) goto lLength2Larger;
        ldr x1, [sp,lLength1]
        ldr x2, [sp,lLength2]
        cmp x1, x2
        ble lLength2Larger

        // lLarger = lLength1;
        ldr x1, [sp,lLength]
        str x1,[sp,lLarger]

        b endIfBil

lLength2Larger:
        // lLarger = lLength2;
        ldr x1, [sp,lLength2]
        str x1,[sp, lLarger]


endIfBil:
        //return lLarger
        ldr x0, [sp,lLarger]
        ldr x30, [sp]
        add sp, sp, INT_LARGER_STACK_BYTECOUNT
        ret




        // Stack offsets 
        .equ INT_ADD_STACK_BYTECOUNT, 64

        // Arguments in BigInt_add
        .equ oAddend1, 8

        .equ oAddend2, 16

        .equ oSum, 24

        // Local variables

        .equ  ulCarry, 32

        .equ ulSum, 40

        .equ lIndex, 48

        .equ lSumLength, 56

        // Struct offset 

        .equ lLength, 0

        .equ aulDigits, 8


        // Max Digits
        .equ maxDigits, 32768

        // Size of unsigned long 
        .equ SIZEOFU, 8


        .global BigInt_add
BigInt_add:
        sub sp, sp, INT_ADD_STACK_BYTECOUNT

        str x30, [sp] //return address
        str x0,[sp,oAddend1] // Save oAddend1
        str x1,[sp,oAddend2] // Save oAddend2
        str x2,[sp,oSum] // Save oSum


        // lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength);
        
        ldr x0, [sp, oAddend1] 
        ldr x0,  [x0,lLength]
        ldr x1, [sp, oAddend2]
        ldr x1, [x1, lLength]

        bl BigInt_larger
        str x0, [sp, lSumLength]  // Store lSumLength


        // if (oSum->lLength <=lSumLength) goto else1bia;
        ldr x1, [sp,oSum]
        ldr x1, [x1, lLength]
        ldr x2, [sp, lSumLength]
        cmp x1,x2
        ble else1bia

        // memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));
        ldr x0, [sp, oSum]
        add x0,  x0, aulDigits
        mov w1, 0
        mov x2, maxDigits
        mov x3, SIZEOFU
        mul x2, x2, x3
        bl memset

else1bia:

        // ulCarry = 0;
        mov x1,0
        str x1, [sp,ulCarry]

        // lIndex=0;
        mov x1, 0
        str x1, [sp,lIndex]
        
forLoopbia:
        // if (lIndex >=lSumLength) goto endForLoopbia;
        ldr x1, [sp,lIndex]
        ldr x2, [sp,lSumLength]
        cmp x1, x2
        bge endForLoopbia

        // ulSum = ulCarry;
        ldr x0, [sp, ulCarry]
        str x0, [sp,ulSum]

        // ulCarry = 0;
        mov x1, 0
        str x1, [sp, ulCarry]

        // ulSum += oAddend1->aulDigits[lIndex];
        ldr x1, [sp, oAddend1]
        add x1,  x1, aulDigits
        ldr x2, [sp, lIndex]

        ldr x3, [x1, x2, lsl 3]
        
        ldr x4, [sp, ulSum]
        add x4, x4, x3
        str x4, [sp, ulSum]

        // if (ulSum >= oAddend1->aulDigits[lIndex]) goto noOverflow1bia;
        ldr x1, [sp,oAddend1]
        add x1, x1, aulDigits
        ldr x2, [sp, lIndex]
        ldr x3, [x1, x2, lsl 3]
        
        ldr x4,[sp, ulSum]
        cmp x4, x3
        bhs noOverflow1bia

        // ulCarry = 1;
        mov x1,1
        str x1, [sp,ulCarry]

noOverflow1bia:


        // ulSum += oAddend12->aulDigits[lIndex];
        ldr x1, [sp, oAddend2]
        add x1,  x1, aulDigits
        ldr x2, [sp, lIndex]

        ldr x3, [x1, x2, lsl 3]
        
        ldr x4, [sp, ulSum]
        add x4, x4, x3
        str x4, [sp, ulSum]

        // if (ulSum >= oAddend2->aulDigits[lIndex]) goto noOverflow2bia;
        ldr x1, [sp,oAddend2]
        add x1, x1, aulDigits
        ldr x2, [sp, lIndex]
        ldr x3, [x1, x2, lsl 3]
        
        ldr x4,[sp, ulSum]
        cmp x4, x3
        bhs noOverflow2bia

        // ulCarry = 1;
        mov x1,1
        str x1, [sp, ulCarry]

noOverflow2bia:


        // oSum->aulDigits[lIndex] = ulSum;
        ldr x1, [sp,oSum]
        add x1, x1, aulDigits
        ldr x2, [sp, lIndex]

        ldr x3, [sp, ulSum]
        str x3, [x1, x2, lsl 3]
        
        // lIndex++
        ldr x1, [sp, lIndex]
        add x1, x1, 1
        str x1,[sp, lIndex]

        b forLoopbia

endForLoopbia:
        // if (ulCarry != 1) goto else2bia;
        ldr x1, [sp, ulCarry]
        cmp x1, TRUE 
        bne else2bia

        // if (lSumLength != MAX_DIGITS) goto else3bia;
        ldr x1, [sp, lSumLength]
        cmp x1, maxDigits
        bne else3bia

        // Epilogue & return false
        mov w0, FALSE
        ldr x30, [sp]
        add sp, sp, INT_ADD_STACK_BYTECOUNT
        ret 

else3bia:

        // oSum->aulDigits[lIndex] = 1;
        ldr x1, [sp,oSum]
        add x1, x1, aulDigits
        ldr x2, [sp, lIndex]

        mov x3, 1
        str x3, [x1, x2, lsl 3]

        // lSumLength++
        ldr x1, [sp, lSumLength]
        add x1, x1, 1
        str x1, [sp, lSumLength]

else2bia:

        // oSum->lLength = lSumLength;
        ldr x1, [sp, oSum]
        ldr x2, [sp, lSumLength]
        str x2, [x1,lLength]

        // Epilogue & return TRUE
        mov w0, TRUE
        ldr x30, [sp]
        add sp, sp, INT_ADD_STACK_BYTECOUNT
        ret 




