/*--------------------------------------------------------------------*/
/* bigintaddflat.c                                                           */
/* Author: Valeria Torres-Olivares & Kevin Castro                                                */
/*--------------------------------------------------------------------*/


#include "bigint.h"
#include "bigintprivate.h"
#include <assert.h>
#include <string.h>

/* In lieu of a boolean data type. */
enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

static long BigInt_larger(long lLength1, long lLength2) {
   
   long lLarger;
   if (lLength1 <= lLength2) {
      goto lLength2Larger;
   }
   lLarger = lLength1;

   goto endifbil;

   lLength2Larger:
   lLarger = lLength2;

   endifbil:
   return lLarger;
}

/*--------------------------------------------------------------------*/

/* Assign the sum of oAddend1 and oAddend2 to oSum.  oSum should be
   distinct from oAddend1 and oAddend2.  Return 0 (FALSE) if an
   overflow occurred, and 1 (TRUE) otherwise. */

int BigInt_add(BigInt_T oAddend1, BigInt_T oAddend2, BigInt_T oSum)
{
   unsigned long ulCarry;
   unsigned long ulSum;
   long lIndex;
   long lSumLength;

   assert(oAddend1 != NULL);
   assert(oAddend2 != NULL);
   assert(oSum != NULL);
   assert(oSum != oAddend1);
   assert(oSum != oAddend2);

   /* Determine the larger length. */
   lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength);

   /* Clear oSum's array if necessary. QUESTION idk about this one where would else go*/
   if (oSum->lLength <=lSumLength){
      goto else1bia;
   }
      
   memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));

   else1bia:

   /* Perform the addition. */
   ulCarry = 0;
   lIndex=0;
   forLoopbia:
      if (lIndex >=lSumLength){
         goto endForLoopbia;
      }
      
      ulSum = ulCarry;
      ulCarry = 0;

      ulSum += oAddend1->aulDigits[lIndex];
      if (ulSum >= oAddend1->aulDigits[lIndex]){ /* Check for overflow. */
         goto noOverflow1bia;
      }
      ulCarry = 1;
      noOverflow1bia:

      ulSum += oAddend2->aulDigits[lIndex];
      if (ulSum >= oAddend2->aulDigits[lIndex]) {/* Check for overflow. */
         goto noOverflow2bia;
      }
      ulCarry = 1;

      noOverflow2bia: /* DOUBLE CHECK ??*/

      oSum->aulDigits[lIndex] = ulSum;

      lIndex++;
      goto forLoopbia;
   endForLoopbia:

   /* Check for a carry out of the last "column" of the addition. */
   if (ulCarry != 1) {
      goto else2bia;
   }
   
   if (lSumLength != MAX_DIGITS) {
      goto else3bia;
   }
   return FALSE;

   else3bia:
   oSum->aulDigits[lSumLength] = 1;
   lSumLength++;

   else2bia:

   /* Set the length of the sum. */
   oSum->lLength = lSumLength;

   return TRUE;
}
