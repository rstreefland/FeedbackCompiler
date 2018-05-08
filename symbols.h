#ifndef SYMBOLS_H
#define SYMBOLS_H

/* capacity of symbol table */
#define SYMBOLTABLE_INIT_CAPACITY 100

/* Symbol data structure */
typedef struct symbol {
	float value;  		/* constant value of variable */
	char *identifier;	/* identifier of the variable */
} symbol;

/* Symbol table */
typedef struct symbolTable {
    symbol *items;  	/* symbols */
    int capacity;		/* capacity of symbol table */
    int total;			/* number of variables in symbol table */
} symbolTable;

/* function prototypes */
void symbolTable_init(symbolTable *);
int symbolTable_total(symbolTable *);
void symbolTable_add(symbolTable *,  symbol item);
void symbolTable_set(symbolTable *, int, symbol item);
symbol *symbolTable_get(symbolTable *, int);

#endif
