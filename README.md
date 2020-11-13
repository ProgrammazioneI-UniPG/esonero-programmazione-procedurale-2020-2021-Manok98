###############
#RC4-Encrypter#
###############

-Nome: Benedetto

-Cognome: Manoni

-N°Matricola: `329636`


Please open this file with Github Markdown syntax!       
SourceCode File: On "Debug"(Master) branch the source file is _"cifrario.c"_;      
Executable Program: _RC4-Encrypter_;     

_Source code notes:_    
1)Il file `rc4.fc` contiene il diagramma di flusso del programma.       

2)_Perchè si sono inizializzati gli array a 129?_     
   La risposta è semplice: è bene lasciare un carattere terminatore alla fine di ogni array ('\0'). Tenendo poi conto del fatto che     
   la funzione fgets acquisisce n-1 caratteri si è scritto nel codice, dove compare fgets, 129 e non 128. Scrivendo infatti 129, si      
   acquisiscono 128 caratteri effettivi che vengono salvati nelle cella da 0 a 127. `E' errore scrivere fgets con 128, perchè non    rispetteremmo la richiesta del testo dell'esonero`, dato che, anziché consentire l'acquisizione di 128 caratteri, ne permetteremo     
   la scrittura di soli 127!     

   
#END of ReadMe file#
