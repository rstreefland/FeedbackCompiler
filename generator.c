#include <stdio.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "grammar.tab.h"
#include "symbols.h"

float total = 0;
int i;
symbol temp;

int generateCode(node *n);
int generateWithoutOptimisation(node *n);
float optimise(node *n);

int generateCode(node *n) {

	/* if node is empty - do nothing */
	if (!n)
		return 0;

	/* do something different based on the type of node */
	switch (n->type) {

	/* if node is a constant */
	case typeConstant:
		total = n->constant.floatValue;
		fprintf(output, "%g", n->constant.floatValue); /* print float */
		break;

		/* if node is an identifier */
	case typeIdentifier:
		fprintf(output, "%s", n->identifier.value); /* print string representing the identifier */
		break;

		/* if node is an operator */
	case typeOperator:
		/* do something different based on the operator */
		switch (n->operator.operator) {

		/* WHILE Loop */
		case WHILE:
			fprintf(output, "\n\twhile (");
			generateWithoutOptimisation(n->operator.op[0]); /* expression */
			fprintf(output, ") {\n");
			generateWithoutOptimisation(n->operator.op[1]); /* statements */
			fprintf(output, "\t}\n");
			break;

			/* DO WHILE loop */
		case DO:
			fprintf(output, "\n\tdo {\n");
			generateWithoutOptimisation(n->operator.op[1]); /* statements */
			fprintf(output, "\t} while (");
			generateWithoutOptimisation(n->operator.op[0]); /* expression */
			fprintf(output, ");\n");
			break;

			/* IF statements */
		case IF:
			/* IF ELSE STATEMENT */
			if (n->operator.nOperands > 2) {
				fprintf(output, "\tif (");
				generateCode(n->operator.op[0]); /* expression */

				fprintf(output, ") {\n");
				generateCode(n->operator.op[1]); /* statements */

				fprintf(output, "\t} else {\n");
				generateCode(n->operator.op[2]); /* statements */

				/* IF STATEMENT */
			} else {
				fprintf(output, "\n\tif (");
				generateCode(n->operator.op[0]); /* expression */
				fprintf(output, ") {\n");
				generateCode(n->operator.op[1]); /* statements */
			}
			fprintf(output, "\t}\n\n");
			break;

			/* PRINT statement (representing numbers) */
		case PRINT:
			fprintf(output, "\tcout << %g << endl;\n", optimise(n->operator.op[0]));
			break;

			/* PRINT statement (representing strings) */
		case PRINTSTR:
			fprintf(output, "\tcout << \"%s\" << endl;\n",
					n->operator.op[0]->identifier.value);
			break;

			/* LET statement */
		case LET:
			fprintf(output, "\tfloat %s = ",
					n->operator.op[0]->identifier.value);
			generateCode(n->operator.op[1]); /* operand */
			fprintf(output, ";\n");
			break;

			/* assignment operator */
		case '=':
			fprintf(output, "\t%s = ", n->operator.op[0]->identifier.value);
			generateCode(n->operator.op[1]); /* operand */

			temp = *symbolTable_get(&s, i);
			temp.value = total;
			symbolTable_set(&s, i, temp);

			fprintf(output, ";\n");
			break;

			/* uminus (-) operator */
		case UMINUS:

			/* Don't allow minus zero (-0) */
			if (n->operator.op[0]->constant.floatValue != 0) {
				fprintf(output, "-");
			}
			generateCode(n->operator.op[0]); /* operand */
			break;

			/* operators */
		default:
			switch (n->operator.operator) {
			case ';':
				generateCode(n->operator.op[0]);
				generateCode(n->operator.op[1]);
				break;
			case '+':
				total = optimise(n->operator.op[0])
						+ optimise(n->operator.op[1]);

				/* use cmath.h definitions if total evaluates to nan, inf, or -inf */
				/* else print the value */
				if (total == -INFINITY) {
					fprintf(output, "-INFINITY");
				} else if (total != total) {
					fprintf(output, "NAN");
				} else if (total == INFINITY) {
					fprintf(output, "INFINITY");
				} else {
					fprintf(output, "%g", total);
				}
				break;
			case '-':
				total = optimise(n->operator.op[0])
						- optimise(n->operator.op[1]);

				/* use cmath.h definitions if total evaluates to nan, inf, or -inf */
				/* else print the value */
				if (total == -INFINITY) {
					fprintf(output, "-INFINITY");
				} else if (total != total) {
					fprintf(output, "NAN");
				} else if (total == INFINITY) {
					fprintf(output, "INFINITY");
				} else {
					fprintf(output, "%g", total);
				}
				break;
			case '*':
				total = optimise(n->operator.op[0])
						* optimise(n->operator.op[1]);

				/* use cmath.h definitions if total evaluates to nan, inf, or -inf */
				/* else print the value */
				if (total == -INFINITY) {
					fprintf(output, "-INFINITY");
				} else if (total != total) {
					fprintf(output, "NAN");
				} else if (total == INFINITY) {
					fprintf(output, "INFINITY");
				} else {
					fprintf(output, "%g", total);
				}
				break;
			case '/':
				total = optimise(n->operator.op[0])
						/ optimise(n->operator.op[1]);

				/* use cmath.h definitions if total evaluates to nan, inf, or -inf */
				/* else print the value */
				if (total == -INFINITY) {
					fprintf(output, "-INFINITY");
				} else if (total != total) {
					fprintf(output, "NAN");
				} else if (total == INFINITY) {
					fprintf(output, "INFINITY");
				} else {
					fprintf(output, "%g", total);
				}
				break;
			case '<':
				generateCode(n->operator.op[0]); /* first operand */
				fprintf(output, " < ");
				generateCode(n->operator.op[1]); /* second operand */
				break;
			case '>':
				generateCode(n->operator.op[0]); /* first operand */
				fprintf(output, " > ");
				generateCode(n->operator.op[1]); /* second operand */
				break;
			case GE:
				generateCode(n->operator.op[0]); /* first operand */
				fprintf(output, " >= ");
				generateCode(n->operator.op[1]); /* second operand */
				break;
			case LE:
				generateCode(n->operator.op[0]); /* first operand */
				fprintf(output, " <= ");
				generateCode(n->operator.op[1]); /* second operand */
				break;
			case NE:
				generateCode(n->operator.op[0]); /* first operand */
				fprintf(output, " != ");
				generateCode(n->operator.op[1]); /* second operand */
				break;
			case EQ:
				generateCode(n->operator.op[0]); /* first operand */
				fprintf(output, " == ");
				generateCode(n->operator.op[1]); /* second operand */
				break;
			}
		}
	}
	return 0;
}

int generateWithoutOptimisation(node *n) {

	/* if node is empty - do nothing */
	if (!n)
		return 0;

	/* do something different based on the type of node */
	switch (n->type) {

	/* if node is a constant */
	case typeConstant:
		total = n->constant.floatValue;
		fprintf(output, "%g", n->constant.floatValue); /* print float */
		break;

		/* if node is an identifier */
	case typeIdentifier:
		fprintf(output, "%s", n->identifier.value); /* print string representing the identifier */
		break;

		/* if node is an operator */
	case typeOperator:
		/* do something different based on the operator */
		switch (n->operator.operator) {

		/* WHILE Loop */
		case WHILE:
			fprintf(output, "\n\twhile (");
			generateWithoutOptimisation(n->operator.op[0]); /* expression */
			fprintf(output, ") {\n");
			generateWithoutOptimisation(n->operator.op[1]); /* statements */
			fprintf(output, "\t}\n");
			break;

			/* DO WHILE loop */
		case DO:
			fprintf(output, "\n\tdo {\n");
			generateWithoutOptimisation(n->operator.op[1]); /* statements */
			fprintf(output, "\t} while (");
			generateWithoutOptimisation(n->operator.op[0]); /* expression */
			fprintf(output, ");\n");
			break;

			/* IF statements */
		case IF:
			/* IF ELSE STATEMENT */
			if (n->operator.nOperands > 2) {
				fprintf(output, "\tif (");
				generateWithoutOptimisation(n->operator.op[0]); /* expression */

				fprintf(output, ") {\n");
				generateWithoutOptimisation(n->operator.op[1]); /* statements */

				fprintf(output, "\t} else {\n");
				generateWithoutOptimisation(n->operator.op[2]); /* statements */

				/* IF STATEMENT */
			} else {
				fprintf(output, "\n\tif (");
				generateWithoutOptimisation(n->operator.op[0]); /* expression */
				fprintf(output, ") {\n");
				generateWithoutOptimisation(n->operator.op[1]); /* statements */
			}
			fprintf(output, "\t}\n\n");
			break;

			/* PRINT statement (representing numbers) */
		case PRINT:
			fprintf(output, "\tcout << %s << endl;\n",
					n->operator.op[0]->identifier.value);
			break;

			/* PRINT statement (representing strings) */
		case PRINTSTR:
			fprintf(output, "\tcout << \"%s\" << endl;\n",
					n->operator.op[0]->identifier.value);
			break;

			/* LET statement */
		case LET:
			fprintf(output, "\tfloat %s = ",
					n->operator.op[0]->identifier.value);
			generateWithoutOptimisation(n->operator.op[1]); /* operand */
			fprintf(output, ";\n");
			break;

			/* assignment operator */
		case '=':
			fprintf(output, "\t%s = ", n->operator.op[0]->identifier.value);
			generateWithoutOptimisation(n->operator.op[1]); /* operand */

			temp = *symbolTable_get(&s, i);
			temp.value = total;
			symbolTable_set(&s, i, temp);

			fprintf(output, ";\n");
			break;

			/* uminus (-) operator */
		case UMINUS:

			/* Don't allow minus zero (-0) */
			if (n->operator.op[0]->constant.floatValue != 0) {
				fprintf(output, "-");
			}
			generateWithoutOptimisation(n->operator.op[0]); /* operand */
			break;

			/* operators */
		default:
			switch (n->operator.operator) {
			case ';':
				generateWithoutOptimisation(n->operator.op[0]);
				generateWithoutOptimisation(n->operator.op[1]);
				break;
			case '+':
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " + ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case '-':
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " - ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case '*':
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " * ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case '/':
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " / ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case '<':
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " < ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case '>':
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " > ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case GE:
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " >= ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case LE:
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " <= ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case NE:
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " != ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			case EQ:
				generateWithoutOptimisation(n->operator.op[0]); /* first operand */
				fprintf(output, " == ");
				generateWithoutOptimisation(n->operator.op[1]); /* second operand */
				break;
			}
		}
	}
	return 0;
}

float optimise(node *n) {
	if (!n)
		return 0;
	switch (n->type) {
	case typeConstant:
		return n->constant.floatValue;
	case typeIdentifier:
		for (i = 0; i < symbolTable_total(&s); i++) {
			temp = *symbolTable_get(&s, i);
			if (strcmp(n->identifier.value, temp.identifier) == 0) {
				return temp.value;
			}
		}
		return 0;
		break;
	case typeOperator:
		switch (n->operator.operator) {
		case UMINUS:
			return -optimise(n->operator.op[0]);
		case '+':
			return optimise(n->operator.op[0]) + optimise(n->operator.op[1]);
		case '-':
			return optimise(n->operator.op[0]) - optimise(n->operator.op[1]);
		case '*':
			return optimise(n->operator.op[0]) * optimise(n->operator.op[1]);
		case '/':
			return optimise(n->operator.op[0]) / optimise(n->operator.op[1]);
		}
	}
	return 0;
}
