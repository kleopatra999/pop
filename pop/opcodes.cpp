// opcodes.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/opcodes.hpp>
#include <cassert>

namespace Pop
{

const char *opcode_name(OpCode code)
{
	switch (code)
	{
		case OpCode::OP_HALT:
			return "HALT";
		case OpCode::OP_NOP:
			return "NOP";
		case OpCode::OP_PRINT:
			return "PRINT";

		case OpCode::OP_OPEN_SCOPE:
			return "OPEN_SCOPE";
		case OpCode::OP_CLOSE_SCOPE:
			return "CLOSE_SCOPE";
		case OpCode::OP_BIND:
			return "BIND";

		case OpCode::OP_CALL:
			return "CALL";
		case OpCode::OP_RETURN:
			return "RETURN";
		case OpCode::OP_JUMP:
			return "JUMP";
		case OpCode::OP_JUMP_TRUE:
			return "JUMP_TRUE";
		case OpCode::OP_JUMP_FALSE:
			return "JUMP_FALSE";

		case OpCode::OP_POP_TOP:
			return "POP_TOP";
		case OpCode::OP_PUSH_NULL:
			return "PUSH_NULL";
		case OpCode::OP_PUSH_TRUE:
			return "PUSH_TRUE";
		case OpCode::OP_PUSH_FALSE:
			return "PUSH_FALSE";
		case OpCode::OP_PUSH_INT:
			return "PUSH_INT";
		case OpCode::OP_PUSH_FLOAT:
			return "PUSH_FLOAT";
		case OpCode::OP_PUSH_STRING:
			return "PUSH_STRING";
		case OpCode::OP_PUSH_SYMBOL:
			return "PUSH_SYMBOL";
		case OpCode::OP_PUSH_LIST:
			return "PUSH_LIST";
		case OpCode::OP_PUSH_DICT:
			return "PUSH_DICT";
		case OpCode::OP_PUSH_SLICE:
			return "PUSH_SLICE";
		case OpCode::OP_PUSH_FUNCTION:
			return "PUSH_FUNCTION";

		case OpCode::OP_INDEX:
			return "INDEX";
		case OpCode::OP_MEMBER:
			return "MEMBER";

		case OpCode::OP_ADD:
			return "ADD";
		case OpCode::OP_SUB:
			return "SUB";
		case OpCode::OP_MUL:
			return "MUL";
		case OpCode::OP_DIV:
			return "DIV";
		case OpCode::OP_MOD:
			return "MOD";
		case OpCode::OP_POW:
			return "POW";
		case OpCode::OP_POS:
			return "POSITIVE";
		case OpCode::OP_NEG:
			return "NEGATIVE";

		case OpCode::OP_LOG_AND:
			return "LOG_AND";
		case OpCode::OP_LOG_OR:
			return "LOG_OR";
		case OpCode::OP_LOG_NOT:
			return "LOG_NOT";
		case OpCode::OP_BIT_AND:
			return "BIT_AND";
		case OpCode::OP_BIT_OR:
			return "BIT_OR";
		case OpCode::OP_BIT_XOR:
			return "BIT_XOR";
		case OpCode::OP_BIT_NOT:
			return "BIT_NOT";
		case OpCode::OP_LEFT_SHIFT:
			return "LEFT_SHIFT";
		case OpCode::OP_RIGHT_SHIFT:
			return "RIGHT_SHIFT";

		case OpCode::OP_IP_ADD:
			return "IP_ADD";
		case OpCode::OP_IP_SUB:
			return "IP_SUB";
		case OpCode::OP_IP_MUL:
			return "IP_MUL";
		case OpCode::OP_IP_DIV:
			return "IP_DIV";
		case OpCode::OP_IP_MOD:
			return "IP_MOD";
		case OpCode::OP_IP_POW:
			return "IP_POW";
		case OpCode::OP_IP_AND:
			return "IP_AND";
		case OpCode::OP_IP_OR:
			return "IP_OR";
		case OpCode::OP_IP_XOR:
			return "IP_XOR";
		case OpCode::OP_IP_LEFT:
			return "IP_LEFT";
		case OpCode::OP_IP_RIGHT:
			return "IP_RIGHT";
		case OpCode::OP_IP_ASSIGN:
			return "ASSIGN";
		case OpCode::OP_IP_PREINC:
			return "PREINC";
		case OpCode::OP_IP_PREDEC:
			return "PREDEC";
		case OpCode::OP_IP_POSTINC:
			return "POSTINC";
		case OpCode::OP_IP_POSTDEC:
			return "POSTDEC";

		case OpCode::OP_EQ:
			return "EQ";
		case OpCode::OP_NE:
			return "NE";
		case OpCode::OP_GT:
			return "GT";
		case OpCode::OP_GE:
			return "GE";
		case OpCode::OP_LT:
			return "LT";
		case OpCode::OP_LE:
			return "LE";

		case OpCode::OP_LABEL:
			// assert(false);
			return "~~LABEL~~";
	}
	return "~~UNKNOWN~~";
}

OpCode opcode_from_token(TokenKind kind)
{
	switch (kind)
	{
		case TokenKind::ADD:
			return OpCode::OP_ADD;
		case TokenKind::SUB:
			return OpCode::OP_SUB;
		case TokenKind::MUL:
			return OpCode::OP_MUL;
		case TokenKind::DIV:
			return OpCode::OP_DIV;
		case TokenKind::MOD:
			return OpCode::OP_MOD;
		case TokenKind::POW:
			return OpCode::OP_POW;
		case TokenKind::UPLUS:
			return OpCode::OP_POS;
		case TokenKind::UMINUS:
			return OpCode::OP_NEG;
		case TokenKind::L_AND:
			return OpCode::OP_LOG_AND;
		case TokenKind::L_OR:
			return OpCode::OP_LOG_OR;
		case TokenKind::L_NOT:
			return OpCode::OP_LOG_NOT;
		case TokenKind::B_AND:
			return OpCode::OP_BIT_AND;
		case TokenKind::B_OR:
			return OpCode::OP_BIT_OR;
		case TokenKind::B_XOR:
			return OpCode::OP_BIT_XOR;
		case TokenKind::B_NOT:
			return OpCode::OP_BIT_NOT;
		case TokenKind::LSHIFT:
			return OpCode::OP_LEFT_SHIFT;
		case TokenKind::RSHIFT:
			return OpCode::OP_RIGHT_SHIFT;
		case TokenKind::ADD_ASSIGN:
			return OpCode::OP_IP_ADD;
		case TokenKind::SUB_ASSIGN:
			return OpCode::OP_IP_SUB;
		case TokenKind::MUL_ASSIGN:
			return OpCode::OP_IP_MUL;
		case TokenKind::DIV_ASSIGN:
			return OpCode::OP_IP_DIV;
		case TokenKind::MOD_ASSIGN:
			return OpCode::OP_IP_MOD;
		case TokenKind::AND_ASSIGN:
			return OpCode::OP_IP_AND;
		case TokenKind::OR_ASSIGN:
			return OpCode::OP_IP_OR;
		case TokenKind::XOR_ASSIGN:
			return OpCode::OP_IP_XOR;
		case TokenKind::LEFT_ASSIGN:
			return OpCode::OP_IP_LEFT;
		case TokenKind::RIGHT_ASSIGN:
			return OpCode::OP_IP_RIGHT;
		case TokenKind::ASSIGN:
			return OpCode::OP_IP_ASSIGN;
		case TokenKind::PREINC:
			return OpCode::OP_IP_PREINC;
		case TokenKind::PREDEC:
			return OpCode::OP_IP_PREDEC;
		case TokenKind::POSTINC:
			return OpCode::OP_IP_POSTINC;
		case TokenKind::POSTDEC:
			return OpCode::OP_IP_POSTDEC;
		case TokenKind::EQ:
			return OpCode::OP_EQ;
		case TokenKind::NE:
			return OpCode::OP_NE;
		case TokenKind::GT:
			return OpCode::OP_GT;
		case TokenKind::GE:
			return OpCode::OP_GE;
		case TokenKind::LT:
			return OpCode::OP_LT;
		case TokenKind::LE:
			return OpCode::OP_LE;
		default:
			assert(false);
			return OpCode::OP_HALT;
	}
}

// namespace Pop
}
