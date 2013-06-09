#include "util.h"

int invia(int id, messaggio *mesgptr){
	if (msgsnd(id, (int *)&(mesgptr->tipo_msg), sizeof(int) + sizeof(char)*20 , 0) < 0)
		return 0;
	else return 1;
}

int ricevi(int id, messaggio *mesgptr){
	int n;
	n = msgrcv(id, (int *)&(mesgptr->tipo_msg), sizeof(int) + sizeof(char)*20 , mesgptr->tipo_msg, 0);
		fflush(stdout);
	if (n < 0)
	  return 0;
	else return 1;
}


/* down*/
int sam_down(int semid, int semnum)
{
   struct sembuf semaforo[1];
   semaforo[0].sem_op = -1;
   semaforo[0].sem_num = semnum;
   semaforo[0].sem_flg = 0;
   semop(semid,&semaforo[0], 1);
}

/*up*/
int sam_up(int semid, int semnum)
{
   struct sembuf semaforo[1];
   semaforo[0].sem_op = 1;
   semaforo[0].sem_num = semnum;
   semaforo[0].sem_flg = 0;
   semop(semid,&semaforo[0], 1);
}

