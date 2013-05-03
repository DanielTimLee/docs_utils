/*
 * Lit un segment de m�moire partag�e prot�g� par un s�maphore.
 *   La cl� du s�maphore est donn� en argument du programme.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGSIZE 0xFF

int main(int argc, char **argv)
{
	key_t key;
	int sem;
	int shm;
	struct sembuf sembuf;
	char *message = NULL;

	if (argc != 2) {
		fprintf(stderr, "USAGE: %s <key file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/*
	 * A completer :
	 * Cr�ation de la cl� avec argv[1]
	 * R�cup�ration du s�maphore
	 * R�cup�ration du segment de m�moire
	 * Attachement au segment de m�moire
	 * Attente du s�maphore
	 * Affichage du message
	 * Lib�ration du s�maphore
	 * Destruction de segment de m�moire
	 * Destruction du s�maphore
	 *
	 */

	exit(EXIT_SUCCESS);
}

