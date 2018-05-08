%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "includes.h"
#include "symbols.h"

#define YYDEBUG 1

/* prototypes */
node *operator(int operator, int nOperands, ...);
node *value(char *i);
node *constantFloat(float value);
void freeMemory(node *n);
int generateCode(node *n);
int yylex(void);

extern int lineNumber;
extern char *errorDescription;
extern char *currentStatement;
extern char yytext[]; 

extern FILE *output;

int i;
symbol temp;

%}

%union {                
    float floatValue;
    char *stringValue;
    node *node;            
};

%token <floatValue> INTEGER
%token <floatValue> FLOAT
%token <stringValue> VARIABLE STRING
%token LET DO WHILE WEND IF THEN ENDIF PRINT END COMMENT PRINTSTR
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%type <node> statement expression statement_list

%%

program:
      	{ fprintf(output,"#include <iostream>\n#include <cmath>\nusing namespace std;\n\nint main(void) { \n\n");} 
        lines           { fprintf(output, "}\n"); fclose(output); exit(0); }
        ;

lines:
        lines statement         {generateCode($2); freeMemory($2); }
        | /* NULL */
        ;
        
statement:
         	END                            											{ $$ = operator(';', 2, NULL, NULL); }
       		| COMMENT					     										{ $$ = operator(';', 2, NULL, NULL); } 
        	| PRINT STRING END				 										{ $$ = operator(PRINTSTR, 1, value($2)); }	
        	| PRINT expression END                 									{ $$ = operator(PRINT, 1, $2); }
        	| LET VARIABLE '=' expression END      									{ $$ = operator(LET, 2, value($2), $4); temp.identifier = $2; temp.value = $4->constant.floatValue; symbolTable_add(&s, temp); }
        	| VARIABLE '=' expression END      	 									{ $$ = operator('=', 2, value($1), $3); }
        	| WHILE expression END statement_list WEND      						{ $$ = operator(WHILE, 2, $2, $4); }
        	| DO END statement_list WHILE expression END WEND     					{ $$ = operator(DO, 2, $5, $3); }
        	| IF expression THEN END statement_list %prec IFX ENDIF 				{ $$ = operator(IF, 2, $2, $5); }
        	| IF expression THEN END statement_list ELSE END statement_list ENDIF 	{ $$ = operator(IF, 3, $2, $5, $8); }
        	;

statement_list:
         	  	statement               	{ $$ = $1; }
       			| statement_list statement	{ $$ = operator(';', 2, $1, $2); }
        		;

expression:
     		INTEGER               				{ $$ = constantFloat($1); }
        	| FLOAT                 			{ $$ = constantFloat($1); }
        	| VARIABLE             				{ $$ = value($1);}
        	| '-' expression %prec UMINUS 		{ $$ = operator(UMINUS, 1, $2); }
        	| expression '+' expression         { $$ = operator('+', 2, $1, $3); }
        	| expression '-' expression         { $$ = operator('-', 2, $1, $3); }
        	| expression '*' expression         { $$ = operator('*', 2, $1, $3); }
        	| expression '/' expression         { $$ = operator('/', 2, $1, $3); }
        	| expression '<' expression         { $$ = operator('<', 2, $1, $3); }
        	| expression '>' expression         { $$ = operator('>', 2, $1, $3); }
        	| expression GE expression          { $$ = operator(GE, 2, $1, $3); }
        	| expression LE expression          { $$ = operator(LE, 2, $1, $3); }
        	| expression NE expression          { $$ = operator(NE, 2, $1, $3); }
        	| expression EQ expression          { $$ = operator(EQ, 2, $1, $3); }
        	| '(' expression ')'          		{ $$ = $2; }
        	;

%%

node *value(char *i) {
	
	/* create new node */
	node *n;

	/* allocate memory for node */
	if ((n = malloc(sizeof(node))) == NULL)
		yyerror("Feedback: out of memory");

	/* copy data into node */
	n->type = typeIdentifier;
	n->identifier.value = i;

	return n;
}

node *constantFloat(float value) {
	
	/* create new node */
	node *n;

	/* allocate memory for node */
	if ((n = malloc(sizeof(node))) == NULL)
		yyerror("out of memory");

	/* copy data into node */
	n->type = typeConstant;
	n->constant.floatValue = value;

	return n;
}

node *operator(int operator, int nOperands, ...) {
	
	va_list vargs; /* create variable argument list */
	node *n; /* create new node */
	int i;

	/* allocate memory for operator node */
	if ((n = malloc(sizeof(node))) == NULL)
		yyerror("out of memory");
		
	/* allocate memory for operand node */
	if ((n->operator.op = malloc(nOperands * sizeof(node *))) == NULL)
		yyerror("out of memory");

	/* copy data into node */
	n->type = typeOperator;
	n->operator.operator = operator;
	n->operator.nOperands = nOperands;
	
	/* copy data for each operand */
	va_start(vargs, nOperands);
	for (i = 0; i < nOperands; i++)
	n->operator.op[i] = va_arg(vargs, node*);
	va_end(vargs);
	return n;
}

void freeMemory(node *n) {
	int i;

	/* If node is empty - do nothing */
	if (!n)
		return;
		
	/* If the node is an operator node */
	if (n->type == typeOperator) {
		
		/* Free memory consumed by each operand stored in the operator node */
		for (i = 0; i < n->operator.nOperands; i++)
			freeMemory(n->operator.op[i]);
			
		/* Free memory consumed by operator */
		free(n->operator.op);
	}
	
	/* Free memory consumed by node */
	free(n);
}

void yyerror() {
	
	/* Print a useful error message when a syntax error ocurred  */
	fprintf(stderr, "Feedback Syntax error: %s on line %d after %s statement\n",
			errorDescription, lineNumber, currentStatement);
}