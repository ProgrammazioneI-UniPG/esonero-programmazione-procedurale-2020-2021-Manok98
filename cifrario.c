#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef short int si;

//Prototypes:
char MenuPrincipale(si);
void Encrypt(char, char);
void Decrypt(char, char);

int main(){
   char stringaDecifrata[128];
   char chiave[128];
   printf("###############\nCifrario RC4\n\n");
   printf("Inserisci la stringa da cifrare: ");
   fgets(stringaDecifrata, 128, stdin);
   printf("La stringa inserita è \"%s\".\n\n", stringaDecifrata);
   chiave = MenuPrincipale(strlen(stringaDecifrata));    //Send the length of the entered string instead of all the array
   printf("\nAvvio della procedura di criptazione...");
   Encrypt(/*par 1, par 2*/);
   return 0;
}

char MenuPrincipale(si stringaDecifrataLength){
   time_t t;
   srand((unsigned) time(&t));   //Casual number generator initialization
   char sceltaMenu = '0';
   char chiave[128];
   printf("[0]Esci dal programma\n");
   printf("[1]Inserimento chiave personalizzata\n");
   printf("[2]Generazione chiave casuale\n");
   printf("\nSelezionare una delle seguenti opzioni: ");
   scanf("%c", &sceltaMenu);    //Acquisendo l'inserimento come carattere possiamo prevedere l'uso di elementi non consentiti al di fuori di numeri
   fflush(stdin); //Prevedendo che qualche utente curioso potrebbe sbattere la testa sulla tastiera e inserire troppi caratteri, viene pulito il buffer

   switch(sceltaMenu){
      case '0':  //Exit
      break; /*FIXME: PROGRAM MUST BE CLOSED: ADD EXIT(0); LIBRARY*/

      case '1':  //User insert manually the key
      printf("Si inserisca la chiave di almeno %hd caratteri: ", stringaDecifrataLength - 1);    //"-1" equals to remove the terminatory char (\0) as it should not be counted in stringaDecifrataLength
      while(strlen(chiave) < stringaDecifrataLength){
         fflush(stdin);    //Clean the buffer from precedent scan before scan again in order to avoid errors
         fgets(chiave, stringaDecifrataLength, stdin);
         strlen(chiave) < stringaDecifrataLength ? printf("\n\033[0;31m!La chiave non è abbastanza lunga!\033[0m Reinserirla: ") : printf("\nLa chiave inserita è la seguente: ");//Il messaggio viene (eventualmente) visulizzato in rosso per incrementare la caratteristica user-friendly
      }
      printf("\033[0;32m%s\033[0m\n", chiave);//Il messaggio viene visulizzato in verde per incrementare la caratteristica user-friendly
      break;

      case '2':  //Generate the key randomly
      for(si i = 0; i < stringaDecifrataLength - 1; i++){   //Key writing
         chiave[i] = (rand() % 96) + 32;  //Con la sottrazione di 32 da 128 si ottiene, con l'operatore modulo, un valore fino a 95; aggiungendo poi 32 si ottengono solo valori tra 32 e 127
      }
      printf("La chiave generata è \"%s\".\n", chiave);
      break;

      default:    //User entered a not valid character
      printf("\n\033[0;31m!E' stato inserita un'opzione non valida. La chiave verrà\ngenerata casualmente.\033[0m\n"); //Il messaggio viene visulizzato in rosso per incrementare la caratteristica user-friendly
      sleep(2);   //Let the user read the error message
      for(si i = 0; i < stringaDecifrataLength - 1; i++){   //Key writing
         chiave[i] = (rand() % 96) + 32;  //Con la sottrazione di 32 da 128 si ottiene, con l'operatore modulo, un valore fino a 95; aggiungendo poi 32 si ottengono solo valori tra 32 e 127
      }
      break;
   } //End of switch
}

void Encrypt(/*par1, par2*/){

}

void Decrypt(/*par1, par2*/){

}
