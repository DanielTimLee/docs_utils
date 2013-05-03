/*
 * Cr�� et �crit un message dans un segment de m�moire partag�e prot�g� par
 * un s�maphore.
 *   La cl� du s�maphore est donn� en argument du programme.
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#define MSGSIZE 0xFF

typedef union {
	int val;						/* value for SETVAL */
	struct semid_ds *buffer;		/* buffer for IPC_STAT */
	unsigned short int *array;		/* buffer for GETALL and SETALL */
} semun_t;


int main(int argc, char **argv)
{
	key_t key;
	int sem;
	int shm;
	struct sembuf sem_buf;
	semun_t semun;
	char *message = NULL;
	unsigned short sem_array[1];

	if (argc != 2) {
		fprintf(stderr, "USAGE: %s <key file>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/*
	 * A completer :
	 * Cr�ation de la cl� avec argv[1]
	 * Cr�ation du segment de m�moire
	 * Attachement du pointeur message au segment de m�moire
	 *
	 */

	/* Cr�ation du s�maphore */
	if ((sem = semget(key, 1, IPC_CREAT | 0600)) == -1) {
		perror("semget");
		exit(EXIT_FAILURE);
	}
	sem_array[0] = 1;
	semun.array = sem_array;
	if (semctl(sem, 0, SETALL, semun) < 0) {
		perror("semctl");
		exit(EXIT_FAILURE);
	}

	/* Acquisition du s�maphore */
	sem_buf.sem_num = 0;
	sem_buf.sem_op  = -1;
	sem_buf.sem_flg = SEM_UNDO;
	if (semop(sem, &sem_buf, 1) < 0) {
		perror("semop");
		exit(EXIT_FAILURE);
	}

	/* Lit le message depuis stdin */
	printf("> ");
	if (fgets(message, MSGSIZE, stdin) == NULL) {
		fprintf(stderr, "fgets error\n");
		exit(EXIT_FAILURE);
	}

	/* Lib�ration du s�maphore */
	sem_buf.sem_op = 1;
	if (semop(sem, & sem_buf, 1) < 0) {
		perror("semop");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

