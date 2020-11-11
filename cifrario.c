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
ch stringaCifrata[129];    //Riserviamo un posto al carattere terminatore \0
ch chiave[129];
ch storico[385];     //Una piccola history dell'ultima stringa decifrata. Non più di una stringa però: ricordiamoci che stiamo scrivendo nella RAM e non nella memoria interna!
int trash;     //Usata per pulizia del buffer
si troppoLunga = 0;  //Da usare per il calcolo della lunghezza della stringa in caso di stringa di 128 caratteri
ch antiBug;

//Prototypes:
void MenuPrincipale(int );
void Crypt();
void Decrypt();
void LastString();

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
   printf("\n[\033[1;34m0\033[0m]Esci dal programma.\n");
   printf("[\033[1;34m1\033[0m]Cripta una stringa.\n");
   printf("[\033[1;34m2\033[0m]Decripta una stringa.\n");
   printf("[\033[1;34m3\033[0m]Storico delle stringhe.\n");

   /*Nel caso in cui il programma sia già stato eseguito*/
   if(actualStatus == 1)
      printf("\n\033[0;32mTrovata stringa criptata\033[0m; digitare \033[1;34m2\033[0m per decriptare.\n");

   printf("\nLa tua scelta[Invio = 1]: \033[1;34m");
   scanf("%c", &antiBug);
   printf("\033[0m");      //Ripristina il colore di default

   /*Interpretiamo l'inserimento*/
   if(antiBug == '\n'){
      sceltaMenus = 1;     //Se l'utente preme invio senza scrivere, viene interpretato come 1
   }
   else if(antiBug < 48 || antiBug > 57)   //I numeri in ascii vanno da 48 a 57, quindi questa condizione equivale a "Se l'utente non inserisce un numero"
   {
      printf("\a\033[1;31mData la scelta non valida, si procede \nautomaticamente con l'opzione 1\033[0m.\n");
      sleep(2);
      sceltaMenus = 1;
      while((trash = getchar()) != '\n' && trash != EOF); //Il buffer contiene ancora almeno un \n sicuramente: puliamo
   }else{
      sceltaMenus = (si)antiBug - 48;
      while((trash = getchar()) != '\n' && trash != EOF); //Il buffer contiene ancora almeno un \n sicuramente: puliamo
   }//Fine dell'if per controllo

   /*Gestisci l'inserimento*/
   switch(sceltaMenus){
      case 0:     /*Uscita dal programma*/
      printf("\a\033[1;31mAr\033[1;32mr\033[01;33mi\033[1;34mve\033[1;35mde\033[1;36mrc\033[0mi!\n");
      sleep(1);
      exit(0);
      break;

      case 1:     /*Cripta una stringa*/
      Crypt();
      break;

      case 2:     /*Decripta una stringa*/
      Decrypt();
      break;

      case 3:
      LastString();
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
   si coincidenzaChiave = 0; //Usata nell'inserimento manuale della chiave

   if(actualStatus == 0){        /*Free*/
      //Si procede normalmente, il programma è al primo ciclo di crittazione
   }else if(actualStatus == 1){        /*Crypting*/
      printf("\a\033[1;31mAttenzione\033[0m: precedentemente si è inserita una stringa\n");
      printf("da criptare; continuando i dati verranno sovrascritti!\n");
      printf("Scrivendo \033[1;36mbackToMain\033[0m come stringa, si tornerà al menu principale.\n");
   }else{         /*Decrypting*/
      printf("\aE' stata appena decriptata una stringa;\n");
      printf("Continuando, i precendenti dati verranno sovrascritti.\n");
      actualStatus = Free;
   }//Fine dell'if

   /*Inserimento della stringa*/
   for(si i = 0; i <= 128; i++){
      stringaInserita[i] = '\0';
   } //Pulisce la stringa prima di riempirla, utile soprattutto se il programma sta criptando molte stringhe
   printf("\nInserisci la stringa da criptare: \033[1;36m");
   fgets(stringaInserita, 129, stdin);       //Salva fino a 128 caratteri della stringa nella variabile <stringaInserita>, dalla cella 0 a 127
   troppoLunga = 0; //La chiave è meno di 128 caratteri
   //Non è necessario pulire il buffer di lettura se questo è già vuoto: dobbiamo verificare se l'utente abbia inserito 128 o più caratteri e in caso pulire!
   if(stringaInserita[127] != '\n' && stringaInserita[127] != '\0'){   //Avendo pulito il vettore prima di acquisire, ora sappiamo che sull'ultima cella acquisita (127) ci deve essere \0 o al più \n
      while((trash = getchar()) != '\n' && trash != EOF);      //L'utente ha messo più di 128 caratteri: dobbiamo pulire tutto ciò che è in più!
      troppoLunga = 1; //Avvisa che la chiave deve essere di 128
      printf("\n"); //La stringa era lunga al massimo, meglio avere un altro a capo per migliorare la vista
   }else if(stringaInserita[0] == '\n'){
      printf("\a\033[1;31mStringa troppo breve\033[0m!\n");
      Crypt(); //Reinserimento della stringa
   }
   printf("\033[0m");

   /*Verifica presenza escape-code to mainMenu*/
   //WARNING: TO FIX
   char escapeSequence[] = "backToMain";
   si matchCounter = 0;       //Conta le corrispondenze nel for di seguito
   if(strlen(stringaInserita)  < 11){ //Se ha più di 10 caratteri (e un \n) sicuramente non è l'scape code
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
      printf("\nLa tua scelta[Invio = 2]: \033[1;34m");
      scanf("%c", &antiBug);
      printf("\033[0m");      //Ripristina il colore di default

      /*Interpretiamo l'inserimento*/
      if(antiBug == '\n'){
         sceltaMenus = 2;     //Se l'utente preme invio senza scrivere, viene interpretato come 2
      }
      else if(antiBug < 48 || antiBug > 57){   //I numeri in ascii vanno da 48 a 57, quindi questa condizione equivale a "Se l'utente non inserisce un numero"
         printf("\a\033[1;31mData la scelta non valida, si procede \nautomaticamente con l'opzione 2\033[0m.\n");
         sleep(2);
         sceltaMenus = 2;
         while((trash = getchar()) != '\n' && trash != EOF); //Il buffer contiene ancora almeno un \n sicuramente: puliamo
      }else{
         sceltaMenus = (si)antiBug - 48;
         while((trash = getchar()) != '\n' && trash != EOF); //Il buffer contiene ancora almeno un \n sicuramente: puliamo
      }//Fine dell'if per controllo

      /*Gestisci l'inserimento della scelta della chiave*/
      switch(sceltaMenus){
         case 0:        /*Torna al menu principale*/
         MenuPrincipale(1);
         break;

         case 1:        /*Inserisci una chiave manualmente*/
         for(si i = 0; i <= 128; i++){
            chiave[i] = '\0';
         }//Pulizia della chiave
         do{
         troppoLunga == 0 ? printf("Digita di seguito la chiave (almeno %ld caratteri): \033[1;36m", strlen(stringaInserita) - 1) : printf("Digita di seguito la chiave (almeno %ld caratteri): \033[1;36m", strlen(stringaInserita));
         fgets(chiave, 129, stdin);       //Salva fino a 128 caratteri della stringa nella variabile <chiave>
         if(chiave[127] != '\n' && chiave[127] != '\0'){
            while((trash = getchar()) != '\n' && trash != EOF);   //Pulisce i caratteri in più
         }
         printf("\033[0m");

         /*Verifica se la chiave coincide con la stringa*/
         coincidenzaChiave = strlen(stringaInserita);
         for(si i = 0; i < strlen(chiave) + 1; i++) {    //Verifichiamo che le stringhe non siano uguali!
               if(chiave[i] == stringaInserita[i]){
                  coincidenzaChiave--;
               }
         }//Fine del for per confrontare chiave e stringa

         if(strlen(chiave) < strlen(stringaInserita)){
             printf("\a\n\033[1;31mLa chiave inserita è troppo corta\033[0m;\n");
         }else if(coincidenzaChiave == 0 && strlen(chiave) > strlen(stringaInserita)){
               printf("\a\n\033[1;31mLa chiave non può contenere la stringa\033[0m!\n"); //Ripeti il do-while perchè la chiave non va bene
         }else if (coincidenzaChiave < 0){
               printf("\a\n\033[1;31mLa chiave e la stringa non possono coincidere\033[0m!\n"); //Ripeti il do-while perchè la chiave non va bene
         }else{
            printf("La chiave è \033[1;36m%s\033[0m\n", chiave);
         }//Fine dell'if per stampare la frase
      }while(strlen(chiave) < strlen(stringaInserita) || coincidenzaChiave == 0 || coincidenzaChiave < 0);  //Fine del controllo della chiave

         printf("\a\033[1;31mAttenzione\033[0m: essendo stata inserita manualmente la chiave, la\nstringa cifrata potrebbe contenere molti caratteri insoliti.\n");
         sleep(2);
         break;

         default:       /*Scelta non valida*/
         printf("\a\033[1;31mScelta non valida\033[0m! Verrà generata una chiave\n casuale. ");
         sleep(2);      //Lascia il tempo di leggere il messaggio
         //Omettendo il break, si eseguirà il case 2, cioè la generazione casuale della chiave, evitando di ripetere i comandi!

         case 2:        /*Generazione chiave casuale*/
         printf("Generazione della chiava in maniera casuale...\n");
         time_t t;
         srand((unsigned) time(&t));   //Inizializzato il generatore random
         for(si i = 0; i <= 128; i++){
            chiave[i] = '\0';
         }  //Pulisce la chiave precedente (eventuale)
         for(si i = 0; i < strlen(stringaInserita) - 1; i++){     //Con "-1" escludiamo il "\n"!
            chiave[i] = rand() % 96 + 32;    //Vengono esclusi i caratteri speciali per una stringa cifrata più visual-friendly
         } //Fine della generazione della chiave
         printf("La chiave è \033[1;36m%s\033[0m\n", chiave);
         break;

      }//Fine del switch

      /*Salvataggio della chiave nella hystory*/
      for(si i = 128; i < 256; i++){  //Pulizia
         storico[i] = '\0';
      }
      for(si i = 0; i < strlen(chiave); i++){   //Scrittura
         storico[i + 128] = chiave[i];
      }
      /*Processo di criptazione*/
      actualStatus = Crypting;      //Comunica al resto del programma che è iniziato un processo di criptazione
      for(si i = 0; i <= 128; i++){
         stringaCifrata[i] = '\0';
      }//Pulisce dalla precedente (eventuale) stringa criptata
      for(si i = 0; i < strlen(stringaInserita); i++){      //Si esclude con lo strettamente minore il carattere \n
         stringaCifrata[i] = stringaInserita[i] ^ chiave[i];
      }//Fine del for per la criptazione
      sleep(2);      //Pausa ad effetto
      printf("\n\a>>La stringa è stata criptata: \n\033[1;36m%s\033[0m\n", stringaCifrata);     //Si va a capo perchè se venisse generato il carattere backspace, la stringa criptata coprirebbe il testo.
      /*Salvataggio della stringa cifrata*/
      for(si i = 0; i < 128; i++){  //Pulizia
         storico[i] = '\0';
      }
      for(si i = 0; i < strlen(stringaCifrata); i++){   //Scrittura
         storico[i] = stringaCifrata[i];
      }
      printf("Avvio del menu principale...\n");
      sleep(6);      //Diamo all'utente il tempo di leggere
      MenuPrincipale(1);

   }//Fine dell'if per controllo escapeSequence

}//Fine di Crypt

void Decrypt(){
   //Private variables:
   ch stringaDecifrata[129];
   ch confirm = 'S'; //Variabile per confermare la decriptazione

   system("clear");
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
         scanf("%c", &antiBug);

         /*Interpretiamo l'inserimento*/
         if(antiBug == '\n'){
            confirm = 'S';     //Se l'utente preme invio senza scrivere, viene interpretato come Y
            printf("\a\033[1;31mChi tace acconsente\033[0m.\n");     //Faccio finta che non hai fatto apposta
         }
         else if(antiBug != 83 && antiBug != 115 && antiBug != 78 && antiBug != 110)
         {
            confirm = 'e'; //e = errore, carattere non valido
            while((trash = getchar()) != '\n' && trash != EOF); //Il buffer contiene ancora almeno un \n sicuramente: puliamo
         }else{
            confirm = (char)antiBug;
            while((trash = getchar()) != '\n' && trash != EOF); //Il buffer contiene ancora almeno un \n sicuramente: puliamo
         }//Fine dell'if per controllo
         if(confirm == 'S' || confirm == 's'){
            for(si i = 0; i < 128; i++){
               stringaDecifrata[i] = '\0';
            }  //Pulisce dalle precedenti stringhe
            printf("Avvio della decriptazione...\n");
            actualStatus = 2;    //Imposta lo stato di decriptazione
            for(si i = 0; i <= strlen(stringaCifrata); i++){   //Minore e uguale per non escludere l'ultimo carattere della stringa
               stringaDecifrata[i] = stringaCifrata[i] ^ chiave[i];
            }
            sleep(2);      //Solita pausa ad effetto
            printf("\n\a>>La stringa decriptata è \033[1;36m%s\033[0m\n", stringaDecifrata);
            /*Salvataggio della stringa*/
            for(si i = 256; i <= 385; i++){  //Pulizia
               storico[i] = '\0';
            }
            for(si i = 0; i < strlen(stringaDecifrata); i++){   //Scrittura
               storico[i + 256] = stringaDecifrata[i];
            }
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

void LastString(){
   //Private variables:
   ch choice[2] = {'\0', '\0'};

   system("clear");
   printf("\033[01;33mStorico stringhe\033[0m:\n");
   printf("Ultima stringa cifrata: \n\t\033[1;36m");
   /*Stampiamo la stringa cifrata*/
   if(storico[0] == '\0'){
      printf("\033[1;31mNessun dato ancora\033[0m!");
   }else{
      for(si i = 0; i < 128; i++){
         if(storico[i] != '0')
            printf("%c", storico[i]);
         else
            break;
      }
   }
   printf("\n\033[0mUltima chiave usata: \n\t\033[1;36m");
   /*Stampiamo la chiave*/
   if(storico[128] == '\0'){
      printf("\033[1;31mNessun dato ancora\033[0m!");
   }else{
      for(si i = 128; i < 256; i++){
         if(storico[i] != '0')
            printf("%c", storico[i]);
         else
            break;
      }
   }
   printf("\n\033[0mUltima stringa decifrata: \n\t\033[1;36m");
   /*Stampiamo la stringa decifrata*/
   if(storico[256] == '\0'){
      printf("\033[1;31mNessun dato ancora\033[0m!");
   }else{
      for(si i = 256; i < 384; i++){
         if(storico[i] != '0')
            printf("%c", storico[i]);
         else
            break;
      }
   }
   printf("\n\n\033[1;32mPremere Invio per tornare al menu\033[0m. ");
   fgets(choice, 3, stdin);
   if(choice[0] == '\n'){
      MenuPrincipale(1);
   }else if(choice[1] != '\0' && choice[1] != '\n'){
      //Torniamo al menu lo stesso, ma puliamo il buffer
      while((trash = getchar()) != '\n' && trash != EOF);
      MenuPrincipale(1);
   }else{   //Prima di premere invio ha premuto un tasto...fa niente
      MenuPrincipale(1);
   }
}//Fine di LastString
