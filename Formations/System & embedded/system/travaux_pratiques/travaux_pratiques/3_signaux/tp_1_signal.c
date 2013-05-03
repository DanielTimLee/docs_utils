/*
 * Capture le signal SIGINT et reset son action par d�faut
 * Le programme devra �tre de interrompu par un nouveau SIGINT.
 */

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handler(int sig)
{
	printf("Signal %d re�u\n", sig);

	/*
	 * A completer :
	 * Reseter l'action par d�faut du signal
	 * Utiliser la fonction signal
	 *
	 */
}

int main(void)
{
	/*
	 * A completer :
	 * Assigner la fonction handler() au signal SIGINT
	 * Utiliser la fonction signal
	 *
	 */

	/* Boucle ind�finiment */
	while (1) {
		printf("Hello world\n");
		sleep(1);
	}
}
