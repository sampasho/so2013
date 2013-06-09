#include "util.h"

int runnig=1;
int sono_il_padre;
int long_live=0;
int msqid;
int semid;
int shmid;
int upload_live;
int uploaded = 0;
char *upload_val;
int time_upload;
int time_to_live;
int pari_dispari_pid_download;
int quantita_download;

//ky duhet te fleje gjume
taskLavora(){
   printf("\n Cooperante: sto lavorando \n");
   sleep(4);
}
//ky duhet te eksekutohet nga repository si utua esclusione
taskUpload(){
   printf("\n Cooperante: sto uplodando \n");

   	messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
	da_spedire->tipo_msg =  TIPO_UPLOAD;
	da_spedire->pid_from =  getpid();

	da_spedire->extra =  upload_live;


    sprintf(da_spedire->azione, "Upload %s", upload_val);
    fflush(stdout);
    invia(msqid, da_spedire);
    printf("\n Cooperante: upload request \n");


}
//merr dicka nga repository
taskDownload(){
   printf("\n Cooperante: sto downloadando \n");

    messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
	da_spedire->tipo_msg =  TIPO_UPLOAD;
	da_spedire->pid_from =  getpid();
    sprintf(da_spedire->azione, "%d", quantita_download);
    fflush(stdout);
    invia(msqid, da_spedire);
    printf("\n Cooperante: download request \n");


}

//tregori repository qe ke lindur, e regjistrohu tek ai
doRegister(){
	printf("\n Cooperante: register \n");

	messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
	da_spedire->tipo_msg =  TIPO_REGISTER;
	da_spedire->pid_from =  getpid();
    sprintf(da_spedire->azione, "Registra %d", getpid());
    fflush(stdout);
    invia(msqid, da_spedire);
    printf("\n Cooperante: register msg inviato \n");

}

//tregori repository qe ke ke mbaruar pune e po veds, ne kete menyre te fshin
doUnregister(){
	printf("\n Cooperante: unregister \n");

	messaggio *da_spedire = (messaggio*)malloc(sizeof(messaggio));
	da_spedire->tipo_msg =  TIPO_UNREGISTER;
	da_spedire->pid_from =  getpid();
    sprintf(da_spedire->azione, "Un Registra %d", getpid());
    fflush(stdout);
    invia(msqid, da_spedire);
    printf("\n Cooperante: register msg inviato \n");
}




main(int argc, char **argv){




   time_upload = atoi(argv[1]);
   upload_val = argv[2];
   upload_live = atoi(argv[3]);
   time_to_live = atoi(argv[4]);
   pari_dispari_pid_download = atoi(argv[5]);
   quantita_download = atoi(argv[6]);

//int execCode =(execl("./cooperante", "cooperante", time_upload, upload_val, upload_live,  time_to_live,  pari_dispari_pid_download, quantita_download, NULL));



   printf("\n Cooperante: time_upload %d \n", time_upload);
   printf("\n Cooperante: upload_val %d \n", upload_val);
   printf("\n Cooperante: upload_live %d \n", upload_live);
   printf("\n Cooperante: time_to_live %d \n", time_to_live);
   printf("\n Cooperante: pari_dispari_pid_download %d \n", pari_dispari_pid_download);
   printf("\n Cooperante: quantita_download %d \n", quantita_download);




   msqid = msgget(KEY, 0);
   if (msqid  < 0){
		printf("\n Cooperante: impossibile attaccarsi alla coda\n");
		fflush(stdout);
		_exit(-1);
	}

	printf("\n Cooperante: msqid = %d\n", msqid);


	/*
	if ((msqid = msgget(KEY, IPC_CREAT | 0666)) < 0){
		printf("\n impossibile avere coda\n");
		fflush(stdout);
		_exit(-1);
   }
	*/
   doRegister();


    //krijoj nje fork, qe sherben si timer, per te:
    // 1. pare sa kam jetuar, e vdekur kur eshte nevoja, duke derguar msg repositoryt
    // 2. bere upload nese eshte momenti


    sono_il_padre = fork();
    if (sono_il_padre == 0){
        printf("\n cooperante: task timer , lived=%d , tolive=%d\n", long_live, time_to_live);


        while(long_live<time_to_live){

            int start_t,end_t;
            start_t=(int)time(NULL);



            if(long_live > time_upload && uploaded== 0){
                uploaded=1;
                //send upload msg
                taskUpload();
            }else{
                taskLavora();
            }

            end_t=(int)time(NULL);

            long_live = long_live + (end_t-start_t);

            printf("\n cooperante: long_live %d \n", long_live );

        }

        //time to die, send unregister msg

        doUnregister();

	}

   //prit mesazh vdekje nga repository, po ta dergoi ai, ti vdis!
	messaggio *da_ricevere = (messaggio*)malloc(sizeof(messaggio));
	da_ricevere->tipo_msg = TIPO_KILL_YOURSELF;

   while(ricevi(msqid, da_ricevere) && runnig>0){
      sleep(1);
      if(da_ricevere->tipo_msg == TIPO_KILL_YOURSELF && da_ricevere->pid_from == getpid()){
        printf("\n devo morire, deto dal repository \n");

        //kill fork timer..
        kill(sono_il_padre, SIGKILL);

        fflush(stdout);
        _exit(-1);
      }else if(da_ricevere->tipo_msg == TIPO_READ_FOR_DOWNLOAD ){
        printf("\n something was uploaded, lets se if i have to download it..  \n");

        int pid_father = da_ricevere->pid_from;
        //vediamo se l'uploader e' pari o dispari

        if(pid_father% 2 != 0){
            //dispari
            if(pari_dispari_pid_download==1){
                taskDownload();
            }
        }else{
            //pari
            if(pari_dispari_pid_download==0){
                taskDownload();
            }
        }

      }else if(da_ricevere->tipo_msg == TIPO_GET_DOWNLOAD && da_ricevere->pid_from == getpid()){
        //receive downloaded data..
        printf("\n --> cooperante: i just downloaded: %s  \n", da_ricevere->azione);
      }else if(da_ricevere->tipo_msg == TIPO_ERROR_UPLOAD && da_ricevere->pid_from == getpid()){
        //receive downloaded data..
        printf("\n --> cooperante:  error upload, will retry later \n");
      }else if(da_ricevere->tipo_msg == TIPO_OK_UPLOAD && da_ricevere->pid_from == getpid()){
        //receive downloaded data..
        printf("\n --> cooperante:  upload ok :) \n");
      }else if(da_ricevere->tipo_msg == TIPO_ERROR_DOWNLOAD && da_ricevere->pid_from == getpid()){
        //receive downloaded data..
        printf("\n --> cooperante:  errore downlaod, sleep 3 sec, and retry again.. :) \n");
        sleep(3);
        taskDownload();
      }


    }



}
