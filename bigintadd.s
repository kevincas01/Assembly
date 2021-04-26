//----------------------------------------------------------------------
// mywc.s
// Author: Kevin Castro and Valeria Torres-Olivares
// NOTE THIS IS NOT IT EXCEPT FOR THE DECLARATIONS
//----------------------------------------------------------------------

        .equ FALSE, 0 

        .equ TRUE, 1

        .equ OADDEND1, 48

        .equ ULCARRY, 24

        .equ LLENGTH, 0

        .equ SIZEOFU, 4

//----------------------------------------------------------------------

        .section .rodata

printfFormatStr:
        .string "%7ld %7ld %7ld\n"

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
        .equ    MAIN_STACK_BYTECOUNT, 16 

        .global main


BigInt_larger:
        sub sp, sp, ULCARRY

        str x30, [sp] //return address
        str x0,[sp,16] //Save lLength1
        str x1,[sp,8] // Save lLength2


        // if (lLength1 <= lLength2) goto lLength2Larger;
        ldr x1, [sp,16]
        ldr x2, [sp,8]
        cmp x1, x2
        ble lLength2Larger

        // lLarger = lLength1;
        ldr x1, [sp,16]
        str x1,[sp,24]

        b endIfBil

lLength2Larger:
        // lLarger = lLength2;
        ldr x1, [sp,8]
        str x1,[sp,24]


endIfBil:
        //return lLarger
        ldr x30, [sp,24]
        ret




BigInt_add:
        sub sp, sp, OADDEND1

        // how much space do we need to subtract from sp??
???????????????????????????????????????????????????????????????????????

        str x30, [sp] //return address
        str x0,[sp,24] //Save oSum
        str x1,[sp,16] // Save oAddend2
        str x2,[sp,8] //save oAddend1


        // BigInt_larger(oAddend1->lLength, oAddend2->lLength);

        //How can we access the values of a structure

        adr x0, [sp, 8 ]  // Load oAddend1
        ldr x0, [x0, OFFSET ]
 ???????????????????????????????????????????????????????????????????????
        ldr x0, [sp, 16]  // Load oAddend2
  ???????????????????????????????????????????????????????????????????????
        bl BigInt_larger
        str x0, [sp, 56]  // Store lSumLength


        // lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength);


        // if (oSum->lLength <=lSumLength) goto else1bia;
   
        ldr x1, [sp,lLength for osum length address]
        ldr x2, [sp, 56]
        cmp x1,x2
        // SIGNED OR UNSIGNED????????????????????????????????????????????????????
        ble  else1bia


        // memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));
        ldr x0, [sp, address for auDigits] //???????????????????
        mov x1, 0
        mov x2, MAX_DIGITS
        mul x2, x2, SIZEOFU

        bl memset


else1bia:

        // ulCarry = 0;
        mov x1,0
        str x1, [sp,32]

        // lIndex=0;
        mov x1,0
        str x1, [sp,48]

        
forLoopbia:

        // if (lIndex >=lSumLength) goto endForLoopbia;
        ldr x1, [sp,48]
        ldr x2, [sp,56]
        cmp x1, x2
        bge endForLoopbia

        // ulSum = ulCarry;
        ldr x0, [sp, 32]
        str x0, [sp,40]

        // ulCarry = 0;
        mov x1,0
        str x1, [sp,32]

        // ulSum += oAddend1->aulDigits[lIndex];
        ldr x1, [sp, 40]
        ldr x2, [sp, auDigits array at indeexlIndex address ]
        add x1, x1, x2
        str x1, [sp, 40]

        // if (ulSum >= oAddend1->aulDigits[lIndex]) goto noOverflow1bia;

        ldr x1, [sp,40]
        ldr x2, [sp, oAddend1->aulDigits[lIndex] address]
        cmp x1, x2
        bge noOverflow1bia

        // ulCarry = 1;
        mov x1,1
        str x1, [sp,32]

noOverflow1bia:


        // ulSum += oAddend12->aulDigits[lIndex];
        ldr x1, [sp, 40]
        ldr x2, [sp, auDigits array at indeexlIndex address ]
        add x1, x1, x2
        str x1, [sp, 40]

        // if (ulSum >= oAddend2->aulDigits[lIndex]) goto noOverflow1bia;

        ldr x1, [sp,40]
        ldr x2, [sp, oAddend2->aulDigits[lIndex] address]
        cmp x1, x2
        bge noOverflow2bia

        // ulCarry = 1;
        mov x1,1
        str x1, [sp,32]

noOverflow2bia:


        // oSum->aulDigits[lIndex] = ulSum;
        ldr x1, [sp, 40]
        str x1 [sp,oSum->aulDigits[lIndex]] ???????????????????????????

        // lIndex++
        ldr x1, [sp, 48]
        add x1, x1, 1
        str x1,[sp, 48]

        b forLoopbia

endForLoopbia:
        // if (ulCarry != 1) goto else2bia;
        ldr x1, [sp, 32]
        cmp x1, TRUE 
        bne else2bia

        // if (lSumLength != MAX_DIGITS) goto else3bia;
        ldr x1, [sp, 56]
        cmp x1, MAX_DIGITS
        bne else3bia

        // return false
        mov x30, FALSE
        ret 

else3bia:

        // oSum->aulDigits[lIndex] = 1;
        mov x1, TRUE
        str x1 [sp,oSum->aulDigits[lIndex]] ???????????????????????????


        // lSumLength++
        ldr x1, [sp, 56]
        add x1, x1, 1
        str x1, [sp, 56]

else2bia:

        // oSum->lLength = lSumLength;

        ldr x1, [sp, 56]
        str x1 [sp,oSum->lLength address] ???????????????????????????


        // Epilog
        //return TRUE


        mov x30, TRUE
        ret 




