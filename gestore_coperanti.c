#include "util.h"

main() 
{ 
   int msqid;
   FILE *fp;
    char upload_val[20];
    int time_upload;
    int time_to_live;
    int pari_dispari_pid_download;
    int quantita_download;
   
   printf("\ninizio gestore cooperandi\n");

   if ((fp = fopen("inputcoperanti.txt", "r")) == NULL)
   {
      printf(" Gestore Cooperanti:E´ stato impossibile aprire il file; file inesistente\n");
      fflush(stdout);
      _exit(-1);
   }
   printf("\n Gestore Cooperanti: Apro file inputcoperanti.txt.\n");

   num_figli = 0;
   fscanf(fp, "%d %s %d %d %d" , &time_upload, &upload_val, &time_to_live, &pari_dispari_pid_download, &quantita_download);

   while(!feof(fp))
   { 
     
     if ((execl("cooperante", time_upload, upload_val, time_to_live,  pari_dispari_pid_download, quantita_download)) < 0)
	 {
            printf("execl non riuscita\n");
            fflush(stdout);
            _exit(-1);
     }
      else   
       	fscanf(fp, "%d %s %d %d %d", &time_upload, &upload_val, &time_to_live, &pari_dispari_pid_download, &quantita_download);
   }

   fclose(fp);
    printf("\n Gestore Cooperanti: Ho creato tutti i figli e poso morire in pace.\n");
}

