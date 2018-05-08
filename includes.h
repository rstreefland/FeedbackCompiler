#include "symbols.h"

/* enumerated type representing the node type */
typedef enum { typeConstant, typeIdentifier, typeOperator} nodeEnum;

/* identifiers */
typedef struct {
    char *value;		/* string representing the identifier */
} identifierNode;

/* constants */
typedef struct {
    float floatValue;	/* float value of the constant */
} constantNode;

/* operators */
typedef struct {
    int operator;               /* integer representing the operator */
    int nOperands;              /* number of operands */
    struct nodeStruct **op;		/* structure of operands */
} operatorNode;

typedef struct nodeStruct {
    nodeEnum type;              /* type of node */

    union {
        constantNode constant;        /* constants */
        identifierNode identifier;    /* identifiers */
        operatorNode operator;        /* operators */
    };
} node;

/* output file pointer */
FILE *output;

/* symbol table */
symbolTable s;

void yyerror(char *s);


