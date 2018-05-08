%{
#include <stdlib.h>
#include "includes.h"
#include "grammar.tab.h"

/* Variables to store data for better syntax error reporting */
int lineNumber = 0;
char *errorDescription = "error";
char *currentStatement = "";

%}

%%
 
 /* ------ Operators ------ */
">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;

 /* ------ Statements ------ */
"LET"		{currentStatement = "LET"; return LET; }
"DO" 		{currentStatement = "DO"; return DO; }
"WHILE"		{currentStatement = "WHILE"; return WHILE; }
"WEND"		{currentStatement = "WEND"; return WEND; }
"IF"		{currentStatement = "IF"; return IF; }
"THEN"		{currentStatement = "THEN"; return THEN; }
"ENDIF"		{currentStatement = "ENDIF"; return ENDIF; }
"ELSE"		{currentStatement = "ELSE"; return ELSE; }
"PRINT"		{currentStatement = "PRINT"; return PRINT; }

 /* ------- String ------- */
\"[^"\n]*["\n]		{ 
						yylval.stringValue = malloc(sizeof(char) * (strlen(yytext)+1)); 
						yylval.stringValue =  strdup(yytext+1);
					
						if (yylval.stringValue[yyleng-2] != '"') {
							errorDescription = "improperly terminated string";
						} else {
							yylval.stringValue[yyleng-2] =  0;
							return STRING; 
						}
					}

 /* ------ Variable ------ */
[a-z][a-z0-9]*      { 
						yylval.stringValue = malloc(sizeof(char) * (strlen(yytext)+1)); 
						yylval.stringValue = strdup(yytext);
               			return VARIABLE;
            		}
            			
 /* ------ Integer ------- */	
0           		{
                		yylval.floatValue = atoi(yytext);
                		return INTEGER;
            		}

[1-9][0-9]* 		{
               			yylval.floatValue = atoi(yytext);
                		return INTEGER;
            		}
            
 /* ------ Float ------ */
[0-9]+\.[0-9]+  	{
           		 		yylval.floatValue = atof(yytext);
            	 		return FLOAT;   
                	}

 /* ------ Other Symbols ------ */
[-()<>=+*/;{}.] 	{
                		return *yytext;
             		}

 /* ------ Comments, new line, and tab characters ------ */
[REM]+.*			return COMMENT;
[\n]				{ lineNumber++; return END; }
[ \t]+        		;       /* ignore whitespace */

.               	yyerror("unknown character");
%%
int yywrap(void) {
    return 1;
}
