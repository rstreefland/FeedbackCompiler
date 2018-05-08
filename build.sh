rm -rf lex.yy.c grammar.tab.c grammar.tab.h feedbc

flex lexer.l
bison -d grammar.y
cc lex.yy.c grammar.tab.c main.c generator.c symbols.c -o feedbc
