/*Simpletron durante malattia prima modifca,
  ritorno al passato ed eliminazione pow */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define READ 10
#define WRITE 11
#define NEWLINE 12
#define LOAD 20
#define STORE 21
#define ADD 30
#define SUBTRACT 31
#define DIVIDE 32
#define MULTIPLY 33
#define MOD 34
#define POW 35
#define BRANCH 40
#define BRANCHNEG 41
#define BRANCHZERO 42
#define HALT 43




int main()                /* funzione principale*/

   {
    long int accumulatore=0;
    long int memoria [100];         /*vettore memoria*/
    int m;
    
    for(m=0;m<100;m++){
       memoria [m] = 0;}


    printf("\n\n\n");
    printf("*** Welcome to Simpletron! ***\n");
    printf("*** Please enter your program one instruction ***\n");
    printf("*** (or data word) at a time. I will type the ***\n");
    printf("*** location number and a question mark (?).  ***\n");
    printf("*** You then type the word for that location. ***\n");
    printf("*** Type the sentinel -1 to stop entering ***\n");
    printf("*** your program. ***\n\n\n\n");
    
    long int registroIstruzione;
    int funzione;
    int indice;
    int i = 0;                   /* contatori vari */ 
    int contatoreIstruzione;
    int x,y;
    
    
    
    
       
    while (accumulatore != -1)
       { 
       do {
           printf("%.2d ? ", i); 
           scanf("%ld", &accumulatore);
           }while( accumulatore < -1 || accumulatore >= 10000 );
           
       memoria[i] = accumulatore;
       ++i;       
       }
    
    
    printf("\n\n");
    printf("*** Program loading completed ***\n");
    printf("*** Program execution begins  ***\n");  
    printf("\n\n");  
    
    
    
    for( contatoreIstruzione = 0; contatoreIstruzione<=i; ++contatoreIstruzione)
      {
       registroIstruzione = memoria[contatoreIstruzione];
       indice = registroIstruzione % 100;
       funzione = registroIstruzione / 100;
       
       switch (funzione)
          {
           case READ:
              printf("Immetti un numero da inserire nella locazione di memoria %d\n", indice);
              scanf("%ld", &memoria[indice]);
              printf("\n");
              break;
                
           case WRITE:
              printf("Il numero contenuto nella locazione di memoria %d e':\n", indice);
              printf("%ld\n\n", memoria[indice]);
              break;
           
           case NEWLINE:
              printf("\n");
              break;
                
           case LOAD:
              printf("Ho caricato il numero contenuto nella locazione\ndi memoria %d nell'accumulatore\n\n", indice);
              accumulatore = memoria [indice];  
              break;
              
           case STORE:
              printf("Ho caricato il numero contenuto nell'accumulatore\nnella locazione di memoria %d\n\n", indice);
              memoria[indice] = accumulatore;
              break;
              
           case ADD:
              printf("Ho sommato al numero contenuto nell'accumulatore\nquello contenuto nella locazione di memoria %d\n\n", indice);
              accumulatore += memoria[indice]; 
              break;
               
           case SUBTRACT:
              printf("Ho sottratto al numero contenuto nell'accumulatore\nquello contenuto nella locazione di memoria %d\n\n", indice);
              accumulatore -= memoria[indice]; 
              break;
               
           case DIVIDE:
              if (accumulatore == 0) 
                 {printf("*** Attempt to divide by zero ***\n");
                  printf("*** Simpletron ezecution abnormally terminated ***\n");}
              else accumulatore = memoria[indice]/accumulatore;
              printf("Ho diviso il numero contenuto nell'accumulatore\nper quello contenuto nella locazione di memoria %d\n\n", indice);  
              break;
              
           case MULTIPLY:
              accumulatore = memoria[indice]*accumulatore; 
              printf("Ho moltiplicato il numero contenuto nell'accumulatore\nper quello contenuto nella locazione di memoria %d\n\n", indice); 
              break;
               
           case MOD:
              printf("Ho diviso il numero contenuto nell'accumulatore\nper quello contenuto nella locazione di memoria %d\n\n", indice);
              accumulatore %= memoria[indice]; 
              break;
              
           case BRANCH:
              contatoreIstruzione = indice; 
              printf("Salto all'istruzione %d\n\n", indice); 
              break;
               
           case BRANCHNEG:
              if(accumulatore < 0) {contatoreIstruzione = indice;  }
              printf("Se il numero contenuto nell'accumulatore è negativo\nsalto all'istruzione %d\n\n", indice); 
              break;
              
           case BRANCHZERO:
              printf("Se il numero contenuto nell'accumulatore è uguale a zero\nsalto all'istruzione %d\n\n", indice);
              if(accumulatore == 0) {contatoreIstruzione = indice;}
              break;
               
           case HALT:  
              printf("*** Simpletron execution terminated ***\n\n\n\n");  /*al dump del computer ci si pensa dopo*/     
              printf("REGISTRI\n");
              printf("accumulatore            %.4ld\n", accumulatore);
              printf("contatoreIstruzione     %.2d\n", contatoreIstruzione);
              printf("registroIstruzione      %.4ld\n", registroIstruzione);
              printf("funzione                %.2d\n", funzione);
              printf("indice                  %.2d\n", indice);
              printf("MEMORIA\n");
              
              
              m=0;
              printf("         ");
              for( x = 0 ; x < 10 ; x++ ){
                 printf("%d      ",x);  }
              printf ("\n\n");
              for( x = 0 ; x < 10 ; x++ ){
                 printf("%.2d  ", x * 10);
                 for ( y = 1 ; y <= 10 ; y++ ){
                    printf ("%.4ld  ", memoria[m]);
                    m++;}
              printf ("\n\n");
              }
                 
                 
              system("pause");     
              return 0;
              
              break;
           }
              
       }
    
     system("pause");
     
     return 0;
     
     }
