#ifndef PARSER_H
#define PARSER_H

void P();
void S();
void Spd();
void Slr();
void M();
void C();
void Clr();
void A();
void Alr();
void R();
void Rpd();
void E();
void Elr();
void T();
void Tlr();
void F();

extern int nextToken;
extern char lexeme [100];
extern int lineCount;
#endif
