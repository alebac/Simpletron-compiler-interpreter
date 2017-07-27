/* Interprete di Simple */

/* nella scrittura dei programmi le righe vanno numerate a partire da 0 poi 1, 2, 3, 4, 5... */

/* I comandi possibili sono */
// rem
// input 
// print
// let
// goto
// if...goto
// end

#include"calcinterprete.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define READ 10
#define WRITE 11
#define NEWLINE 12
#define CONST 13
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

struct tableEntry {
   int symbol;
   char type;         //  'C' costante, 'L' numero riga, oppure 'V' variabile
   int location;      // posizione nella memoria simpletron da 00 a 99.
                      //  Se è un numero di riga rappresenta l'elemento nella memoria simpletron 
                      // da cui inizieranno le istruzioni codificate in quella linea.
                      //  Se è una variabile o costante rappresenta l'elemento della memoria in 
                      // cui è immagazzinata.Queste saranno immagazzinate a partire dal fondo.
};

int operatore( char c );
int cerca( struct tableEntry * symbolTable, int data );
void inserisciVariabile( struct tableEntry * symbolTable, int data, int *contatoreDatiPtr, int *contatoreTabellaPtr );
void inserisciLinea( struct tableEntry * symbolTable, int data, int *contatoreIstruzioniPtr, int *contatoreTabellaPtr );
void inserisciCostante( struct tableEntry * symbolTable, int data, int *contatoreDatiPtr, int *contatoreTabellaPtr );
void stampaTabella( struct tableEntry * symbolTable , int * contatoreTabellaPtr);
void inizializzaTabella( struct tableEntry * symbolTable );
void azzeraStringa( char stringa[30] );

int main( int argc, char *argv[] )
{
   struct tableEntry symbolTable[100];
   int LMS[1000] = { 0 };
   int contatoreDati = 99;
   int *contatoreDatiPtr;
   contatoreDatiPtr = &contatoreDati;
   int contatoreIstruzioni = 0;
   int *contatoreIstruzioniPtr;
   contatoreIstruzioniPtr = &contatoreIstruzioni;
   int contatoreTabella = 1;
   int *contatoreTabellaPtr;
   contatoreTabellaPtr = &contatoreTabella;
   char stringa[30] = { 0 };
   char *tokenPtr;
   char istruzione[10] = { 0 };
#ifdef DEBUG
   int i;                                   // contatore
   int j;
#endif
   int a = 0;                               // serve per la scelta if: ==, !=, >, <.
   char infix[30] = { 0 };
   char postfix[30] = { 0 };
   int store;                               // salvo la locazione di memoria a cui assegnare il risultato del let
   StackNodePtr stackPtr = NULL;   

    long int accumulatore = 0;
    long int memoria [100] = { 0 };            /*vettore memoria*/
    int m;  
    long int registroIstruzione;
    int funzione;
    int indice;
    int contatoreIstruzione = 0;
    int x,y;       
   
   int saltato; 
   int ctvett = 0;
   int ctvett2 = 0;
   char vettore[20][30];                    // vettore in cui sono raccolte le istruzioni                   
   inizializzaTabella( symbolTable );       // inizializza gli elementi symbol della Tabella a -999999
      
   FILE *cfPtr;

   if( argc != 2){
      printf("Utilizzo: ./inteprete nomeSorgente\n");
      return 0;
   }    

#ifdef DEBUG
   i = 0; 
#endif                                                    

   if( ( cfPtr = fopen( argv[1], "r" ) ) == NULL ){
      printf("File could not be opened\n");
      }
   else {      
      while ( !feof( cfPtr ) ) { 
         azzeraStringa( stringa );
         leggicomp( cfPtr, stringa );
         strcpy( vettore[ ctvett2++ ], stringa );
         if( !strcmp( stringa, "end ") ) {
#ifdef DEBUG
            printf("fuori!\n");
#endif    
            break;
            }
      }  // fine while lettura file
   }     // fine else
         
#ifdef DEBUG
   for( i = 0; i < ctvett2; ++i )
      printf("riga %d: %s\n", i, vettore[ i ]);
   i = 0; 
#endif      

    for( ctvett = 0; ctvett<=ctvett2; ++ctvett ) {
#ifdef DEBUG
         printf("ctvett = %d, ctvett2 = %d\n", ctvett, ctvett2 );
         printf("riga %d letta:      %s\n", ctvett, vettore[ ctvett ] );                    
#endif

         /*primo token, istruzione*/
         strcpy( stringa, vettore[ ctvett ] );   
         tokenPtr = strtok( stringa, " " );
         strcpy( istruzione, tokenPtr );                 // così gli if valutano istruzione e non tokenPtr che poi viene modificato

         if ( !strcmp( istruzione, "rem" ) );            // se è rem non c'è più un tubo da fare...

         if ( !strcmp( istruzione, "input" ) ) {         // se è input il simbolo sucessivo sarà una variabile
            tokenPtr = strtok( NULL, " " );
            if( !cerca( symbolTable, *tokenPtr ) )
               inserisciVariabile( symbolTable, *tokenPtr, contatoreDatiPtr, contatoreTabellaPtr );
            LMS[ contatoreIstruzioni++ ] = 1000 + symbolTable[ cerca( symbolTable, *tokenPtr ) ].location;
            }

         if ( !strcmp( istruzione, "print" ) ) {
            tokenPtr = strtok( NULL, " " );
            LMS[ contatoreIstruzioni++ ] = 1100 + symbolTable[ cerca( symbolTable, *tokenPtr ) ].location;   
            }

         if ( !strcmp( istruzione, "let" ) ) { 
            tokenPtr = strtok( NULL, " " );      // ora c'è la variabile a cui sarà assegnato il risultato
            if( !cerca( symbolTable, *tokenPtr ) )                  
               inserisciVariabile( symbolTable, *tokenPtr, contatoreDatiPtr, contatoreTabellaPtr );
            store =  symbolTable[ cerca( symbolTable, *tokenPtr ) ].location; // qui mettiamo il risultato, *tokenPtr perchè è una variabile
            tokenPtr = strtok( NULL, " " );      // questo è =
            tokenPtr = strtok( NULL, " " );      // ora iniziamo a creare infix
            azzeraStringa( infix );
            while ( tokenPtr != NULL ) {
               if( isVariabile( *tokenPtr ) ) 
                  if( !cerca( symbolTable, *tokenPtr ) )                  
                     inserisciVariabile( symbolTable, *tokenPtr, contatoreDatiPtr, contatoreTabellaPtr );
               if( isNumber( *tokenPtr ) )
                  if( !cerca( symbolTable, atoi( tokenPtr) ) ) {
                     inserisciCostante( symbolTable, atoi( tokenPtr ), contatoreDatiPtr, contatoreTabellaPtr ); 
                     LMS[ contatoreIstruzioni++ ] = 1300 + symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location;
                     LMS[ contatoreIstruzioni++ ] = atoi( tokenPtr );
                     }
               strcat( infix, tokenPtr );
               strcat( infix, " ");
               tokenPtr = strtok( NULL, " " );
               }
#ifdef DEBUG
            printf("%s\n", infix );
#endif
            azzeraStringa( postfix );
            convertToPostfix( infix, postfix ); 
#ifdef DEBUG
            printf("convert to postfix fatto\n");
#endif  
#ifdef DEBUG
            printf("%s\n", postfix );
#endif
            tokenPtr = strtok( postfix, " " );
            while( tokenPtr != NULL ) {
               if( isNumber( *tokenPtr ) )
                  push( &stackPtr, symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location );
               else if( isVariabile( *tokenPtr ) )
                  push( &stackPtr, symbolTable[ cerca( symbolTable, *tokenPtr ) ].location );
               else if( isOperator( *tokenPtr ) ) {
                  LMS[ contatoreIstruzioni++ ] = 2000 + pop( &stackPtr );                    // questa va nell'accumulatore
                  LMS[ contatoreIstruzioni++ ] = operatore( *tokenPtr ) + pop( &stackPtr );  // eseguiamo l'operazione
                  LMS[ contatoreIstruzioni++ ] = 2100 + contatoreDati;                       // store temporaneo
                  push( &stackPtr, contatoreDati-- );                                        // risultato messo nella pila e contatoreDati--
                  }
#ifdef DEBUG                 
               printf("Espressione valutata: %s\n", tokenPtr);
#endif                  
               tokenPtr = strtok( NULL, " " );
               }
#ifdef DEBUG                 
               printf("Ne siamo usciti\n");
#endif  
            LMS[ contatoreIstruzioni++ ] = 2000 + pop( &stackPtr );   // carico nell'accumulatore il risultato che è rimasto nella pila
            LMS[ contatoreIstruzioni++ ] = 2100 + store;              // e lo salvo in store
#ifdef DEBUG                 
            printf("Fatto anche il pop\n");
#endif  
            }

         if ( !strcmp( istruzione, "goto" ) ) {
            tokenPtr = strtok( NULL, " " );  
            LMS[ contatoreIstruzioni++ ] = 4000 + atoi( tokenPtr ); 
            }

         if ( !strcmp( istruzione, "if" ) ) {
            tokenPtr = strtok( NULL, " " );     // y
            if( isVariabile( *tokenPtr ) ) 
               if( !cerca( symbolTable, *tokenPtr ) )                  
                  inserisciVariabile( symbolTable, *tokenPtr, contatoreDatiPtr, contatoreTabellaPtr );
            if( isNumber( *tokenPtr ) )
               if( !cerca( symbolTable, atoi( tokenPtr) ) ) {
                  inserisciCostante( symbolTable, atoi( tokenPtr ), contatoreDatiPtr, contatoreTabellaPtr ); 
                  LMS[ contatoreIstruzioni++ ] = 1300 + symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location;
                  LMS[ contatoreIstruzioni++ ] = atoi( tokenPtr );
                  }
            if( isVariabile( *tokenPtr ) )
               LMS[ contatoreIstruzioni++ ] = 2000 + symbolTable[ cerca( symbolTable, *tokenPtr ) ].location;    //mette y nell'acumulatore
            if( isNumber( *tokenPtr ) ) 
               LMS[ contatoreIstruzioni++ ] = 2000 + symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location;  // se è una costante...

            tokenPtr = strtok( NULL, " " );     // operatore, c'è da scegliere tra branchzero, neg..
            if( !strcmp( tokenPtr, "==" )) a = 1;    // branchzero
            if( !strcmp( tokenPtr, "!=" )) a = 2;    // la classe non è acqua 2 istruzioni...
            if( !strcmp( tokenPtr, ">" )) a = 3;     // branchneg
            if( !strcmp( tokenPtr, "<" )) a = 4;     // la classe non è acqua 2 istruzioni...

            tokenPtr = strtok( NULL, " " );     // x
            if( isVariabile( *tokenPtr ) ) 
               if( !cerca( symbolTable, *tokenPtr ) )                  
                  inserisciVariabile( symbolTable, *tokenPtr, contatoreDatiPtr, contatoreTabellaPtr );
            if( isNumber( *tokenPtr ) )
               if( !cerca( symbolTable, atoi( tokenPtr) ) ) {
                  inserisciCostante( symbolTable, atoi( tokenPtr ), contatoreDatiPtr, contatoreTabellaPtr ); 
                  LMS[ contatoreIstruzioni++ ] = 1300 + symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location;
                  LMS[ contatoreIstruzioni++ ] = atoi( tokenPtr );
                  }
            if( isVariabile( *tokenPtr ) )
               LMS[ contatoreIstruzioni++ ] = 3100 + symbolTable[ cerca( symbolTable, *tokenPtr ) ].location;    // fa x - y cioè il secondo meno il primo
            if( isNumber( *tokenPtr ) ) 
               LMS[ contatoreIstruzioni++ ] = 3100 + symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location;  // se è una costante...

            tokenPtr = strtok( NULL, " " );     // goto non serve, occhio al numero che viene
            tokenPtr = strtok( NULL, " " );     // numero dove saltare --> flagssss
            if( a == 1 ) { 
               LMS[ contatoreIstruzioni++ ] = 4200 + atoi( tokenPtr );   
               }
            if( a == 2 ) {
               LMS[ contatoreIstruzioni ] = 4200 + ctvett + 1; 
               contatoreIstruzioni++;
               LMS[ contatoreIstruzioni++ ] = 4000 + atoi( tokenPtr ); 
               }
            if( a == 3 ) { 
               LMS[ contatoreIstruzioni++ ] = 4100 + atoi( tokenPtr );     
               }  
            if( a == 4 ) {
               LMS[ contatoreIstruzioni ] = 4100 + ctvett + 1; 
               contatoreIstruzioni++;
               LMS[ contatoreIstruzioni++ ] = 4000 + atoi( tokenPtr ); 
               }   
            }

         if ( !strcmp( istruzione, "end" ) ) {
            LMS[ contatoreIstruzioni++ ] = 4300;  
            }
#ifdef DEBUG
   for( j = 0; LMS[j] != 0; ++j )
      printf("%.2d %d\n", j, LMS[j] );
   stampaTabella( symbolTable, contatoreTabellaPtr );
   printf( "contatoreIstruzione = %d\n", contatoreIstruzione );      
#endif       


     while( LMS[contatoreIstruzione] != 0 )
      {
       registroIstruzione = LMS[contatoreIstruzione];
#ifdef DEBUG
       printf("Eseguo l'istruzione %ld\n", registroIstruzione);
#endif
       indice = registroIstruzione % 100;
       funzione = registroIstruzione / 100;
       saltato = 0;

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

           case CONST:                                                              // per inserire una costante ci vanno 2 istruzioni
              printf("Nuova costante!\n");
              contatoreIstruzione++;
              memoria[indice] = LMS[contatoreIstruzione];
              printf("%d inserito nella locazione di memoria %d\n\n", LMS[contatoreIstruzione], indice );
              break;
                
           case LOAD:
              printf("Ho caricato il numero contenuto nella locazione\ndi memoria %d nell'accumulatore\n\n", indice);
              accumulatore = memoria [indice];  
              break;
              
           case STORE:
              printf("Ho salvato il numero contenuto nell'accumulatore\nnella locazione di memoria %d\n\n", indice);
              memoria[indice] = accumulatore;
              break;
              
           case ADD:
              printf("Ho sommato al numero contenuto nell'accumulatore\nquello contenuto nella locazione di memoria %d\n\n", indice);
              accumulatore += memoria[indice]; 
              break;
               
           case SUBTRACT:
              printf("Ho sottratto al numero contenuto nell'accumulatore\nquello contenuto nella locazione di memoria %d\n\n", indice);
              accumulatore = memoria[indice] - accumulatore; 
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
              ctvett = indice - 1;
              printf("Salto alla riga %d\n\n", indice); 
              saltato = 1;
              break;
               
           case BRANCHNEG:
              printf("Se il numero contenuto nell'accumulatore è negativo\nsalto all'istruzione %d\n\n", indice); 
              if( accumulatore < 0 ) {
                 ctvett = indice - 1;
                 printf("Salto alla riga %d\n\n", indice ); 
                 saltato = 1;
                 }
              break;
              
           case BRANCHZERO:
              printf("Se il numero contenuto nell'accumulatore è uguale a zero\nsalto all'istruzione %d\n\n", indice);
              if( accumulatore == 0 ) {
                 ctvett = indice - 1;
                 printf("Salto alla riga %d\n\n", indice ); 
                 saltato = 1;
                 }
              break;
               
           case HALT:  
              printf("*** Simpletron execution terminated ***\n\n\n\n");  /*al dump del computer ci si pensa dopo*/     
              printf("REGISTRI\n");
              printf("accumulatore            %.4ld\n", accumulatore);
              printf("contatoreIstruzione     %.2d\n", contatoreIstruzione);
              printf("registroIstruzione      %.4ld\n", registroIstruzione);
              printf("funzione                %.2d\n", funzione);
              printf("indice                  %.2d\n", indice);
              printf("MEMORIA\n\n");
              
              
              m = 0;
              printf("    ");
              for( x = 0 ; x < 10 ; x++ ){
                 printf("%d     ",x);  }
              printf ("\n\n");
              for( x = 0 ; x < 10 ; x++ ){
                 printf("%.2d  ", x * 10);
                 for ( y = 1 ; y <= 10 ; y++ ){
                    printf ("%.4ld  ", memoria[m]);
                    m++;}
              printf ("\n\n");
              }
                  
              return 0;
              
              break;
           }
         contatoreIstruzione++; 

         if( saltato == 1 ) break;    
       }
   }               // fine for possente
 
   /* fine */                          
   return 0;
}

int operatore( char c )
{
   int a;
   
   if     ( c == '+' ) a = 3000;
   else if( c == '-' ) a = 3100;
   else if( c == '/' ) a = 3200;
   else if( c == '*' ) a = 3300;
   else if( c == '%' ) a = 3400;

   return a;
}

int cerca( struct tableEntry * symbolTable, int data )
{
   int i;
   for( i = 1; i < 100; ++i )
      if( symbolTable[i].symbol == data )
         return i;
   return 0;          
}

void inserisciVariabile( struct tableEntry * symbolTable, int data, int *contatoreDatiPtr, int *contatoreTabellaPtr )
{
   symbolTable[ *contatoreTabellaPtr ].symbol = data;
   symbolTable[ *contatoreTabellaPtr ].type = 'V'; 
   symbolTable[ (*contatoreTabellaPtr)++ ].location = (*contatoreDatiPtr)--;
}

void inserisciCostante( struct tableEntry * symbolTable, int data, int *contatoreDatiPtr, int *contatoreTabellaPtr )
{
   symbolTable[ *contatoreTabellaPtr ].symbol = data;
   symbolTable[ *contatoreTabellaPtr ].type = 'C'; 
   symbolTable[ (*contatoreTabellaPtr)++ ].location = (*contatoreDatiPtr)--;
}

void inserisciLinea( struct tableEntry * symbolTable, int data, int *contatoreIstruzioniPtr, int *contatoreTabellaPtr )
{
   symbolTable[ *contatoreTabellaPtr ].symbol = data;
   symbolTable[ *contatoreTabellaPtr ].type = 'L'; 
   symbolTable[ (*contatoreTabellaPtr)++ ].location = *contatoreIstruzioniPtr;
}

void stampaTabella( struct tableEntry * symbolTable , int *contatoreTabellaPtr)
{
   int i;
   for( i = 1; i < *contatoreTabellaPtr; ++i )
      printf("%d %c %.2d\n", symbolTable[i].symbol, symbolTable[i].type, symbolTable[i].location );
}

void inizializzaTabella( struct tableEntry * symbolTable )
{
   int i;
   for( i = 1; i < 100; ++i )
      symbolTable[i].symbol = -999999;      
}

void azzeraStringa( char stringa[30] )
{
   int i;
   for( i = 0; i < 30; ++i )
      stringa[i] = '\0';
}
