/*
===================================================================================
Title           : parser.c
Description     : a lexical analyzer system for the cooke programming language
Author          : var28790 (R#11998328)
Date            : 04/03/2026
Version         : 1.0
Usage           : Compile and run this program using the GNU C compiler
Notes           : This program has no requirements
C Version       : TODO
===================================================================================
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "front.h"
#include "parser.h"

/* Global Variable */
int nextToken;
char lexeme [100];
int lineCount = 1;

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
    printf("Cooke Analyzer :: R11998328\n\n");
    
    /* Open the input data file and process its contents */
    if (argv[1] == NULL) {
        printf("ERROR - file wasn't provided\n");
        exit(2);
    } else if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - file %s does not exist \n", argv[1]);
        return 3;
    } else {
        getChar();
        
        lex();
        P();
    }

    printf("Syntax Validated");
    exit(0);
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
            printf("Error encounter on line %d: The next lexeme was %s and the next token was UNKNOWN\n", lineCount, lexeme);
            exit(1);
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
    else {
        printf("Error - lexeme is too long \n");
        exit(1);
    }
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
            break;
    } /* End of switch */

    return nextToken;
} /* End of function lex */
