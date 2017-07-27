/* Compilatore Simple */

/* I comandi possibili sono */
// rem
// input 
// print
// let
// goto
// if...goto
// end

// problemi con le costanti nei programmi non possono comparire, mancano istruzioni LMS adeguate per supportarle
// niente anche le costanti ora vanno

#include"calc.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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
void stampaFlags( int flags[100] );

int main( int argc, char *argv[] )
{
   struct tableEntry symbolTable[100];
   int flags[100];
   int LMS[100] = { 0 };
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
   int i;                                   // contatore
#ifdef DEBUG
   int j;
#endif
   int a = 0;                               // serve per la scelta if: ==, !=, >, <.
   char infix[30] = { 0 };
   char postfix[30] = { 0 };
   int store;                               // salvo la locazione di memoria a cui assegnare il risultato del let
   StackNodePtr stackPtr = NULL;   

   for( i = 0; i < 100; ++i )
      flags[i] = -1;                        // inizializzazione vettore flags a -1   
                       
   inizializzaTabella( symbolTable );       // inizializza gli elementi symbol della Tabella a -999999
      
   FILE *cfPtr;

#ifdef DEBUG
   i = 0; 
#endif    

   if( argc != 3 ){
      printf("Utilizzo: ./compilatore nomeSorgente nomeEseguibile\n");
      return 0;
   }                                                

   if( ( cfPtr = fopen( argv[1], "r" ) ) == NULL ){
      printf("File could not be opened\n");
      }
   else {      
      while ( !feof( cfPtr ) ) { 
         azzeraStringa( stringa );
         leggicomp( cfPtr, stringa );
         if( !isNumber( *stringa ) ) 
            break;                                      // vile astuzia, per finire si aggiunge la riga:  fine ;
                                                        // o qualsiasi altra minchiata che non inizi con un numero;
         
#ifdef DEBUG
         printf("riga %d letta:      %s\n", ++i, stringa );                    
#endif
  
         /* primo token, numero riga */
         tokenPtr = strtok( stringa, " " );
#ifdef DEBUG
         printf("atoi ha letto %d \n", atoi( tokenPtr ) );                    
#endif
         if( !cerca( symbolTable, atoi( tokenPtr )) ) {
            inserisciLinea( symbolTable, atoi( tokenPtr ), contatoreIstruzioniPtr, contatoreTabellaPtr );
#ifdef DEBUG
            printf("%d inserito nella tabella come numero di riga \n", atoi( tokenPtr ) );                    
#endif
            }

         /*secondo token, istruzione*/
         tokenPtr = strtok( NULL, " " );
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
            if( !cerca( symbolTable, atoi( tokenPtr ) ) ) {
               flags[ contatoreIstruzioni ] = atoi( tokenPtr );  
               LMS[ contatoreIstruzioni++ ] = 4000; 
               }
            else 
               LMS[ contatoreIstruzioni++ ] = 4000 + symbolTable[ cerca( symbolTable, atoi( tokenPtr ) ) ].location;
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
               flags[ contatoreIstruzioni ] = atoi( tokenPtr );  
               LMS[ contatoreIstruzioni++ ] = 4200;   
               }
            if( a == 2 ) {
               LMS[ contatoreIstruzioni ] = 4200 + contatoreIstruzioni + 2; 
               contatoreIstruzioni++;
               flags[ contatoreIstruzioni ] = atoi( tokenPtr ); 
               LMS[ contatoreIstruzioni++ ] = 4000; 
               }
            if( a == 3 ) {
               flags[ contatoreIstruzioni ] = atoi( tokenPtr );  
               LMS[ contatoreIstruzioni++ ] = 4100;     
               }  
            if( a == 4 ) {
               LMS[ contatoreIstruzioni ] = 4100 + contatoreIstruzioni + 2; 
               contatoreIstruzioni++;
               flags[ contatoreIstruzioni ] = atoi( tokenPtr ); 
               LMS[ contatoreIstruzioni++ ] = 4000; 
               }   
            }

         if ( !strcmp( istruzione, "end" ) ) {
            LMS[ contatoreIstruzioni++ ] = 4300;  
            break;                              // esce dal while di cfPtr
            }
#ifdef DEBUG
         for( j = 0; LMS[j] != 0; ++j )
            printf("%.2d %d\n", j, LMS[j] );
#endif  
#ifdef DEBUG
         stampaTabella( symbolTable, contatoreTabellaPtr );
         stampaFlags( flags );
         if( feof( cfPtr ) )
            printf("Lettura file terminata\n" );
#endif       
      }
   }   
#ifdef DEBUG      
   printf("primo passaggio completato\n");
#endif

   /* Secondo passaggio */

   for( i = 0; i < 100; ++i )
      if( flags[i] != -1 )
         LMS[i] += symbolTable[ cerca( symbolTable, flags[i] ) ].location;                    // bisogna completare questa istruzione
      
   /* Scrittura codice macchina ottenuto su file */
   if ( ( cfPtr = fopen( argv[2], "w" ) ) == NULL ){
      printf( "File could not be opened\n" );
	  }
   else {
      for( i = 0; LMS[i] != 0; ++i )
         fprintf( cfPtr, "%d\n", LMS[i] );
   }
   
   /* fine */                          
   return 0;
}

int operatore( char c )
{
   int a;
   
   if     ( c == '+' ) a = 3000;
   else if( c == '-' ) a = 3100;
   else if( c == '*' ) a = 3300;
   else if( c == '/' ) a = 3200;
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

void stampaFlags( int flags[100] )
{
   int i;
   printf("flags:  ");
   for( i = 0; i < 100; ++i )
      if( flags[i] != -1 )
         printf("%.2d  ", flags[i]);
   printf("\n");
}
