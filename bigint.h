/*--------------------------------------------------------------------*/
/* bigint.h                                                           */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#ifndef BIGINT_INCLUDED
#define BIGINT_INCLUDED

#include <stdio.h>

/* A BigInt_T object is a high-precision integer. */

typedef struct BigInt *BigInt_T;

/*--------------------------------------------------------------------*/

/* Return a new BigInt_T object initialized to ulValue, or NULL
   if insufficient memory is available. */

BigInt_T BigInt_new(unsigned long ulValue);

/*--------------------------------------------------------------------*/

/* Free oBigInt. */

void BigInt_free(BigInt_T oBigInt);

/*--------------------------------------------------------------------*/

/* If pcHexStr is a string of hexadecimal digits, then assign the
   unsigned integer represented by that string to oBigInt and return
   1 (TRUE). Otherwise leave oBigInt unchanged and return 0 (FALSE). */

int BigInt_assignFromHexStr(BigInt_T oBigInt, const char *pcHexStr);

/*--------------------------------------------------------------------*/

/* Assign to oBigInt the largest possible BigInt_T object. */

void BigInt_largest(BigInt_T oBigInt);

/*--------------------------------------------------------------------*/

/* Randomly choose a number of digits.  Then assign to oBigInt a
   random integer containing that many digits. */

void BigInt_random(BigInt_T oBigInt);

/*--------------------------------------------------------------------*/

/* Assign the sum of oAddend1 and oAddend2 to oSum.  oSum should be
   distinct from oAddend1 and oAddend2.  Return 0 (FALSE) if an
   overflow occurred, and 1 (TRUE) otherwise. */

int BigInt_add(BigInt_T oAddend1, BigInt_T oAddend2, BigInt_T oSum);

/*--------------------------------------------------------------------*/

/* Write oBigInt to psFile in hexadecimal format. */

void BigInt_writeHex(FILE *psFile, BigInt_T oBigInt);

/*--------------------------------------------------------------------*/

/* Write an abbreviated version of oBigInt -- just its first and
   last digits -- to psFile in hexadecimal format. */

void BigInt_writeHexAbbrev(FILE *psFile, BigInt_T oBigInt);

#endif

