#include <stdio.h>
#include <stdlib.h>
#include "symbols.h"

void symbolTable_init(symbolTable *s)
{
    s->capacity = SYMBOLTABLE_INIT_CAPACITY;  /* set capacity */
    s->total = 0;	/* set total symbols to 0 */
    s->items = malloc(sizeof(void *) * s->capacity); /* allocate memory for symbols */
}

int symbolTable_total(symbolTable *s)
{
    return s->total; /* return total number of symbols */
}

void symbolTable_add(symbolTable *s, symbol item)
{
    s->items[s->total++] = item; /* add new symbol to the table */
}

void symbolTable_set(symbolTable *s, int index, symbol item)
{
    if (index >= 0 && index < s->total)
        s->items[index] = item; /* update an existing symbol */
}

symbol *symbolTable_get(symbolTable *s, int index)
{
    if (index >= 0 && index < s->total)
        return &s->items[index];		/* get a symbol from the table */
    return NULL;
}
