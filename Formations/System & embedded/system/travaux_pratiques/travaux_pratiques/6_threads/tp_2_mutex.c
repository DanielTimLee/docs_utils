/*
 * Cr�ation de deux threads incr�mentant une variable prot�g�e par un mutex
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define NTHREADS 10

int counter = 4;

/* Initialisation du mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Fonction ex�cut�e par les threads
 */
void *inc_counter(void *arg)
{
	(void) arg;

	/*
	 * A completer :
	 * Acquisition du mutex
	 * Incr�mentation de counter
	 *
	 */

	fprintf(stderr, "Thread %ld : Counter value: %d\n", pthread_self(), counter);

	/*
	 * A completer :
	 * Lib�ration du mutex
	 *
	 */

	return NULL;
}


int main(void)
{
	pthread_t thread_id[NTHREADS];
	int ii;

	/*
	 * A completer :
	 * Cr�er NTHREADS
	 * Attendre la terminaison des threads
	 *
	 */

	printf("Final counter value %d\n", counter);

	exit(EXIT_SUCCESS);
}
