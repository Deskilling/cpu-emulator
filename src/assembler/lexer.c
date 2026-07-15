#include "assembler.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 16

static const char* instructions[] = {"MOV", "ADD", "SUB", "MUL", "DIV", "AND", "OR", "JZ", "CMP", "LESS", "NOP", NULL};

e_TokenType convert_token(char* buffer) {
	size_t len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == ':') {
		return TOKEN_LABEL;
	}

	for (int i = 0; instructions[i] != NULL; i++) {
		if (strcmp(buffer, instructions[i]) == 0) {
			return TOKEN_INSTRUCTION;
		}
	}

	if (buffer[0] == '#' || isxdigit(buffer[0]) || buffer[0] == '-') {
		if (buffer[0] == '#') {
			memmove(buffer, buffer + 1, len);
		}
		return TOKEN_IMMEDIATE;
	}

	if ((buffer[0] == 'R' && len > 1 && isxdigit(buffer[1])) || (len == 1 && (buffer[0] == 'A' || buffer[0] == 'B'))) {
		return TOKEN_REGISTER;
	}

	if (buffer[0] == 'M' && buffer[1] >= '0' && buffer[1] <= 'F') {
		memmove(buffer, buffer + 1, len);
		return TOKEN_MEMORY;
	}

	return TOKEN_EOF;
}

s_token* create_token(e_TokenType type, char* value) {
	s_token* tok = malloc(sizeof(s_token));
	if (!tok) {
		fprintf(stderr, "malloc failed\n");
		exit(-1);
	}
	tok->type = type;
	tok->value = value;
	tok->next = NULL;
	return tok;
}

s_token* lexer(FILE* file) {
	s_token* head = NULL;
	s_token* tail = NULL;

	char buffer[BUFFER] = {0};
	uint16_t bufferIdx = 0;

	char c;
	while ((c = fgetc(file)) != EOF) {
		if (c == ';') {
			while ((c = fgetc(file)) != '\n' && c != EOF);
		}
		if (isspace(c) || c == ',') {
			if (bufferIdx > 0) {
				buffer[bufferIdx] = '\0';
				e_TokenType type = convert_token(buffer);
				s_token* tok = create_token(type, strdup(buffer));

				if (!head)
					head = tail = tok;
				else {
					tail->next = tok;
					tail = tok;
				}
				bufferIdx = 0;
			}
		} else {
			if (bufferIdx >= BUFFER - 1) {
				fprintf(stderr, "Token too long\n");
				exit(-1);
			}
			buffer[bufferIdx++] = c;
		}
	}

	print_tokens(head);

	return head;
}

const char* token_type_to_string(e_TokenType type) {
	switch (type) {
	case TOKEN_INSTRUCTION:
		return "INSTRUCTION";
	case TOKEN_REGISTER:
		return "REGISTER";
	case TOKEN_MEMORY:
		return "MEMORY";
	case TOKEN_IMMEDIATE:
		return "IMMEDIATE";
	case TOKEN_LABEL:
		return "LABEL";
	case TOKEN_EOF:
		return "EOF";
	default:
		return "UNKNOWN";
	}
}

void print_tokens(s_token* head) {
	for (s_token* t = head; t != NULL; t = t->next) {
		printf("%s: %s\n", token_type_to_string(t->type), t->value);
	}
}
