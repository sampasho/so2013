#include "util.h"

main() { 
   //vlere nga e cila do kuptojme a jemi ne threadin krysore apo nje nga forket
   int sono_il_padre;
   //kjo na duhet ne fund, qe babai te prese tere femijet e pastaj te mbyllet
   int num_figli=0;
   //faili qe do lexojme
   FILE *fp;
   //5 parametrat per rresht qe na duhen
   char upload_val[20];
   char time_upload[20];
   char time_to_live[20];
   char pari_dispari_pid_download[20];
   char quantita_download[20];
   
   printf("\n inizio gestore cooperandi \n");
   //hapim failin, nese ka gabim ose nuk eksiston, atehere gabim, dhe dal
   if ((fp = fopen("inputcoperanti.txt", "r")) == NULL){
      printf(" Gestore Cooperanti:E´ stato impossibile aprire il file; file inesistente\n");
      fflush(stdout);
      _exit(-1);
   }
   
   printf("\n Gestore Cooperanti: Apro file inputcoperanti.txt.\n");
   //lexoj rreshtin e pare
   fscanf(fp, "%s %s %s %s %s" , time_upload, upload_val, time_to_live, pari_dispari_pid_download, quantita_download);

   while(!feof(fp)){ 
	    //cdo rrresht nje femije
	    num_figli++;
		//bejme nje fork te procesit ku jemi, dmth krijojme nje proces kopje te ketij qe kemi, qe vazhdon eksekutimin nga ketu ku jam tani, e te njetn gje ben dhe procesi babe
		//femiu do leshoje binari e vet, ku do ete tere llogjiken e tij, kurse babai do lexoje nje rresh te ri, per forun e ardhshem ne ciklin while
		sono_il_padre = fork();
		if (sono_il_padre == 0){ 
			printf("\n eksekutome cooperante: %s %s %s %s %s.\n" , time_upload, upload_val, time_to_live,  pari_dispari_pid_download, quantita_download);
			//metoda execl ben run te nje binari, me parametra, ne thredi ku je, parametri i pare eshte path  i binarit qe do hapesh, i dyti eshte emri i failit, te tjerat jane parametrat, qe duhet te jene *char
			//e ne fund nje NULL, mos e hiqni nullin, pasi do tu jape gabim (te pakten ne ubuntu 13.04)
			int execCode =(execl("./cooperante", "cooperante", time_upload, upload_val, time_to_live,  pari_dispari_pid_download, quantita_download, NULL));
			printf("\n Gestore Cooperanti: execl code %i \n", execCode);
			if ( execCode < 0){
				printf("\n Gestore Cooperanti: execl non riuscita \n");
				fflush(stdout);
				_exit(-1);
			}
		}else{   
			fscanf(fp, "%s %s %s %s %s", time_upload, upload_val, time_to_live, pari_dispari_pid_download, quantita_download);
		}	
	}
    
	fclose(fp);
	//per cdo fimije, ben wait, dmth pret we secili te mbyllet: http://linux.die.net/man/2/wait
	while(num_figli > 0){
      wait(0);
      num_figli--;
    }
	// tutto finito, jep mesazhin perfundimtar
	printf("\n Gestore Cooperanti: Ho lanciato il fork e execl di tutti i figli, ora poso morire in pace.\n");
}
