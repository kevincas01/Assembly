/*--------------------------------------------------------------------*/
/* bigint.c                                                           */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include "bigint.h"
#include "bigintprivate.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

/*--------------------------------------------------------------------*/

BigInt_T BigInt_new(unsigned long ulValue)
{
   BigInt_T oBigInt;

   oBigInt = (BigInt_T)calloc(1, sizeof(struct BigInt));
   if (oBigInt == NULL)
      return NULL;

   if (ulValue > 0)
   {
      oBigInt->lLength = 1;
      oBigInt->aulDigits[0] = ulValue;
   }

   return oBigInt;
}

/*--------------------------------------------------------------------*/

void BigInt_free(BigInt_T oBigInt)
{
   assert(oBigInt != NULL);
   free(oBigInt);
}

/*--------------------------------------------------------------------*/

/* Return 1 (TRUE) if and only if pcStr is a string of hexadecimal
   digits. */

static int BigInt_isHexString(const char *pcStr)
{
   assert(pcStr != NULL);
   
   if (*pcStr == '\0')
      return 0;
   for ( ; *pcStr != '\0'; pcStr++)
      if (! isxdigit(*pcStr))
         return 0;
   return 1;
}

/*--------------------------------------------------------------------*/

/* Append the hexadecimal digit ulDigit to oBigInt. */
 
static void BigInt_append(BigInt_T oBigInt, unsigned long ulDigit)
{ 
   const unsigned long BITS_PER_HEX_DIGIT = 4;
   const unsigned long BITS_PER_ULONG = 
      sizeof(unsigned long) * CHAR_BIT; /* 64 */
   
   unsigned long ulCarry;
   unsigned long ulNextCarry;
   long lIndex;
   
   ulCarry = ulDigit;
   for (lIndex = 0; lIndex < oBigInt->lLength; lIndex++)
   {
      ulNextCarry = oBigInt->aulDigits[lIndex] >> 
         (BITS_PER_ULONG - BITS_PER_HEX_DIGIT);
      oBigInt->aulDigits[lIndex] <<= BITS_PER_HEX_DIGIT;
      oBigInt->aulDigits[lIndex] |= ulCarry;
      ulCarry = ulNextCarry;
   }
   
   if (ulCarry != 0)
   {
      oBigInt->aulDigits[lIndex] = ulCarry;
      oBigInt->lLength++;
   }
}

/*--------------------------------------------------------------------*/

int BigInt_assignFromHexStr(BigInt_T oBigInt, const char *pcHexStr)
{
   char cDigit;
   unsigned long ulDigit;

   assert(oBigInt != NULL);
   assert(pcHexStr != NULL);

   /* Validate pcHexStr. */
   if (! BigInt_isHexString(pcHexStr))
      return 0;

   /* Clear oBigInt. */
   oBigInt->lLength = 0;
   memset(oBigInt->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));
   
   /* One by one append the hexadecimal digits of pcHexStr to oBigInt.*/
   for ( ; *pcHexStr != '\0'; pcHexStr++)
   {
      cDigit = *pcHexStr;
      if ((cDigit >= '0') && (cDigit <= '9'))
         ulDigit = (unsigned long)cDigit - (unsigned long)'0';
      else if ((cDigit >= 'a') && (cDigit <= 'f'))
         ulDigit = (unsigned long)cDigit - (unsigned long)'a' + 0xa;
      else
         ulDigit = (unsigned long)cDigit - (unsigned long)'A' + 0xA;
      BigInt_append(oBigInt, ulDigit);
   }
   
   return 1;
}

/*--------------------------------------------------------------------*/

void BigInt_largest(BigInt_T oBigInt)
{
   long lIndex;
   
   assert(oBigInt != NULL);
   
   for (lIndex = 0; lIndex < MAX_DIGITS; lIndex++)
      oBigInt->aulDigits[lIndex] = ULONG_MAX;
   oBigInt->lLength = MAX_DIGITS;
}

/*--------------------------------------------------------------------*/

/* Return a random unsigned long. */

static unsigned long BigInt_randomUL(void)
{
   const unsigned long BYTE_MASK = 0xFF;
   
   unsigned long ulRandom = 0;
   unsigned long ulRandomByte;
   size_t u;
   
   /* Concatenate the appropriate number of random bytes. */
   for (u = 0; u < sizeof(unsigned long); u++)
   {
      ulRandomByte = (unsigned long)rand() & BYTE_MASK;
      ulRandom = (ulRandom << CHAR_BIT) | ulRandomByte;
   }
      
   return ulRandom;
}

/*--------------------------------------------------------------------*/

void BigInt_random(BigInt_T oBigInt)
{
   long lIndex;
   long lNewLength;

   assert(oBigInt != NULL);

   /* Clear oBigInt. */
   oBigInt->lLength = 0;
   memset(oBigInt->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));

   /* Choose a random number of digits. */
   lNewLength = (long)(rand() % MAX_DIGITS);
   
   /* Assign random unsigned longs to the digits of oBigInt. */
   for (lIndex = 0; lIndex < lNewLength; lIndex++)
      oBigInt->aulDigits[lIndex] = BigInt_randomUL();
   
   /* Account for the small possibility of leading zeros. */
   for (;;)
   {
      if (lNewLength == 0)
         break;
      if (oBigInt->aulDigits[lNewLength-1] != 0)
         break;
      lNewLength--;
   }
      
   oBigInt->lLength = lNewLength;
}  

/*--------------------------------------------------------------------*/

void BigInt_writeHex(FILE *psFile, BigInt_T oBigInt)
{
   long lIndex;

   assert(psFile != NULL);
   assert(oBigInt != NULL);

   if (oBigInt->lLength == 0)
      fprintf(psFile, "%016lx", 0UL);
   else
   {
      for (lIndex = oBigInt->lLength-1; lIndex >= 0; lIndex--)
         fprintf(psFile, "%016lx", oBigInt->aulDigits[lIndex]);
   }
}

/*--------------------------------------------------------------------*/

void BigInt_writeHexAbbrev(FILE *psFile, BigInt_T oBigInt)
{
   long lLength;

   assert(psFile != NULL);
   assert(oBigInt != NULL);

   lLength = oBigInt->lLength;
   if (lLength == 0)
      fprintf(psFile, "%016lx", 0UL);
   else if (lLength == 1)
      fprintf(psFile, "%016lx", oBigInt->aulDigits[0]);
   else if (lLength == 2)
   {
      fprintf(psFile, "%016lx", oBigInt->aulDigits[1]);
      fprintf(psFile, "%016lx", oBigInt->aulDigits[0]);
   }
   else
   {
      fprintf(psFile, "%016lx", oBigInt->aulDigits[lLength-1]);
      fprintf(psFile, "...");
      fprintf(psFile, "%016lx", oBigInt->aulDigits[0]);
   }
}
