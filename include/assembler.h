#ifndef PARSERS_H
#define PARSERS_H

#include <stdio.h>

typedef enum {
	TOKEN_INSTRUCTION = 0,
	TOKEN_REGISTER = 1,
	TOKEN_MEMORY = 2,
	TOKEN_IMMEDIATE = 3,
	TOKEN_LABEL = 4,
	TOKEN_EOF = 5,
} e_TokenType;

typedef struct {
	e_TokenType type;
	char* value;

	void* next;
} s_token;

typedef struct {
	const char* name;
	char opcode;
	void (*builder)(char*, s_token*, s_token*, char);
	e_TokenType dest_type;
	e_TokenType src_type;
} s_InstructionPattern;

void print_tokens(s_token* head);
const char* token_type_to_string(e_TokenType type);

s_token* lexer(FILE* file);
void assemble(s_token* tokens, const char* fileName);

#endif
