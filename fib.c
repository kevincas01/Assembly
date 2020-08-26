/*--------------------------------------------------------------------*/
/* fib.c                                                              */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include "bigint.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#ifndef S_SPLINT_S
#include <sys/resource.h>
#endif

/*--------------------------------------------------------------------*/

#ifndef S_SPLINT_S
/* Set the process's "CPU time" resource limit.  After the CPU
   time limit expires, the OS will send a SIGKILL signal to the
   process. */

static void setCpuTimeLimit(void)
{
   enum {CPU_TIME_LIMIT_IN_SECONDS = 300};
   struct rlimit sRlimit;
   sRlimit.rlim_cur = CPU_TIME_LIMIT_IN_SECONDS;
   sRlimit.rlim_max = CPU_TIME_LIMIT_IN_SECONDS;
   setrlimit(RLIMIT_CPU, &sRlimit);
}
#endif

/*--------------------------------------------------------------------*/

/* Create and return a new BigInt object initialized to ul.  Exit
   with status EXIT_FAILURE if insufficient memory is available. */

static BigInt_T createBigInt(unsigned long ul)
{
   BigInt_T oBigInt = BigInt_new(ul);
   if (oBigInt == NULL)
   {
      fprintf(stderr, "Memory allocation error\n");
      exit(EXIT_FAILURE);
   }
   return oBigInt;
}

/*--------------------------------------------------------------------*/

/* Compute and write fib(iNum) to stdout.  Write the CPU time
   consumed to stderr. */

static void computeFib(int iNum)
{
   int i;
   BigInt_T oFirstPrev;
   BigInt_T oSecondPrev;
   BigInt_T oCurrent;
   BigInt_T oTemp;

   if (iNum < 2)
   {
      BigInt_T oBigInt = createBigInt((unsigned long)iNum);
      printf("Fibonacci number %d:\n", iNum);
      BigInt_writeHex(stdout, oBigInt);
      putchar('\n');
      BigInt_free(oBigInt);
      return;
   }

   oSecondPrev = createBigInt(0);
   oFirstPrev = createBigInt(1);
   oTemp = createBigInt(0);

   oCurrent = NULL;

   for (i = 2; i <= iNum; i++)
   {
      oCurrent = oTemp;
      if (! BigInt_add(oFirstPrev, oSecondPrev, oCurrent))
      {
         fprintf(stderr, "Addition overflow\n");
         exit(EXIT_FAILURE);
      }
      oTemp = oSecondPrev;
      oSecondPrev = oFirstPrev;
      oFirstPrev = oCurrent;
   }

   printf("Fibonacci number %d:\n", iNum);
   BigInt_writeHex(stdout, oCurrent);
   putchar('\n');

   BigInt_free(oTemp);
   BigInt_free(oFirstPrev);
   BigInt_free(oSecondPrev);
}

/*--------------------------------------------------------------------*/

/* Boundary test the BigInt_add function. */

static void boundaryTest(void)
{
   {  /* 0 + 0 */

      BigInt_T oFirst = createBigInt(0);
      BigInt_T oSecond = createBigInt(0);
      BigInt_T oSum = createBigInt(0);

      printf("Boundary test 1: "); 
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* Clear of sum required. */

      BigInt_T oFirst = createBigInt(0);
      BigInt_T oSecond = createBigInt(0);
      BigInt_T oSum = createBigInt(0);
      BigInt_assignFromHexStr(oSum, "100000000");

      /* This addition should clear element 1 of oSum. */
      printf("Boundary test 2a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_assignFromHexStr(oFirst, "100000000");
      BigInt_assignFromHexStr(oSecond, "0");

      /* This addition will fail if element 1 of oSum wasn't cleared. */
      printf("Boundary test 2b: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSum, oSecond))
         BigInt_writeHex(stdout, oSecond);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* fffffffffffffffe + 0, 0 + fffffffffffffffe */

      BigInt_T oFirst = createBigInt(ULONG_MAX - 1);
      BigInt_T oSecond = createBigInt(0);
      BigInt_T oSum = createBigInt(0);

      printf("Boundary test 3a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 3b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* fffffffffffffffe + 1, 1 + fffffffffffffffe */

      BigInt_T oFirst = createBigInt(ULONG_MAX - 1);
      BigInt_T oSecond = createBigInt(1);
      BigInt_T oSum = createBigInt(0);

      printf("Boundary test 4a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 4b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* ffffffffffffffff + 0, 0 + ffffffffffffffff */

      BigInt_T oFirst = createBigInt(ULONG_MAX);
      BigInt_T oSecond = createBigInt(0);
      BigInt_T oSum = createBigInt(0);

      printf("Boundary test 5a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 5b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* ffffffffffffffff + 1, 1 + ffffffffffffffff */

      BigInt_T oFirst = createBigInt(ULONG_MAX);
      BigInt_T oSecond = createBigInt(1);
      BigInt_T oSum = createBigInt(0);

      printf("Boundary test 6a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 6b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* ffffffffffffffff0000000000000001 + ffffffffffffffff,
         ffffffffffffffff + ffffffffffffffff0000000000000001 */

      BigInt_T oFirst = createBigInt(0);
      BigInt_T oSecond = createBigInt(ULONG_MAX);
      BigInt_T oSum = createBigInt(0);
      BigInt_assignFromHexStr(oFirst,
         "ffffffffffffffff0000000000000001");

      printf("Boundary test 7a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 7b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHex(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* largest possible + 0, 0 + largest possible */

      BigInt_T oFirst = createBigInt(0);
      BigInt_T oSecond = createBigInt(0);
      BigInt_T oSum = createBigInt(0);
      BigInt_largest(oFirst);

      printf("Boundary test 8a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHexAbbrev(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 8b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHexAbbrev(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* largest possible + 1, 1 + largest possible */

      BigInt_T oFirst = createBigInt(0);
      BigInt_T oSecond = createBigInt(1);
      BigInt_T oSum = createBigInt(0);
      BigInt_largest(oFirst);

      printf("Boundary test 9a: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHexAbbrev(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      printf("Boundary test 9b: ");
      fflush(stdout);
      if (BigInt_add(oSecond, oFirst, oSum))
         BigInt_writeHexAbbrev(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }

   {  /* largest possible + largest possible */

      BigInt_T oFirst = createBigInt(0);
      BigInt_T oSecond = createBigInt(0);
      BigInt_T oSum = createBigInt(0);
      BigInt_largest(oFirst);
      BigInt_largest(oSecond);

      printf("Boundary test 10: ");
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHexAbbrev(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      BigInt_free(oSum);
      BigInt_free(oSecond);
      BigInt_free(oFirst);
   }
}

/*--------------------------------------------------------------------*/

/* Stress test the BigInt_add function. */

static void stressTest(void)
{
   /* The number of stress test additions to perform. */
   enum {STRESS_TEST_COUNT = 20};  /* Arbitrary */

   BigInt_T oFirst;
   BigInt_T oSecond;
   BigInt_T oSum;
   int i;

   /* Create three BigInt objects. */

   oFirst = createBigInt(0);
   oSecond = createBigInt(0);
   oSum = createBigInt(0);

   /* Perform stress tests. */

   for (i = 0; i < STRESS_TEST_COUNT; i++)
   {
      /* Assign random numbers to the three BigInt objects. */
      BigInt_random(oFirst);
      BigInt_random(oSecond);
      BigInt_random(oSum);

      /* Compute and write a sum. */
      printf("Stress test %da: ", i);
      fflush(stdout);
      if (BigInt_add(oFirst, oSecond, oSum))
         BigInt_writeHexAbbrev(stdout, oSum);
      else
         printf("Addition overflow");
      putchar('\n');

      /* Compute and write a sum using the previous sum. */
      BigInt_random(oFirst);
      BigInt_random(oSecond);
      printf("Stress test %db: ", i); 
      fflush(stdout);
      if (BigInt_add(oFirst, oSum, oSecond))
         BigInt_writeHexAbbrev(stdout, oSecond);
      else
         printf("Addition overflow");
      putchar('\n');
   }

   /* Free the BigInt objects. */

   BigInt_free(oSum);
   BigInt_free(oSecond);
   BigInt_free(oFirst);
}

/*--------------------------------------------------------------------*/

/* First write fibonacci number n to stdout, where n is provided as
   argv[1].  n is expressed in decimal notation.  The fibonacci number
   is expressed in hexadecimal notation.  Write the CPU time consumed
   to stderr.
   Then boundary test and stress test the BigInt_add() function by
   performing some computations and writing the results to stdout.
   As always, argc is the argument count.  Return 0 iff successful. */

int main(int argc, char *argv[])
{
   int iNum;
   clock_t iInitialClock;
   clock_t iFinalClock;

   /* Validate the command line argument, and assign it to iNum. */

   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s nonneginteger\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   if (sscanf(argv[1], "%d", &iNum) != 1)
   {
      fprintf(stderr, "Argument must be an integer\n");
      exit(EXIT_FAILURE);
   }
   if (iNum < 0)
   {
      fprintf(stderr, "Argument must be non-negative\n");
      exit(EXIT_FAILURE);
   }
   
#ifndef S_SPLINT_S
   setCpuTimeLimit();
#endif

   printf("------------------------------------------------\n");

   /* Compute and write fibonacci number iNum. */

   iInitialClock = clock();
   computeFib(iNum);
   iFinalClock = clock();
   fprintf(stderr, "CPU time:  %f seconds\n",
      ((double)(iFinalClock - iInitialClock)) / CLOCKS_PER_SEC);

   printf("------------------------------------------------\n");

   /* Perform boundary tests. */

   boundaryTest();

   printf("------------------------------------------------\n");
   
   /* Perform stress tests. */

   stressTest();

   printf("------------------------------------------------\n");

   return 0;
}
