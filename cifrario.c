#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
/*ATTENZIONE: RIPULIRE DAI WARNING NEL CODICE E CANCELLARE QUESTO COMMENTO*/
/*ATTENZIONE: CONTINUARE DA "HERE:"*/
typedef char ch;
typedef short int si;
typedef enum State {Free, Crypting, Decrypting} status;

//Public variables:
si sceltaMenus = 0;
status actualStatus = Free;
ch stringaCifrata[129];    //129 di cui 128 per la stringa e 1 per il carattere \0
ch chiave[129];
int trash;     //Usata per pulizia del buffer
si sceltaNonValida = 0;    //Prevediamo ogni inserimento dell'utente e non faremo crashare il programma

//Prototypes:
void MenuPrincipale(int );
void Crypt();
void Decrypt();

int main() {
   system("clear");     //Pulizia del terminale per una visione ottimale
   printf("\t\t\t\t################\n\t\t\t\t# Cifrario RC4 #\n\t\t\t\t################\n\n");     //Messaggio d'avvio centrato
   MenuPrincipale(0);
   return 0;
}

void MenuPrincipale(int caller){
   if(caller == 0){        //Il programma è appena stato avviato
      printf("\033[01;33mMenu Principale:\033[0m\n");
   }else{                  //Il programma è già stato eseguito
      system("clear");
      printf("\033[01;33mMenu Principale\033[0m\n");
   }//Fine del messaggio di benvenuto

   /*Mostra le opzioni*/
   sceltaMenus = 1;     //Se l'utente inserisci lettere anzichè numeri, si selezionerà la criptazione di una stringa
   printf("\n[\033[1;34m0\033[0m]Esci dal programma.\n");
   printf("[\033[1;34m1\033[0m]Cripta una stringa.\n");
   printf("[\033[1;34m2\033[0m]Decripta una stringa.\n");

   /*Nel caso in cui il programma sia già stato eseguito*/
   if(actualStatus == 1)
      printf("\n\033[0;32mTrovata stringa criptata\033[0m; digitare \033[1;34m2\033[0m per decriptare.\n");

   printf("\nLa tua scelta: \033[1;34m");
   scanf("%hd", &sceltaMenus);      //Anche se l'utente inserisse un numero oltre il max dello shortint, questo verrebbe tagliato
   printf("\033[0m");      //Ripristina il colore di default

   /*Gestisci l'inserimento*/
   switch(sceltaMenus){
      case 0:     /*Uscita dal programma*/
      exit(0);
      break;

      case 1:     /*Cripta una stringa*/
      Crypt();
      break;

      case 2:     /*Decripta una stringa*/
      Decrypt();
      break;

      default:    /*Numero non valido*/
      printf("\033[1;31mInserisci una scelta valida!\033[0m\n");
      sleep(2);      //Lascia il tempo di leggere il messaggio
      if(caller != 0 && actualStatus != 0)
      MenuPrincipale(1);
      else
      main();        //Oppure, in alternativa, system("./RC4-Encrypter"); per riavviare il programma
      break;
   }//Fine del switch

}//Fine del MenuPrincipale

void Crypt(){
   //Private variables:
   ch stringaInserita[129];

   if(actualStatus == 0){        /*Free*/
      //Si procede normalmente, il programma è al primo ciclo di crittazione
   }else if(actualStatus == 1){        /*Crypting*/
      printf("\a\033[1;31mAttenzione\033[0m: precedentemente si è inserita una stringa\n");
      printf("da criptare; continuando i dati verranno sovrascritti!\n");
      printf("Scrivendo \033[1;36mbackToMain\033[0m come stringa, si tornerà al menu principale.\n");
   }else{         /*Decrypting*/
      printf("\aE' stata appena decriptata una stringa;\n");
      printf("Continuando, i precendenti dati verranno sovrascritti.\n");
   }//Fine dell'if

   /*Inserimento della stringa*/
   printf("\nInserisci la stringa da criptare: \033[1;36m");
   while((trash = getchar()) != '\n' && trash != EOF);       //Pulisce il buffer di lettura prima di leggere
   fgets(stringaInserita, 128, stdin);       //Salva fino a 128 caratteri della stringa nella variabile <stringaInserita>
   printf("\033[0m");
   /*Verifica presenza escape-code to mainMenu*/
   char escapeSequence[] = "backToMain";
   si matchCounter = 0;       //Conta le corrispondenze nel for di seguito
   if(strlen(stringaInserita)  < 10){ //Se ha più di 10 caratteri sicuramente non è l'scape code
      for(int i = 0; i < 10; i++){
         if(escapeSequence[i] == stringaInserita[i])
             matchCounter++;
         else{
             break;     //Risparmiamo le forze: se 1 valore soltanto varia, sono sicuramente diverse!
         }
      }
   }//Fine if per confronto escapeSequence
   if(matchCounter == 10){    //Se e solo se sono 10
      MenuPrincipale(1);
   }else{
      printf("La stringa è stata salvata come segue: \033[1;36m%s\033[0m\n", stringaInserita);
      /*Mostra le opzioni*/
      sceltaMenus = 2;        //Se l'utente inserisce lettere, in automatica si opta per la generazione di una chiave casuale
      printf("[\033[1;34m0\033[0m]Torna al menu.\n");
      printf("[\033[1;34m1\033[0m]Inserisci chiave.\n");
      printf("[\033[1;34m2\033[0m]Genera chiave.\n");
      printf("\nInserisci la tua scelta: \033[1;34m");
      //WARNING: TEST FOR CLEAN NEEDED
      scanf("%hd", &sceltaMenus);
      printf("\033[0m");      //Ripristina il colore di default

      /*Gestisci l'inserimento*/
      switch(sceltaMenus){
         case 0:        /*Torna al menu principale*/
         MenuPrincipale(1);
         break;

         case 1:        /*Inserisci una chiave manualmente*/
         sceltaNonValida = 0;       //Scelta qua sotto: inizializzazione della variabile di controllo
         do{
         printf("Digita di seguito la chiave (almeno %ld caratteri): \033[1;36m", strlen(stringaInserita) - 1);
         if(sceltaNonValida == 0)      //Necessario: se il ciclo venisse ripetuto, il buffer cleaning non deve essere ripetuto o impallerà il programma!
            while((trash = getchar()) != '\n' && trash != EOF);       //Pulisce il buffer di lettura prima di leggere
         else
            sceltaNonValida = 0; //Ripristina lo stato
         fgets(chiave, 128, stdin);       //Salva fino a 128 caratteri della stringa nella variabile <chiave>
         printf("\033[0m");      //Ripristina il colore di default
         strlen(chiave) < strlen(stringaInserita) ? printf("\a\n\033[1;31mLa chiave inserita è troppo corta\033[0m;\n"), sceltaNonValida = 1 : printf("La chiave è \033[1;36m%s\033[0m\n", chiave);
         }while(strlen(chiave) < strlen(stringaInserita));  //Fine del controllo della chiave
         printf("\a\033[1;31mAttenzione\033[0m: essendo stata inserita manualmente la chiave, la\nstringa cifrata potrebbe contenere molti caratteri insoliti.\n");
         sleep(2);
         break;

         default:       /*Scelta non valida*/
         printf("\a\033[1;31mScelta non valida\033[0m! Verrà generata \nuna chiave casuale.\n");
         sleep(2);      //Lascia il tempo di leggere il messaggio
         //Omettendo il break, si eseguirà il case 2, cioè la generazione casuale della chiave, evitando di ripetere i comandi!

         case 2:        /*Generazione chiave casuale*/
         printf("Generazione della chiava in maniera casuale...\n");
         time_t t;
         srand((unsigned) time(&t));   //Inizializzato il generatore random
         for(si i = 0; i < strlen(stringaInserita) - 1; i++){     //Con "-1" escludiamo il "\n"!
            chiave[i] = rand() % 96 + 32;    //Vengono esclusi i caratteri speciali per una stringa cifrata più visual-friendly
         } //Fine della generazione della chiave
         printf("La chiave è \033[1;36m%s\033[0m.\n", chiave);
         break;

      }//Fine del switch

      /*Processo di criptazione*/
      actualStatus = Crypting;      //Comunica al resto del programma che è iniziato un processo di criptazione
      printf("Avvio della criptazione...\n");
      for(si i = 0; i < strlen(stringaInserita); i++){      //WARNING: ESCLUDERE IL "\n" DALLA CIFRATURA?
         stringaCifrata[i] = stringaInserita[i] ^ chiave[i];
      }//Fine del for per la criptazione
      sleep(2);      //Pausa ad effetto
      printf("\n\a>>La stringa è stata criptata: \n\033[1;36m%s\033[0m\n", stringaCifrata);     //Si va a capo perchè se venisse generato il carattere backspace, la stringa criptata coprirebbe il testo.
      printf("Avvio del menu principale...\n");
      sleep(6);      //Diamo all'utente il tempo di leggere
      MenuPrincipale(1);

   }//Fine dell'if per controllo escapeSequence

}//Fine di Crypt

void Decrypt(){
   //Private variables:
   ch stringaDecifrata[129];
   ch confirm = 'Y'; //Variabile per confermare la decriptazione

   /*Verifica della possibilità di procedere*/
   printf("Controllo della presenza di stringhe decriptabili...\n");
   sleep(1);
   if(actualStatus == 0 || actualStatus == 2){     //Non sono state avviate procedure di decriptazione o sono state concluse
      printf("\a\033[1;31mE' necessario prima criptare una stringa per procedere\ncon la decriptazione\033[0m!\n");
      sleep(3);
      MenuPrincipale(1);
   }//Fine del controllo
    else{      //Else non del tutto necessario, ma inserito per maggiore correttezza
      /*Si procede quindi con la decriptazione*/
      printf("\033[0;32mTrovata stringa criptata\033[0m!\n");
      do{
         printf("Procedere?[S/N] ");
         while((trash = getchar()) != '\n' && trash != EOF);
         scanf("%c", &confirm);
         if(confirm == 'S' || confirm == 's'){
            printf("Avvio della decriptazione...\n");
            actualStatus = 2;    //Imposta lo stato di decriptazione
            for(si i = 0; i < strlen(stringaCifrata); i++){
               stringaDecifrata[i] = stringaCifrata[i] ^ chiave[i];
            }
            sleep(2);      //Solita pausa ad effetto
            printf("\n\a>>La stringa decriptata è \033[1;36m%s\033[0m\n", stringaDecifrata);
            printf("Avvio del menu principale...\n");
            sleep(6);
            MenuPrincipale(1);
         }else if(confirm == 'N' || confirm == 'n'){
            MenuPrincipale(1);
         }else
            printf("\a\033[1;31mScelta non valida\033[0m! ");
      }while(confirm != 'S' || confirm != 's' || confirm != 'N' || confirm != 'n');
   }//Fine dell'else

}//Fine di Decrypt
