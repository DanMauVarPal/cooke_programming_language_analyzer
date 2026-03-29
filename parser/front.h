#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define DOT 2
#define UNKNOWN 99

/* Token codes */
#define ASSIGNMENT_OP 10 
#define BOOLEAN_OR 11
#define BOOLEAN_AND 12
#define BOOLEAN_NOT 13
#define LESSER_OP 14
#define GREATER_OP 15
#define EQUAL_OP 16
#define NEQUAL_OP 17
#define LEQUAL_OP 18
#define GEQUAL_OP 19
#define SEMICOLON 20
#define COLON 21
#define LEFT_PAREN 22
#define RIGHT_PAREN 23
#define KEY_IF 24
#define KEY_ELIF 25
#define KEY_ELSE 26
#define KEY_FOR 27
#define KEY_IN 28
#define KEY_CIN 29
#define KEY_COUT 30
#define SHIFT_R_OP 31
#define SHIFT_L_OP 32
#define RANGE_OP 33
#define ADD_OP 34
#define SUB_OP 35
#define MULT_OP 36
#define DIV_OP 37
#define MOD_OP 38
#define INC_OP 39
#define DEC_OP 40
#define IDENT 41
#define INT_LITERAL 42
#define FLOAT_LITERAL 43


int lex();

#endif
