#include <stdio.h>
//#include <sys/shm.h>
//#include <sys/sem.h>
#include <stdlib.h>
#include <sys/types.h>
//#include <sys/ipc.h>
#include <unistd.h>
//#include <sys/wait.h>
#include <string.h>
//#include <sys/msg.h>
#include <signal.h>

#define KEY 2945 //la coda  dei messaggi - KEY
#define KEYMEM 2542 //memoria -KEY
#define KEYSEM 4875 //semafori -KEY
#define PISTA 0 //il semaforo pista
#define GATE 1 //il semaforo gate
#define CEPRIMO 2 //identifica semaforo corridoio entrata 
#define CESECONDO 3 
#define CETERZO 4
#define CUPRIMO 5 //identifica semaforo corridoio uscita
#define CUSECONDO 6 
#define CUTERZO 7
#define MEM 8 //identifica semaforo memoria condivisa
#define BLOCCA 9 //identifica semaforo per bloccare il gestore
#define ALLERT 1 
#define ARRIVO 2
#define PARTENZA 3
#define OKARRIVI 4 //type messaggio inviato dal gestore arrivi alla torre per mayday
#define OKPARTENZE 5 //type messaggio inviato dal gestore partenze alla torre per mayday

//la struttura del messaggio   
typedef struct {
	long mesg_type;  
	int num_volo;
	char azione[20];
}messaggio;

typedef struct {
   int num_volo;
   int pid_gestore;
   char messaggio[100];
}istruzioni;

//memoria condivisa del log
typedef struct {
   istruzioni array[1000];
   int cont;
}mem;

