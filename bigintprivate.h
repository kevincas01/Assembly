/*--------------------------------------------------------------------*/
/* bigintprivate.h                                                    */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#ifndef BIGINTPRIVATE_INCLUDED
#define BIGINTPRIVATE_INCLUDED

/* The maximum number of digits that a BigInt object can contain. */
enum {MAX_DIGITS = 32768};   /* Arbitrary */

/*--------------------------------------------------------------------*/

/* A struct BigInt is a high precision integer. */

struct BigInt
{
   /* The number of used digits in the BigInt object. The integer 0
      has length 0. This field could be of type int, but then the
      compiler would place padding between this field and the next. */
   long lLength;
   
   /* The digits comprising the BigInt object. aulDigits[0] stores the
      least significant digit.  The unused digits are set to 0. */
   unsigned long aulDigits[MAX_DIGITS];
};

#endif

