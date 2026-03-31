/**
 * This the example lexical analyzer code in pages 173 - 177 of the
 * textbook,
 *
 * Sebesta, R. W. (2012). Concepts of Programming Languages. 
 * Pearson, 10th edition.
 *
 */

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;
char lexeme [100];
int lineCount = 0;

/* Local Variables */
static int charClass;
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char *argv[])
{
    /* Open the input data file and process its contents */
    if (argv[1] == NULL) {
        printf("ERROR - file wasn't provided\n");
        return 2;
    } else if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - cannot open %s \n", argv[1]);
        return 3;
    } else {
        printf("Cooke Analyzer :: R11998328\n\n");
        getChar();
        do {
            lex();

            if (nextToken == UNKNOWN)
                return 1;

            P();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and return the 
 * token */
static int lookup(char ch) {
    switch (ch) {
		// Assignment and colon tokens
        case ':':
            addChar();
            getChar();
            if (nextChar == '=') {
				addChar();
                getChar();
                nextToken = ASSIGNMENT_OP;
            } else
                nextToken = COLON;
            break;

		// Relational and shift operator tokens
        case '<':
            addChar();
            getChar();
            if (nextChar == '>') {
                addChar();
                getChar();
                nextToken = NEQUAL_OP;
            }
            else if (nextChar == '=') {
                addChar();
                getChar();
                nextToken = LEQUAL_OP;
            }
            else if (nextChar == '<') {
                addChar();
                getChar();
                nextToken = SHIFT_L_OP;
			}
            else nextToken = LESSER_OP;
            break;

		// Relational and shift operator tokens
        case '>':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                getChar();
                nextToken = GEQUAL_OP;
            }
            else if (nextChar == '>') {
                addChar();
                getChar();
                nextToken = SHIFT_R_OP;
            }
            else nextToken = GREATER_OP;
            break;

		// Equality operator token
        case '=':
            addChar();
            getChar();
            if (nextChar == '=') {
                addChar();
                getChar();
                nextToken = EQUAL_OP;
            }
            else nextToken = UNKNOWN;
            break;

			// Semicolon and parentheses tokens
        case ';':
            addChar();
            getChar();
            nextToken = SEMICOLON;
            break;
        case '(':
            addChar();
            getChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            getChar();
            nextToken = RIGHT_PAREN;
            break;

		// Increment and addition operator tokens
        case '+':
            addChar();
            getChar();
            if (nextChar == '+') {
                addChar();
                getChar();
                nextToken = INC_OP;
            }
            else nextToken = ADD_OP;
            break;

        // Decrement and subtraction operator tokens
        case '-':
            addChar();
            getChar();
            if (nextChar == '-') {
                addChar();
                getChar();
                nextToken = DEC_OP;
            }
            else nextToken = SUB_OP;
            break;

        // Multiplication, division and modulo operator tokens
        case '*':
            addChar();
            getChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            getChar();
            nextToken = DIV_OP;
            break;
        case '%':
            addChar();
            getChar();
            nextToken = MOD_OP;
            break;

		// Unknown character token
        default:
            addChar();
            getChar();
            nextToken = UNKNOWN;
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else printf("Error - lexeme is too long \n");
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar) || nextChar == '_')
            charClass = LETTER;

        else if (isdigit(nextChar))
            charClass = DIGIT;

        else if (nextChar == '.')
            charClass = DOT;

        else charClass = UNKNOWN;
    }
    else charClass = EOF;
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) {
        if (nextChar == '\n')
            lineCount += 1;

        getChar();
    }
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();

    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }

			// Reserved keywords and boolean operator token check
            if (strcmp(lexeme, "or") == 0)
                nextToken = BOOLEAN_OR;

            else if (strcmp(lexeme, "and") == 0)
                nextToken = BOOLEAN_AND;

            else if (strcmp(lexeme, "not") == 0)
                nextToken = BOOLEAN_NOT;

            else if (strcmp(lexeme, "if") == 0)
                nextToken = KEY_IF;

            else if (strcmp(lexeme, "elif") == 0)
                nextToken = KEY_ELIF;

            else if (strcmp(lexeme, "else") == 0)
                nextToken = KEY_ELSE;

            else if (strcmp(lexeme, "for") == 0)
                nextToken = KEY_FOR;

            else if (strcmp(lexeme, "in") == 0)
                nextToken = KEY_IN;

            else if (strcmp(lexeme, "cin") == 0)
                nextToken = KEY_CIN;

            else if (strcmp(lexeme, "cout") == 0)
                nextToken = KEY_COUT;
			
            // Identifier token
            else nextToken = IDENT;
            break;

        /* Parse range operator and float literals */
        case DOT:
            addChar();
            getChar();
			if (nextChar == '.') {
                addChar();
                getChar();
				nextToken = RANGE_OP;
            } else {
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }

                nextToken = FLOAT_LITERAL;
            }
            break;

        /* Parse integer and float literals*/
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }

            if (charClass == DOT) {
		        addChar();
		        getChar();
                if (charClass == DOT) {
		            fseek(in_fp, -1, SEEK_CUR);
                    lexeme[--lexLen] = 0;
		            nextToken = INT_LITERAL;
                }
                else {
                    while (charClass == DIGIT) {
                        addChar();
                        getChar();
				    }

                    nextToken = FLOAT_LITERAL;
                }
            }
            else nextToken = INT_LITERAL;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
			return nextToken;
    } /* End of switch */

	// Token type check and print
    switch (nextToken) {
        case ASSIGNMENT_OP:
            printf("%s\tASSIGNMENT_OP\n", lexeme);
			break;
        case BOOLEAN_OR:
            printf("%s\tBOOLEAN_OR\n", lexeme);
            break;
        case BOOLEAN_AND:
            printf("%s\tBOOLEAN_AND\n", lexeme);
            break;
        case BOOLEAN_NOT:
            printf("%s\tBOOLEAN_NOT\n", lexeme);
            break;
        case LESSER_OP:
            printf("%s\tLESSER_OP\n", lexeme);
            break;
        case GREATER_OP:
            printf("%s\tGREATER_OP\n", lexeme);
            break;
        case EQUAL_OP:
            printf("%s\tEQUAL_OP\n", lexeme);
            break;
        case NEQUAL_OP:
            printf("%s\tNEQUAL_OP\n", lexeme);
            break;
        case LEQUAL_OP:
            printf("%s\tLEQUAL_OP\n", lexeme);
            break;
        case GEQUAL_OP:
            printf("%s\tGEQUAL_OP\n", lexeme);
            break;
        case SEMICOLON:
            printf("%s\tSEMICOLON\n", lexeme);
            break;
        case COLON:
            printf("%s\tCOLON\n", lexeme);
            break;
        case LEFT_PAREN:
            printf("%s\tLEFT_PAREN\n", lexeme);
            break;
        case RIGHT_PAREN:
			printf("%s\tRIGHT_PAREN\n", lexeme);
            break;
		case KEY_IF:
            printf("%s\tKEY_IF\n", lexeme);
			break;
		case KEY_ELIF:
            printf("%s\tKEY_ELIF\n", lexeme);
			break;
		case KEY_ELSE:
            printf("%s\tKEY_ELSE\n", lexeme);
			break;
		case KEY_FOR:
            printf("%s\tKEY_FOR\n", lexeme);
			break;
		case KEY_IN:
            printf("%s\tKEY_IN\n", lexeme);
			break;
		case KEY_CIN:
            printf("%s\tKEY_CIN\n", lexeme);
			break;
		case KEY_COUT:
            printf("%s\tKEY_COUT\n", lexeme);
			break;
		case SHIFT_R_OP:
            printf("%s\tSHIFT_R_OP\n", lexeme);
			break;
		case SHIFT_L_OP:
            printf("%s\tSHIFT_L_OP\n", lexeme);
			break;
		case RANGE_OP:
            printf("%s\tRANGE_OP\n", lexeme);
			break;
		case ADD_OP:
            printf("%s\tADD_OP\n", lexeme);
			break;
		case SUB_OP:
            printf("%s\tSUB_OP\n", lexeme);
			break;
		case MULT_OP:
            printf("%s\tMULT_OP\n", lexeme);
			break;
		case DIV_OP:
            printf("%s\tDIV_OP\n", lexeme);
			break;
		case MOD_OP:
            printf("%s\tMOD_OP\n", lexeme);
			break;
		case INC_OP:
            printf("%s\tINC_OP\n", lexeme);
			break;
		case DEC_OP:
            printf("%s\tDEC_OP\n", lexeme);
			break;
        case IDENT:
            printf("%s\tIDENT\n", lexeme);
            break;
        case INT_LITERAL:
            printf("%s\tINT_LITERAL\n", lexeme);
            break;
        case FLOAT_LITERAL:
            printf("%s\tFLOAT_LITERAL\n", lexeme);
            break;
		case UNKNOWN:
        default:
			printf("Error encounter on line %d: The next lexeme was %s and the next token was UNKNOWN\n", lineCount);
            return 1;
    }
    return 0;
} /* End of function lex */
