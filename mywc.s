//----------------------------------------------------------------------
// mywc.s
// Author: Kevin Castro and Valeria Torres-Olivares
//----------------------------------------------------------------------

    .equ FALSE, 0 

    .equ TRUE, 1

    .equ EOF, -1

    .equ NEW_LINE, '\n'

//----------------------------------------------------------------------

        .section .rodata

printfFormatStr:
        .string "%7ld %7ld %7ld\n"

//----------------------------------------------------------------------

        .section .data


lLineCount:
        .quad 0

lWordCount:
        .quad 0

lCharCount:
        .quad 0

iInWord:
        .word FALSE


//----------------------------------------------------------------------

        .section .bss

iChar:
        .skip 4


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

main:

        // Prolog
        sub     sp, sp, MAIN_STACK_BYTECOUNT
        str     x30, [sp]

whileLoop:

            // if ((iChar = getchar()) == EOF) goto endWhileLoop;
            
            bl getchar
            adr x3, iChar
            str w0, [x3]
            cmp w0, EOF
            beq endWhileLoop

            // lCharCount++;
            adr x0, lCharCount
            ldr x1, [x0]
            add x1, x1, 1
            str x1, [x0]

            // if (!isspace(iChar)) goto else1;
            adr x0, iChar
            ldr x0, [x0]
            bl isspace
            cmp w0, FALSE
            beq else1

            // if(!iInWord) goto endIfNotInWord;
            adr x0,iInWord
            cmp x0, FALSE
            beq endIfNotInWord

            // lWordCount++;
            adr x0, lWordCount
            ldr x2, [x0]
            add x2, x2, 1
            str x2, [x0]

            //iInWord = FALSE;
            mov x0, FALSE
            adr x3, iInWord
            str x0,[x3]

endIfNotInWord:
            // goto endIf1;
            b endIf1
else1:  

            //if (iInWord) goto endIfInWord;
            adr x0,iInWord
            cmp x0, FALSE
            beq endIfInWord

            // iInWord = TRUE;
            mov x0, TRUE
            adr x3, iInWord
            str x0, [x3]

endIfInWord:

endIf1:

            // if(iChar != '\n') goto endIfNewLine;
            adr x0, iChar
            ldr x0, [x0]
            cmp x0, NEW_LINE
            bne endIfNewLine

            // lLineCount++;
            adr x0, lLineCount
            ldr w2, [x0]
            add w2, w2, 1
            str w2, [x0]

endIfNewLine:

            b whileLoop
        
endWhileLoop:

        // if(!iInWord) goto endIfNotInWord2
        adr x0,iInWord
        ldr w1, [x0]
        cmp w1, FALSE
        beq endIfNotInWord2

        // lWordCount++
        adr x0, lWordCount
        ldr x1, [x0]
        add x1, x1, 1
        str x1, [x0]


endIfNotInWord2:

        adr x0, printfFormatStr
        adr x1, lLineCount
        ldr x1, [x1]
        adr x2, lWordCount
        ldr x2, [x2]
        adr x3, lCharCount
        ldr x3, [x3]
        bl printf

        mov  w0, 0
        ldr x30, [sp]
        add sp, sp, MAIN_STACK_BYTECOUNT

        .size   main, (. - main)
        