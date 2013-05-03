/*
 * Cr�ation de deux threads incr�mentant une variable prot�g�e par un mutex
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define NTHREADS 10

int counter = 4;

sem_t semaphore;

/*
 * Fonction ex�cut�e par les threads
 */
void *inc_counter(void *arg)
{
	(void) arg;

	/* Acquisition du semaphore */
	sem_wait(&semaphore);

	counter++;
	fprintf(stderr, "Thread %ld : Counter value: %d\n", pthread_self(), counter);

	/* Lib�ration du semaphore */
	sem_post(&semaphore);

	return NULL;
}


int main(void)
{
	pthread_t thread_id[NTHREADS];
	int ii;

	/* Initialisation du semaphore */
	sem_init(&semaphore, 0, 1);

	/* Cr�ation des threads */
	for (ii = 0; ii < NTHREADS; ii ++) {
		if (pthread_create(&thread_id[ii], NULL, &inc_counter, NULL) != 0) {
			perror("pthread_create");
			exit(EXIT_FAILURE);
		}
	}

	/* Attente de la terminaison des threads */
	for (ii = 0; ii < NTHREADS; ii ++) {
		if (pthread_join(thread_id[ii], NULL) != 0) {
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}

	/* Destruction du semaphore */
	sem_destroy(&semaphore);

	printf("Final counter value %d\n", counter);

	exit(EXIT_SUCCESS);
}
