#include "util.h"

main(int argc, char **argv)
{  
   
   int msqid, semid, shmid;
   
    char upload_val[20];
    int time_upload;
    int time_to_live;
    int pari_dispari_pid_download;
    int quantita_download;
   
   time_upload = atoi(argv[1]);
   upload_val = atoi(argv[1]);
   time_to_live = atoi(argv[2]);
   pari_dispari_pid_download = atoi(argv[3]);
   quantita_download = atoi(argv[3]);
   
   
   printf("\n Cooperante: time_upload %d \n", time_upload);
   printf("\n Cooperante: upload_val %s \n", upload_val);
   printf("\n Cooperante: time_to_live %d \n", time_to_live);
   printf("\n Cooperante: pari_dispari_pid_download %d \n", pari_dispari_pid_download);
   printf("\n Cooperante: quantita_download %d \n", quantita_download);

   /*
   msqid = msgget(KEY, 0);
   if (msqid  < 0) 
   {
      printf("\n GESTORE AEREO: Non e´ stato possibile attaccarsi alla coda\n");
      fflush(stdout);
      _exit(-1);
   }
   printf("\n GESTORE AEREO: Mi sono attaccato alla coda.\n");
   
   semid = semget(KEYSEM,10, 0);
   if (semid  < 0) 
   {
      printf("\n GESTORE AEREO: Non e stato possibile attaccarsi al pool di semafori\n");
      fflush(stdout);
      _exit(-1);
   }
   printf("\nGESTORE AEREO: Mi sono attaccato al pool di semafori.\n");
      
   mem *log;
   if ((shmid = shmget(KEYMEM, sizeof(mem), 0)) < 0)
   {
      printf("\n GESTORE AEREO: Non e´ stato possibile attaccarsi alla memoria condivisa\n");
      fflush(stdout);
      _exit(-1);
   } 
   printf("\nGESTORE AEREO: Mi sono attaccato alla memoria condivisa");
      
   messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
   messaggio *da_ricevere = (messaggio*)malloc(sizeof(messaggio));
   da_ricevere->mesg_type = num_volo;
   da_spedire->mesg_type =  stato; 
   da_spedire->num_volo = num_volo;
   sprintf(da_spedire->azione, "%d", getpid());
   
   if (stato == 1) //se stato e' == 1 vuol dire che e'un mayday
   {
      printf("\n GESTORE AEREO : L'aereo nr. %d chiede il MAYDAY\n", num_volo);
      fflush(stdout);
      
      p(semid, MEM);
      log = (mem*) shmat(shmid, NULL, 0);
      log->array[log->cont].num_volo = num_volo;
      log->array[log->cont].pid_gestore = getpid();
      strcpy(log->array[log->cont].messaggio, "CHIEDE MAYDAY");
      log->cont++;
      shmdt((void*)log);
      v(semid, MEM);       
      invia(msqid, da_spedire);    
      ricevi(msqid, da_ricevere);  
      
      if ((strcmp(da_ricevere->azione, "LAND 5")!= 0))  
      { 
         printf("GESTORE AEREO: ERRORE land per MAYDAY\n");
         fflush(stdout);
         _exit(-1);
      }
      
      printf("\nGESTORE AEREO : L'aereo numero %d in MAYDAY atterra \n", num_volo);
      fflush(stdout);
      sleep(2);
     
      p(semid, MEM);
      log = (mem*) shmat(shmid, NULL, 0);
      log->array[log->cont].num_volo = num_volo;
      log->array[log->cont].pid_gestore = getpid();
      strcpy(log->array[log->cont].messaggio, "LANDED");
      log->cont++;
      shmdt((void*)log);
      v(semid, MEM);
     			
      strcpy(da_spedire->azione, "LANDED"); 
      da_spedire->mesg_type = num_volo-1;	
      
      invia(msqid, da_spedire);      
      _exit(0);
   }
   
   da_spedire->mesg_type =  stato; 
   printf("\nGESTORE AEREO : L'aereo numero %d chiede di atterrare\n", num_volo);
   fflush(stdout);
   
   p(semid, MEM);
   log = (mem*) shmat(shmid, NULL, 0);
   log->array[log->cont].num_volo = num_volo;
   log->array[log->cont].pid_gestore = getpid();
   strcpy(log->array[log->cont].messaggio, "CHIEDE DI ATTERRARE");
   log->cont++;
   shmdt((void*)log);
   v(semid, MEM);
   
   invia(msqid, da_spedire);
   ricevi(msqid, da_ricevere);   
   if ((strcmp(da_ricevere->azione, "LAND")!= 0))  
   { 
       printf("\nGESTORE AEREO : ERRORE LAND\n");
       fflush(stdout);
      _exit(-1);
   } 
   printf("\nGESTORE AEREO: L'aereo numero %d ha ottenuto LAND: pista e gate\n", num_volo);
   fflush(stdout);
   
   ricevi(msqid, da_ricevere);  
   if ((strcmp(da_ricevere->azione, "TOGATE")!= 0))
   { 
       printf("\nGESTORE AEREO : ERRORE TOGATE\n");
       fflush(stdout);
      _exit(-1);
   }
   printf("\nGESTORE AEREO : L'aereo numero %d ha ottenuto TOGATE: corridoio\n", num_volo);
   fflush(stdout);
   
   ricevi(msqid, da_ricevere);  
   if ((strcmp(da_ricevere->azione, "GOGATE")!= 0))
   { 
       printf("\n GESTORE AEREO : ERRORE GOGATE \n");
       fflush(stdout);
       _exit(-1);
   }
   printf("\nGESTORE AEREO : L'aereo numero %d ha ottenuto GOGATE: primo della coda \n", num_volo);
   fflush(stdout);
 
   da_spedire->mesg_type = getpid();
   invia(msqid, da_spedire);
         
   if (tempo == 0) // se tmp == 0. L´aereo non parte più.
   {
      printf("\nGESTORE AEREO : L'aereo numero %d  non deve piu' partire quindi entra nell' angar\n", da_ricevere->mesg_type);
      fflush(stdout);
      v(semid, GATE);
      _exit(0);
   }
     
   else
   { 
      //sleep per il tempo che deve stare fermo prima di partire
      sleep(tempo);
         
      da_ricevere->mesg_type = num_volo-1;  
      da_spedire->mesg_type = PARTENZA;  
      sprintf(da_spedire->azione, "%d", getpid());
      printf("\nGESTORE AEREO : L'aereo numero %d chiede di partire\n", num_volo);
      fflush(stdout);
      invia(msqid, da_spedire); 
    
      p(semid, MEM);
      log = (mem*) shmat(shmid, NULL, 0);
      log->array[log->cont].num_volo = num_volo;
      log->array[log->cont].pid_gestore = getpid();
      strcpy(log->array[log->cont].messaggio, "CHIEDE DI PARTIRE");
      log->cont++;
      shmdt((void*)log);
      v(semid, MEM);
   
      ricevi(msqid, da_ricevere);  
      if ((strcmp(da_ricevere->azione, "LEAVEGATE")!= 0))
      { 
         printf("\NGESTORE AEREO : ERRORE LEAVEGATE\n");
         fflush(stdout);
         _exit(-1);
      }  	                    
      printf("\nGESTORE AEREO : L'aereo numero %d ha ottenuto il corridoio in uscita : LEAVEGATE\n", num_volo);
      fflush(stdout);
      
      ricevi(msqid, da_ricevere);    
      if ((strcmp(da_ricevere->azione, "TORA")!= 0)) 
      { 
         printf("GESTORE AEREO : ERRORE TORA\n");
         fflush(stdout);
         _exit(-1);
      }  	                   
      printf("\nGESTORE AEREO : L'aereo numero %d ha ottenuto la pista : TORA\n", num_volo);
      fflush(stdout);        
      
      da_spedire->mesg_type = getpid();
      invia(msqid, da_spedire);   
      _exit(0);
  
   
   */	
    } 
}
