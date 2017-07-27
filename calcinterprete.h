/* trasforma notazione infissa ( tradizionale ) in notazione polacca inversa */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stackNode {
   char data;
   struct stackNode *nextPtr;
   };

typedef struct stackNode StackNode;
typedef StackNode *StackNodePtr;

void push( StackNodePtr *topPtr, char info );
char pop( StackNodePtr *topPtr );
char stackTop( StackNodePtr topPtr );
int isEmpty( StackNodePtr topPtr );
void printStack ( StackNodePtr currentPtr );
int isOperator( char c );
int isNumber( char c );
int livello( char c );
char * convertToPostfix( char infix[], char postfix[] ); 
int precedence( char operator1, char operator2 );      //fatto ma può restituire anche -2 e 2 
int evalutatePostfixExpression( char *expr );
int calculate( int op1, int op2, char operatore );
char *leggicalc( char * stringa );
char *leggicomp( FILE * cfPtr, char * stringa );

void push( StackNodePtr *topPtr, char info )
{
   StackNodePtr newPtr;
   
   newPtr = malloc( sizeof( StackNode ));
   
   if( newPtr != NULL ){
      newPtr->data = info;
      newPtr->nextPtr = *topPtr;
      *topPtr = newPtr;
   }
   else {
      printf("%d not inserted. No memory avaiable.\n", info );
	  }
}

char pop( StackNodePtr *topPtr )
{
   StackNodePtr tempPtr;
   char popValue;
   
   tempPtr = *topPtr;
   popValue = (*topPtr)->data;
   *topPtr = (*topPtr)->nextPtr;
   free( tempPtr );
   
   return popValue;
}

char stackTop( StackNodePtr topPtr )
{
   return  topPtr->data;
}

int isEmpty( StackNodePtr topPtr )
{
   return topPtr == NULL;
   }

void printStack ( StackNodePtr currentPtr )
{
   if( currentPtr == NULL ){
      printf("The stack is empty.\n\n");
	  }
   else {
      printf("The stack is:\n");
	  while( currentPtr != NULL ){
	     printf("%d --> ", currentPtr->data);
		 currentPtr = currentPtr->nextPtr;
		 }
	}
}

int isOperator( char c )
{
   return ( c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' );
}

int isNumber( char c )
{
   return c >= '0' && c <= '9';
}

int isVariabile( char c )
{
   return c >= 'a' && c <= 'z';
}

int livello( char c )
{
   int a;
   if( c == '+' || c == '-' )                  a = 0;
   else if( c == '*' || c == '/' || c == '%')  a = 1;
   else if( c == '^' )                         a = 2;
   return a;
}

char * convertToPostfix( char infix[], char postfix[] )
{
   StackNodePtr stackPtr = NULL;
   char *tokenPtr;
   char tempPtr[10];

   postfix[0] = '\0';
 
   push( &stackPtr, '(');
      
   strcat( infix, " )" );

   tokenPtr = strtok( infix, " " );
   while( !isEmpty( stackPtr ) && tokenPtr != NULL ){
      if( isNumber( *tokenPtr ) || isVariabile ( *tokenPtr ) ) { 
         strcat( postfix, tokenPtr );
         strcat( postfix, " " );
         }
      else if( *tokenPtr == '(' ) 
         push( &stackPtr, *tokenPtr );
      else if( isOperator( *tokenPtr )){
         while( precedence( stackPtr->data, *tokenPtr ) >= 0 && isOperator( stackPtr->data ) ) {
            tempPtr[0] = pop( &stackPtr );
            tempPtr[1] = '\0';
            strcat( postfix, tempPtr );
            strcat( postfix, " " );
            }
         push( &stackPtr, *tokenPtr );
         }
      else if( *tokenPtr == ')' ){
         while( stackPtr->data != '(' && isOperator( stackPtr->data ) ) {
            tempPtr[0] = pop( &stackPtr );
            tempPtr[1] = '\0';
            strcat( postfix, tempPtr );
            strcat( postfix, " " );
            }
         pop( &stackPtr );                            /*eliminazione '(' */
         }
      tokenPtr = strtok( NULL , " " );
      }

   return postfix;
}

int precedence( char op1, char op2 )
{
   return livello( op1 ) - livello( op2 );
}

char *leggicomp( FILE * cfPtr, char * stringa )
{
   char tokenPtr[10] = { 0 };
   fscanf( cfPtr, "%s", tokenPtr );
   fscanf( cfPtr, "%s", tokenPtr );
   while( *tokenPtr != ';' ) {
      strcat( stringa, tokenPtr );
      strcat( stringa, " " );
      fscanf( cfPtr, "%s", tokenPtr );
      }
   return stringa;
}
