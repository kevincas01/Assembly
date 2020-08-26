/*--------------------------------------------------------------------*/
/* simple.c                                                           */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include "bigint.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/resource.h>

/*--------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------*/

/* Write the sum of argv[1] and argv[2] to stdout. argv[1] and argv[2] 
   should be expressed in hexadecimal. The number written to stdout
   also is expressed in hexadecimal. As always, argc is the argument
   count.  Return 0 iff successful. */

int main(int argc, char *argv[])
{
   int iSuccessful;
   BigInt_T oAddend1;
   BigInt_T oAddend2;
   BigInt_T oSum;
   
   if (argc != 3)
   {
      fprintf(stderr, "Usage: %s hexint hexint\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   setCpuTimeLimit();

   oAddend1 = BigInt_new(0);
   oAddend2 = BigInt_new(0);
   oSum = BigInt_new(0);
   
   iSuccessful = BigInt_assignFromHexStr(oAddend1, argv[1]);
   if (! iSuccessful)
   {
      fprintf(stderr, "%s: Failure during creation of addend1\n",
         argv[0]);
      fprintf(stderr, "The first command-line argument should not\n");
      fprintf(stderr, "begin with 0x or 0X, and should consist of\n");
      fprintf(stderr, "hexadecimal digits only.\n");
      exit(EXIT_FAILURE);
   }      
   
   iSuccessful = BigInt_assignFromHexStr(oAddend2, argv[2]);
   if (! iSuccessful)
   {
      fprintf(stderr, "%s: Failure during creation of addend2\n",
         argv[0]);
      fprintf(stderr, "The second command-line argument should not\n");
      fprintf(stderr, "begin with 0x or 0X, and should consist of\n");
      fprintf(stderr, "hexadecimal digits only.\n");
      exit(EXIT_FAILURE);
   }
   
   iSuccessful = BigInt_add(oAddend1, oAddend2, oSum);
   if (! iSuccessful)
   {
      fprintf(stderr, "%s: Overflow during addition\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   
   BigInt_writeHex(stdout, oSum);
   putchar('\n');

   BigInt_free(oSum);
   BigInt_free(oAddend2);
   BigInt_free(oAddend1);

   return 0;
}
