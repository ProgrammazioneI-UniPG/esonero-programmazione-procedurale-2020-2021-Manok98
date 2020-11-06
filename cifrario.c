#include <stdio.h>

int main(){
   char stringaDecifrata[128];
   printf("###############\nCifrario RC4\n\n");
   printf("Inserisci la stringa da cifrare: ");
   fgets(stringaDecifrata, 128, stdin);
   printf("La stringa inserita è %s\n", stringaDecifrata);
   return 0;
}
