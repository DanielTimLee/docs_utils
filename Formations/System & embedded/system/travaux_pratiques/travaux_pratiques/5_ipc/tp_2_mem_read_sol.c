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

	/* Cr�ation de la cl� */
	if ((key = ftok(argv[1], 0)) == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	/* R�cup�ration du s�maphore */
	if ((sem = semget(key, 1, IPC_CREAT | 0600)) == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}

	/* R�cup�ration du segment de m�moire */
	if ((shm = shmget(key, MSGSIZE, IPC_CREAT | 0600)) == -1) {
		perror("shmget");
		exit(EXIT_FAILURE);
	}

	/* Attachement au segment de m�moire */
	if ((message = shmat(shm, NULL, SHM_RDONLY)) == (void *) -1) {
		perror("shmat");
		exit(EXIT_FAILURE);
	}

	/* Attente du s�maphore */
	sembuf.sem_num = 0;
	sembuf.sem_op = -1;
	sembuf.sem_flg = 0;
	if (semop(sem, & sembuf, 1) < 0) {
		perror("semop");
		exit(EXIT_FAILURE);
	}

	/* Affichage du message */
	fprintf(stdout, "%s\n", message);

	/* Lib�ration du s�maphore */
	sembuf.sem_op = 1;
	if (semop(sem, & sembuf, 1) < 0) {
		perror("semop");
		exit(EXIT_FAILURE);
	}

	/* Destruction de segment de m�moire */
	if (shmctl(shm, IPC_RMID, NULL)) {
		perror("shmctl");
		exit(EXIT_FAILURE);
	}

	/* Destruction du s�maphore */
	if (semctl(sem, IPC_RMID, 0)) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

