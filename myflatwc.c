/*--------------------------------------------------------------------*/
/* mywc.c                                                             */
/* Author: Bob Dondero translated into C by
/* Kevin Castro and Valeria Torres-Olivares                                               */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>

/*--------------------------------------------------------------------*/

/* In lieu of a boolean data type. */
enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

static long lLineCount = 0;      /* Bad style. */
static long lWordCount = 0;      /* Bad style. */
static long lCharCount = 0;      /* Bad style. */
static int iChar;                /* Bad style. */
static int iInWord = FALSE;      /* Bad style. */

/*--------------------------------------------------------------------*/

/* Write to stdout counts of how many lines, words, and characters
   are in stdin. A word is a sequence of non-whitespace characters.
   Whitespace is defined by the isspace() function. Return 0. */


int main(void)
{
   whileLoop:
      if ((iChar = getchar()) == EOF)
      {
         goto endWhileLoop;
      }

      lCharCount++;
      
      if (!isspace(iChar))
      {
         goto else1;
      }
      
      if (!iInWord)
      {
         goto endIfNotInWord;
      }
      lWordCount++;
      iInWord = FALSE;

      endIfNotInWord:

      goto endIf1;
      else1: 
      
      if (iInWord){
         goto endIfInWord;
      }
      iInWord = TRUE;

      endIfInWord:
         
      endIf1:

      if (iChar != '\n'){
         goto endIfNewLine;
      }
      lLineCount++;

      endIfNewLine:
            
      goto whileLoop;
   endWhileLoop:

   if (!iInWord){
      goto endIfNotInWord2;
   }
   lWordCount++;

   endIfNotInWord2:
      
   printf("%7ld %7ld %7ld\n", lLineCount, lWordCount, lCharCount);
   return 0;
}