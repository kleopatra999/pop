// opcodes.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_OPCODES_HPP
#define POP_OPCODES_HPP

#include <pop/token.hpp>
#include <cstdint>

namespace Pop
{

enum class OpCode : std::uint8_t
{
	OP_HALT,
	OP_NOP,

	OP_OPEN_SCOPE,
	OP_CLOSE_SCOPE,
	OP_BIND,

	OP_CALL,
	OP_RETURN,
	OP_JUMP,
	OP_JUMP_TRUE,
	OP_JUMP_FALSE,

	OP_POP_TOP,
	OP_PUSH_NULL,
	OP_PUSH_TRUE,
	OP_PUSH_FALSE,
	OP_PUSH_INT,
	OP_PUSH_FLOAT,
	OP_PUSH_STRING,
	OP_PUSH_SYMBOL,
	OP_PUSH_LIST,
	OP_PUSH_DICT,
	OP_PUSH_SLICE,
	OP_PUSH_FUNCTION,

	OP_INDEX,
	OP_MEMBER,

	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_MOD,
	OP_POW,
	OP_POS,
	OP_NEG,

	OP_LOG_AND,
	OP_LOG_OR,
	OP_LOG_NOT,

	OP_BIT_AND,
	OP_BIT_OR,
	OP_BIT_XOR,
	OP_BIT_NOT,
	OP_LEFT_SHIFT,
	OP_RIGHT_SHIFT,

	OP_IP_ADD,
	OP_IP_SUB,
	OP_IP_MUL,
	OP_IP_DIV,
	OP_IP_MOD,
	OP_IP_AND,
	OP_IP_OR,
	OP_IP_XOR,
	OP_IP_NOT,
	OP_IP_LEFT,
	OP_IP_RIGHT,
	OP_IP_ASSIGN,
	OP_IP_PREINC,
	OP_IP_PREDEC,
	OP_IP_POSTINC,
	OP_IP_POSTDEC,

	OP_EQ,
	OP_NE,
	OP_GT,
	OP_GE,
	OP_LT,
	OP_LE,

	OP_LABEL = 255,
};

const char *opcode_name(OpCode code);
OpCode opcode_from_token(TokenKind kind);

// namespace Pop
}

#endif // POP_OPCODES_HPP
