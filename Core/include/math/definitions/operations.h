#pragma once

#define OCT_MAX(a, b) ((a) > (b) ? (a) : (b))
#define OCT_MIN(a, b) ((a) < (b) ? (a) : (b))

typedef enum {
	OCT_OP_ADD,
	OCT_OP_SUB,
	OCT_OP_MUL,
	OCT_OP_DIV
} OCT_operations;