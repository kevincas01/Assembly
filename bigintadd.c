/*--------------------------------------------------------------------*/
/* bigint.c                                                           */
/* Author: Valeria Torres-Olivares & Kevin Castro                                                */
/*--------------------------------------------------------------------*/

#include "bigint.h"
#include "bigintprivate.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

/*--------------------------------------------------------------------*/

static long BigInt_larger(long lLength1, long lLength2) {
   
   long lLarger;
   if (lLength1 < lLength2) {
      goto lLength1Larger;
   }
   lLarger = lLength2;

   lLength1Larger:
   lLarger = lLength1;

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
   if (oSum->lLength < lSumLength)
      goto else1;
   memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));

   /* would else1 go here??? */

   /* Perform the addition. */
   ulCarry = 0;

   forLoop:
      if (!lIndex < lSumLength)
         goto endForLoop;
      
      ulSum = ulCarry;
      ulCarry = 0;

      ulSum += oAddend1->aulDigits[lIndex];
      if (ulSum > oAddend1->aulDigits[lIndex]) /* Check for overflow. */
         goto noOverflow;
      ulCarry = 1;

      ulSum += oAddend2->aulDigits[lIndex];
      if (ulSum > oAddend2->aulDigits[lIndex]) /* Check for overflow. */
         goto noOverflow;
      ulCarry = 1;

      noOverflow: /* DOUBLE CHECK ??*/

      oSum->aulDigits[lIndex] = ulSum;

      lIndex++;
      goto forLoop;
   
   endForLoop:

   /* Check for a carry out of the last "column" of the addition. */
   if (ulCarry != 1) {
      goto else1:
   }
   if (lSumLength != MAX_DIGITS)
      goto else2;

   return FALSE;
   oSum->aulDigits[lSumLength] = 1;
   lSumLength++;

   else2:
   /* IDK ??*/

   else1:
   /* IDK ??*/

   /* Set the length of the sum. */
   oSum->lLength = lSumLength;

   return TRUE;
}
