
#include "util.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <signal.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

	// Alloco la memoria condivisa
   int shmid;
   mem *log;
   int timer_pulizie_sek=5;
   int repmid;
   repository *rep;
   int msqid;
   int pid_pulizie;
   int pid_uploader;
   int semid;
   int val;

   int nr_coopranti = 0;
   int max_coopranti = 100;
   int coda_cooperanti[100];

   file_elem upload_queue[100];//fino a 100 msg di uploa din coda
   int queue_nr=0;



int salvalog(int shmid, mem *log){



    log = (mem*) shmat(shmid, NULL, 0);
    int i = 0;
    FILE *fp = fopen("log", "w");
    if(fp != NULL){
       while(i < log->cont){
         fprintf(fp,"numero pid cooperante: %d \t messaggio: %s;\n", log->array[i].pid_from, log->array[i].messaggio);
         i++;
      }
      fclose(fp);
      shmdt((void*)log);
      return 1;
   }
   else return 0;
}

void termina(){//int repmid, int shmid, int msqid, int semid, mem *log, int pid_pulizie






   salvalog(shmid, log);

   kill(pid_pulizie, SIGKILL);
   kill(pid_uploader, SIGKILL);



   int val;
   val = semctl(semid, 0, GETVAL, 0);
   printf("SEM 0 %d\n", val);
   fflush(stdout);
   val = semctl(semid, 1, GETVAL, 0);
   printf("SEM 1 %d\n", val);
   fflush(stdout);


   // disalloco la memoria condivisa log
   if (shmctl(shmid, IPC_RMID, 0) < 0){
	   printf("\n impossibile rimuovere la memoria condivisa log\n");
	   fflush(stdout);
	   _exit(-1);
	}

	// disalloco la memoria condivisa repository
    if (shmctl(repmid, IPC_RMID, 0) < 0){
	   printf("\n impossibile rimuovere la memoria condivisa repository\n");
	   fflush(stdout);
	   _exit(-1);
	}

	// disalloco la coda di messaggi
	if (msgctl(msqid, IPC_RMID, (struct msqid_ds *)0) < 0){
	   printf("\n impossibile rimuovere la coda di messaggi\n");
	   fflush(stdout);
	   _exit(-1);
	}

	// disalloco il vettore di semafori
	if (semctl(semid, 11, IPC_RMID, 0) < 0) {
	   printf("\n impossibile rimuovere il vettore di semafori\n");
	   fflush(stdout);
	   _exit(-1);
	}
}


// Define the function to be called when ctrl-c (SIGINT) signal is sent to process
void signal_callback_handler(int signum){
   printf("Caught signal %d\n",signum);

    //send kill yourself msg to evryone

      //quando ne elimino uno, meto valore -1 - todo si puo migliorare rispostando tutto l'arrey
        int i;
        for(i=0;i<max_coopranti; i++){
            if(coda_cooperanti[nr_coopranti] >0){//lo ho trovato

                coda_cooperanti[nr_coopranti]=-1;

                //send mesage to kill himself

                messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
                da_spedire->tipo_msg =  TIPO_KILL_YOURSELF;
                da_spedire->pid_from =  coda_cooperanti[nr_coopranti];
                sprintf(da_spedire->azione, "Uciditi %d", coda_cooperanti[nr_coopranti]);
                fflush(stdout);
                invia(msqid, da_spedire);


            }
        }


   // Cleanup and close up stuff here
	termina();

  //termina(repmid, shmid, msqid, semid, log, pid_pulizie);

   // Terminate program
   exit(signum);
}





int main(int argc, char *argv[]) {
	printf("\n Gestore Repository: ciao merde, io inizio a funzionare \n");

	// Register signal and signal handler
    signal(SIGINT, signal_callback_handler);





   if ((shmid = shmget(KEYMEM, sizeof(mem), IPC_CREAT | 0666)) < 0){
		printf("\n impossibile avere memoria condivisa per i log\n");
		fflush(stdout);
		_exit(-1);
	}
   log = (mem*) shmat(shmid, NULL, 0);
   log->cont = 0;
   shmdt((void*)log);

	//Alloco la memoria della repository


   if ((repmid = shmget(KEYMEM, sizeof(repository), IPC_CREAT | 0666)) < 0){
		printf("\n impossibile avere memoria condivisa per il repository\n");
		fflush(stdout);
		_exit(-1);
   }
   rep = (mem*) shmat(repmid, NULL, 0);
   rep->cont = 0;
   shmdt((void*)rep);

   //Alloco la coda di messaggi


   if ((msqid = msgget(KEY, IPC_CREAT | 0666)) < 0){
		printf("\n impossibile avere coda\n");
		fflush(stdout);
		_exit(-1);
   }

   printf("\n Cooperante: msqid = %d\n", msqid);

  //Alloco un vettore di semafori

  if ((semid = semget(KEYSEM, 11, IPC_CREAT | 0666)) < 0) {
		printf("\n Gestore Repository: impossibile avere il semaforo\n");
		fflush(stdout);
		_exit(-1);
   }

   semctl(semid, 0, SETVAL, 1); //pulizie
   semctl(semid, 1, SETVAL, 4); //upload


   // creo un fork, che esegue un po di codice per fare le pulizie ogni tot



   pid_pulizie = fork();
   if (pid_pulizie == 0){
        //execl("gestore_arrivi","gestore_arrivi", NULL);

		//flej 10 sec, e pastaj bej pastrimet
		int forever=0;
		while (forever<1) {

            int start_t,end_t;
            start_t=(int)time(NULL);
            //printf("\n start_t = %d \n", start_t );

			sleep(timer_pulizie_sek);


			// do the real cleaning in here

            int i;
            for(i=0; i< 20; i++){ // all of them

                file_elem my_elem  = rep->array[i];
                my_elem.tempo_di_vita = my_elem.tempo_di_vita - timer_pulizie_sek;

                //fake delete here...
                if(my_elem.tempo_di_vita < 1){

                    rep->array[i].dato="";
                    rep->array[i].padre_id=0;
                    rep->array[i].tempo_di_vita=-1;

                    rep->cont = i;
                }

            }

            //TODO - delete invalid data




            end_t=(int)time(NULL);
            printf("\n end_t = %d \n", (int)end_t);
            int exec_time = end_t-start_t;

            //printf("\n esecuzione in %d secondi \n", exec_time);
            printf("\n Desperatehousewife: FAI LE PULIZIE \n");
		}
		//ketu kam kodin e timerit qe do pastroje here pas here..

   }


   pid_uploader = fork();
   if (pid_uploader == 0){

         printf("\n UPLOADER: FACCIO UPLOAD SE NE CE BISOGNO \n");

		//flej 10 sec, e pastaj bej pastrimet
		int forever=0;
		while (forever<1) {
			sleep(4);//dormi 5 sec, e poi riprova a fare tutti gli upload

            int i;
            for(i=0;i<queue_nr ; i++){

             file_elem new_elem  = upload_queue[i];

             if(new_elem.padre_id>0){//elem valido..

               // if(semafotro ){
                    //add to queue
                    rep = (repository*) shmat(shmid, NULL, 0);
                    rep->array[rep->cont] = new_elem;


                    rep->cont++;

                    if(rep->cont==20){
                        rep->cont=0;
                    }

                    shmdt((void*)rep);
                    //emel becomes blak // todo - remove this.
                    new_elem.dato = "";
                    new_elem.padre_id = 0;
                    new_elem.tempo_di_vita = 0;

                    upload_queue[i] = new_elem;
                    //mando msg upload avenuto

                    messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
                    da_spedire->tipo_msg =  TIPO_OK_UPLOAD;
                    da_spedire->pid_from =  new_elem.padre_id;
                    sprintf(da_spedire->azione, "Uploaded ok %s ", new_elem.dato);
                    fflush(stdout);
                    invia(msqid, da_spedire);
                /*
                }else{

                    messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
                    da_spedire->tipo_msg =  TIPO_ERROR_UPLOAD;
                    da_spedire->pid_from =  new_elem->pid_from;
                    sprintf(da_spedire->azione, "Uploaded error %s ", new_elem->dato);
                    fflush(stdout);
                    invia(msqid, da_spedire);


                }
                */

             }
            }


            //printf("\n esecuzione in %d secondi \n", exec_time);
            printf("\n UPLOADER: FINE CICLO UPLOADZ\n");
		}
		//ketu kam kodin e timerit qe do pastroje here pas here..

   }



   //pres mesazhet in arrivo
   messaggio *da_ricevere = (messaggio*)malloc(sizeof(messaggio));
   da_ricevere->tipo_msg = TIPO_REGISTER;


   while(ricevi(msqid, da_ricevere)){
      sleep(1);
      printf("GESTIONE REPOSITORY: Waiting for msg \n");

     if(da_ricevere->tipo_msg == TIPO_REGISTER){

		printf("GESTIONE REPOSITORY: register new cooperante %s \n", da_ricevere->azione);

		if(nr_coopranti<max_coopranti){

			coda_cooperanti[nr_coopranti] = da_ricevere->pid_from;
			nr_coopranti++;

			log = (mem*) shmat(shmid, NULL, 0);
			log->array[log->cont].pid_from = getpid();
			strcpy(log->array[log->cont].messaggio, "Register some cooperator!");
			log->cont++;
			shmdt((void*)log);

		}else{
			printf("GESTIONE REPOSITORY: non oosso gestore piu di %d cooperatori  \n", max_coopranti);
		}

	 }else if(da_ricevere->tipo_msg == TIPO_UNREGISTER){
        printf("GESTIONE REPOSITORY: unregister new cooperante %s \n", da_ricevere->azione);
        //quando ne elimino uno, meto valore -1 - todo si puo migliorare rispostando tutto l'arrey

        int i;
        for(i=0;i<max_coopranti; i++){
            if(coda_cooperanti[nr_coopranti] == da_ricevere->pid_from){//lo ho trovato
                coda_cooperanti[nr_coopranti]=-1;

                //send mesage to kill himself

                messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
                da_spedire->tipo_msg =  TIPO_KILL_YOURSELF;
                da_spedire->pid_from =  da_ricevere->pid_from;
                sprintf(da_spedire->azione, "Uciditi %d", da_ricevere->pid_from);
                fflush(stdout);
                invia(msqid, da_spedire);


            }
        }

	 }else if(da_ricevere->tipo_msg == TIPO_DOWNLOAD){
        printf("GESTIONE REPOSITORY: request a new download %s \n", da_ricevere->pid_from);

        //respond with data to tipo_get_download
        //todo

        //if(semaforo can download){

            int quantita_download = (int)da_ricevere->azione;

            messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));

                char res[3000];
                int i;
                //rep = (mem*) shmat(shmid, NULL, 0);
                //ho meno elementi di quelli richiesti
                if(quantita_download > rep->cont){
                  for(i=0; i<rep->cont; i++){
                    strcpy(res,rep->array[i].dato);
                  }
                }else{

                  for(i=rep->cont-1 ; i > rep->cont - quantita_download -1; i--){
                    strcpy(res, rep->array[i].dato);
                  }

                }



            da_spedire->tipo_msg =  TIPO_GET_DOWNLOAD;
            da_spedire->pid_from =  da_ricevere->pid_from;
            sprintf(da_spedire->azione, "| %s", res);
            fflush(stdout);
            invia(msqid, da_spedire);


        /*
        }else{
            //cant download right now, so send error msg, user will retry later..


                messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
                da_spedire->tipo_msg =  TIPO_ERROR_DOWNLOAD;
                da_spedire->pid_from =  da_ricevere->pid_from;
                sprintf(da_spedire->azione, "Cant download this time, try later %d", da_ricevere->pid_from);
                fflush(stdout);
                invia(msqid, da_spedire);

        }
        */



     }else if(da_ricevere->tipo_msg == TIPO_UPLOAD){

        printf("GESTIONE REPOSITORY: new upload request from %s \n", da_ricevere->pid_from);

        //add to upload list

        file_elem new_elem;
        //struct file_elem *new_elem= malloc(sizeof(*new_elem));

        //file_elem new_elem = malloc(sizeof(file_elem));

        new_elem.padre_id=da_ricevere->pid_from;
        new_elem.tempo_di_vita =da_ricevere->extra;

        sprintf(new_elem.dato , "%s",da_ricevere->azione);

        upload_queue[queue_nr] = new_elem;
        fflush(stdout);


        queue_nr++;


	 }

   }

   termina();
   return 0;
}



