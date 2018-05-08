#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"
#include "symbols.h"

extern int yydebug;
extern int yyparse();
extern FILE *yyin;

int main(int argc, char *argv[]) {

	/* initialise the symbol table */
	symbolTable_init(&s);

	if (argc == 2) {
		/* open source file */
		FILE *source = fopen(argv[1], "r");

		/* if source file cannot be opened */
		if (!source) {
			printf("Feedback: Could not open source file\n");
		} else {

			/* write to the console */
			output = stdout;

			/* invoke compiler */
			yyin = source;
			yyparse();
			fclose(source);
		}
	} else if (argc == 3 && strcmp(argv[2], "-debug") != 0) {
		/* open source file */
		FILE *source = fopen(argv[1], "r");

		/* if source file cannot be opened */
		if (!source) {
			printf("Feedback: Could not open source file\n");
		} else {

			/* set output file */
			char fileName[sizeof(argv[2]) + 5];
			strcpy(fileName, argv[2]);
			strcat(fileName, ".cpp");

			/* write to output file */
			output = fopen(fileName, "w");
			if (!output) {
				fprintf(stderr, "Feedback: Could not write to target file\n");
				exit(1);
			}

			/* invoke compiler */
			yyin = source;
			yyparse();
			fclose(source);

		}
	} else if (argc == 4 && strcmp(argv[3], "-debug") == 0) {

		/* enable compiler debug mode */
		yydebug = 1;

		/* open source file */
		FILE *source = fopen(argv[1], "r");

		/* if source file cannot be opened */
		if (!source) {
			printf("Feedback: Could not open source file\n");
		} else {

			/* set output file */
			char fileName[sizeof(argv[2]) + 5];
			strcpy(fileName, argv[2]);
			strcat(fileName, ".cpp");

			/* write to output file */
			output = fopen(fileName, "w");
			if (!output) {
				fprintf(stderr, "Feedback: Could not write to target file\n");
				exit(1);
			}

			/* invoke compiler */
			yyin = source;
			yyparse();
			fclose(source);
		}

	} else {
		/* print compiler usage instruction */
		printf("\nUSAGE: ./feedbc <source> <output> \n");
	}
	return 0;
}
