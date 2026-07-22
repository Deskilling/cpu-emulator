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

void build_rr(char* hexStr, s_token* dest, s_token* src, char* opcode) {
	hexStr[0] = opcode[0];
	hexStr[1] = opcode[1];
	hexStr[2] = '0';
	hexStr[3] = dest->value[1];
	hexStr[4] = '0';
	hexStr[5] = src->value[1];
	hexStr[6] = '0';
	hexStr[7] = '0';
	hexStr[8] = '\0';
}

void build_rm(char* hexStr, s_token* dest, s_token* src, char* opcode) {
	hexStr[0] = opcode[0];
	hexStr[1] = opcode[1];
	hexStr[2] = '0';
	hexStr[3] = dest->value[1];
	char* padded = pad_hex(src->value);
	hexStr[4] = padded[0];
	hexStr[5] = padded[1];
	hexStr[6] = '0';
	hexStr[7] = '0';
	hexStr[8] = '\0';
}

void build_mr_reverse(char* hexStr, s_token* dest, s_token* src, char* opcode) {
	hexStr[0] = opcode[0];
	hexStr[1] = opcode[1];
	hexStr[2] = '0';
	hexStr[3] = src->value[1];
	char* padded = pad_hex(dest->value);
	hexStr[4] = padded[0];
	hexStr[5] = padded[1];
	hexStr[6] = '0';
	hexStr[7] = '0';
	hexStr[8] = '\0';
}

void build_mr(char* hexStr, s_token* dest, s_token* src, char* opcode) {
	hexStr[0] = opcode[0];
	hexStr[1] = opcode[1];
	char* padded = pad_hex(dest->value);
	hexStr[2] = padded[0];
	hexStr[3] = padded[1];
	hexStr[4] = '0';
	hexStr[5] = src->value[1];
	hexStr[6] = '0';
	hexStr[7] = '0';
	hexStr[8] = '\0';
}

void build_ri(char* hexStr, s_token* dest, s_token* src, char* opcode) {
	hexStr[0] = opcode[0];
	hexStr[1] = opcode[1];
	hexStr[2] = '0';
	hexStr[3] = dest->value[1];
	int value = atoi(src->value);
	uint8_t byte_val = (uint8_t)value;
	char hex[3];
	sprintf(hex, "%02X", byte_val);
	hexStr[4] = hex[0];
	hexStr[5] = hex[1];
	hexStr[6] = '0';
	hexStr[7] = '0';
	hexStr[8] = '\0';
}

void assemble(s_token* tokens, const char* output_file) {
	FILE* out = fopen(output_file, "w");
	if (!out) {
		fprintf(stderr, "Failed to open output\n");
		exit(-1);
	}

	static const s_InstructionPattern patterns[] = {
	    {.name = "MOV", .opcode = "00", .builder = build_rm, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_MEMORY},
	    {.name = "MOV", .opcode = "01", .builder = build_mr_reverse, .dest_type = TOKEN_MEMORY, .src_type = TOKEN_REGISTER},
	    {.name = "MOV", .opcode = "02", .builder = build_mr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_MEMORY_REGISTER},
	    {.name = "MOV", .opcode = "03", .builder = build_mr, .dest_type = TOKEN_MEMORY_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "MOV", .opcode = "04", .builder = build_ri, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_LITERAL},
	    {.name = "MOV", .opcode = "05", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "ADD", .opcode = "06", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "SUB", .opcode = "07", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "MUL", .opcode = "08", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "DIV", .opcode = "09", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "AND", .opcode = "0A", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "OR", .opcode = "0B", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "JZ", .opcode = "0C", .builder = build_ri, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_LITERAL},
	    {.name = "CMP", .opcode = "0D", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "LESS", .opcode = "0E", .builder = build_rr, .dest_type = TOKEN_REGISTER, .src_type = TOKEN_REGISTER},
	    {.name = "NOP", .opcode = "0F", .builder = NULL, .dest_type = 0, .src_type = 0}};

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
				char hexStr[9] = {0};
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
