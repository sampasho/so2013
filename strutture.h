#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/msg.h>
#include <signal.h>

#include <time.h>

#define KEY 2945 //la coda  dei messaggi - KEY
#define KEYMEM 2542 //memoria -KEY
#define KEYSEM 4875 //semafori -KEY

#define MEM 8 //identifica semaforo memoria condivisa, pulizie
#define BLOCCA 9 //identifica semaforo per bloccare gli upload

#define TIPO_REGISTER 1
#define TIPO_UNREGISTER 2
#define TIPO_DOWNLOAD 3
#define TIPO_UPLOAD 4
#define TIPO_ERROR_UPLOAD 5
#define TIPO_OK_UPLOAD 6
#define TIPO_KILL_YOURSELF 7
#define TIPO_READ_FOR_DOWNLOAD 8
#define TIPO_GET_DOWNLOAD 9
#define TIPO_ERROR_DOWNLOAD 10


//tipo i te dhenave qe duhet te ruaj ne buffer cirlocalre
typedef struct {
	int padre_id;
	int tempo_di_vita;
	char *dato;
}file_elem;

//mem condivisa upload/download, fixed to 20 elems
typedef struct {
   file_elem array[20];
   int cont;
}repository;


//la struttura del messaggio
typedef struct {
	long tipo_msg;
	int pid_from;
	int extra;
	char azione[20];
}messaggio;

typedef struct {
   int pid_from;
   char messaggio[100];
}istruzioni;

//memoria condivisa del log
typedef struct {
   istruzioni array[1000];
   int cont;
}mem;

