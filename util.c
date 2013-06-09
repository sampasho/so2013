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


