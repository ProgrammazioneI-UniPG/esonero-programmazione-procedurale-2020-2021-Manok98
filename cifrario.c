#include <stdio.h>
#include <stdlib.h>

typedef short int si

int main(){
   char stringaDecifrata[128];
   printf("###############\nCifrario RC4\n\n");
   printf("Inserisci la stringa da cifrare: ");
   fgets(stringaDecifrata, 128, stdin);
   printf("La stringa inserita è %s\n", stringaDecifrata);
   MenuPrincipale(strlen(stringaDecifrata));
   return 0;
}

void MenuPrincipale(si strLength){
   si sceltaMenu = 0;
   printf("\n[0]Esci dal programma\n");
   printf("[1]Inserimento chiave personalizzata\n");
   printf("[2]Generazione chiave casuale\n")
   printf("\nSelezionare una delle seguenti opzioni: ");
   do {
      scanf("%d", &sceltaMenu);
   }while(sceltaMenu != 0 && scletaMenu != 1 && sceltaMenu != 2);

   switch(sceltaMenu){
      case 0:  //Uscita dal programma
      break; /*FIXME: PROGRAM MUST BE CLOSED: ADD EXIT(0); LIBRARY*/
      case 1:  //Inserimento chiave dall'utente
      printf("Si inserisca la chiave di almeno %d caratteri", strLength);
      break;
      case 2:  //Generatore chiave casuale
      break;
   }
}
