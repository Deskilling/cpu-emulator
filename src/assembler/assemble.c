#include "assembler.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char* pad_hex(const char* val) {
	static char buf[3] = {0};
	if (strlen(val) == 1) {
		buf[0] = '0';
		buf[1] = val[0];
		buf[2] = '\0';
		return buf;
	}
	return (char*)val;
}

void build_rr(char* hexStr, s_token* dest, s_token* src, char opcode) {
	hexStr[0] = opcode;
	hexStr[1] = dest->value[1];
	hexStr[2] = src->value[1];
	hexStr[3] = '0';
	hexStr[4] = '\0';
}

void build_rm(char* hexStr, s_token* dest, s_token* src, char opcode) {
	hexStr[0] = opcode;
	hexStr[1] = dest->value[1];
	char* padded = pad_hex(src->value);
	hexStr[2] = padded[0];
	hexStr[3] = padded[1];
	hexStr[4] = '\0';
}

void build_mr_reverse(char* hexStr, s_token* dest, s_token* src, char opcode) {
	hexStr[0] = opcode;
	hexStr[1] = src->value[1];
	char* padded = pad_hex(dest->value);
	hexStr[2] = padded[0];
	hexStr[3] = padded[1];
	hexStr[4] = '\0';
}

void build_mr(char* hexStr, s_token* dest, s_token* src, char opcode) {
	hexStr[0] = opcode;
	char* padded = pad_hex(dest->value);
	hexStr[1] = padded[0];
	hexStr[2] = padded[1];
	hexStr[3] = src->value[1];
	hexStr[4] = '\0';
}

void build_ri(char* hexStr, s_token* dest, s_token* src, char opcode) {
	hexStr[0] = opcode;
	hexStr[1] = dest->value[1];
	int value = atoi(src->value);
	uint8_t byte_val = (uint8_t)value;
	char hex[3];
	sprintf(hex, "%02X", byte_val);
	hexStr[2] = hex[0];
	hexStr[3] = hex[1];
	hexStr[4] = '\0';
}

void assemble(s_token* tokens, const char* output_file) {
	FILE* out = fopen(output_file, "w");
	if (!out) {
		fprintf(stderr, "Failed to open output\n");
		exit(-1);
	}

	static const s_InstructionPattern patterns[] = {
	    {.name = "MOV", .opcode = '0', .builder = build_rm, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_MEMORY},
	    {.name = "MOV", .opcode = '1', .builder = build_mr_reverse, .dest_type = TOKEN_MEMORY, .src_type = TOKEN_REGISTER},
	    {.name = "MOV", .opcode = '2', .builder = build_mr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_MEMORY_REGISTER},
	    {.name = "MOV", .opcode = '3', .builder = build_mr, .dest_type = TOKEN_MEMORY_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "MOV", .opcode = '4', .builder = build_ri, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_LITERAL},
	    {.name = "MOV", .opcode = '5', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "ADD", .opcode = '6', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "SUB", .opcode = '7', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "MUL", .opcode = '8', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "DIV", .opcode = '9', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "AND", .opcode = 'A', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "OR", .opcode = 'B', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "JZ", .opcode = 'C', .builder = build_ri, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_LITERAL},
	    {.name = "CMP", .opcode = 'D', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "LESS", .opcode = 'E', .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "NOP", .opcode = 'F', .builder = NULL, .dest_type = 0, .src_type = 0}};

	for (s_token* t = tokens; t != NULL; t = t->next) {
		// TODO gerade nur instruktions halt, aber rest gommt später
		if (t->type != TOKEN_INSTRUCTION) {
			continue;
		}

		s_token* dest = t->next;
		s_token* src = dest ? dest->next : NULL;

		if (!dest || !src) {
			fprintf(stderr, "Incomplete: %s\n", t->value);
			continue;
		}

		int matched = 0;
		for (int i = 0; patterns[i].name; i++) {
			if (strcmp(t->value, patterns[i].name) == 0 && dest->type == patterns[i].dest_type && src->type == patterns[i].src_type) {
				char hexStr[5];
				patterns[i].builder(hexStr, dest, src, patterns[i].opcode);
				fprintf(out, "%s\n", hexStr);
				matched = 1;
				break;
			}
		}

		if (!matched) {
			fprintf(stderr, "Invalid instruction: %s %s %s\n", t->value, dest->value, src->value);
		}
	}

	fclose(out);
}
