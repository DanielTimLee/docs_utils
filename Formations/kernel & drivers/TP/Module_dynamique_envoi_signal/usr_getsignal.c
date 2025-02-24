/**********************************
  Program :- To handle alarm signal.
 ***********************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#define DELAY_IN_SECOND 5

bool g_exitloop = false;
int  g_signal   = 0;

/**
 * Function used to wrap signal
 */
static void my_signal_handler(int no)
{
     printf("\n---> Receiving signal no.%d - '%s' is received \n", no, strsignal(no));
     sleep(1);
     
     switch (no)
     {
       case SIGINT:
       case SIGQUIT:
       case SIGTERM:
       case SIGTSTP:
       case SIGSTOP:
	 sleep(2);
	 g_exitloop= true;
	 break;
	 
       case SIGALRM:
	 break;
	 
       default:
	 break;
    }
} /* signal_handler */

/**
 * @brief Main entry of the process
 */
int main(int argc, char* argv[], char* env[])
{
     int rc_main    = 0;
     
     /*
      * checking the parameters
      */
     if ( argc > 1)
     {
	/*
	 * Getting the signal number to catch
	 */
	 g_signal  = atoi(argv[1]);
     
	 printf("\n--> Starting userspace program [%s] \n", argv[0] );
     
        /*
         * Registering the signal handler with the SIGALRM signal
         */
         printf("\nRegistering handle to the %d - '%s' signal \n", g_signal, strsignal(g_signal));
         signal(g_signal, my_signal_handler);
     
        /*
         * Displaying the current processus ID
         */
         printf("\nProcess ID : %d \n", getpid());
     
        /*
         * Testing the handler
         */
         printf("\nA signal will be send for test in %d seconds...\n", DELAY_IN_SECOND);
         alarm(DELAY_IN_SECOND);

        /*
         * Infinite loop...
         */
	 printf("\nEntering the infinite loop...");
         while(false == g_exitloop);

     } else {
        /*
         * Infinite loop...
         */
	printf("\nNot enough parameters !!\n");
	printf("\nUsage %s <signal number> \n", argv[0]);
     } /* IF */
	 
     printf("\n<-- Ending userspace program [%s] \n", argv[0] );
     
     return(rc_main);
} /* MAIN */
