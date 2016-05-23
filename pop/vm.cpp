// vm.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/vm.hpp>
#include <iostream>

namespace Pop
{

VM::VM() : VM(0, nullptr)
{
}

VM::VM(int argc, char **argv) : VM(nullptr, 0, 0, nullptr)
{
}

VM::VM(const Uint8 *code, CodeAddr len, int argc, char **argv)
    : ip(0), dec(&ip, code, len), env(new Env(nullptr)), running(false),
      paused(false), exit_code(0), argc(argc), argv(argv)
{
}

int VM::execute(const Uint8 *code, CodeAddr len)
{
	dec = Decoder(&ip, code, len);
	running = true;
	paused = false;
	exit_code = 0;

	while (running && !paused)
	{
		auto op = dec.read_op();
		switch (op)
		{
			case OpCode::OP_HALT:
				running = false;
				break;
			case OpCode::OP_NOP:
				// do nothing
				break;
			case OpCode::OP_PRINT:
				std::cout << pop()->_repr_() << std::endl;
				break;
			case OpCode::OP_OPEN_SCOPE:
				env = new Env(env);
				break;
			case OpCode::OP_CLOSE_SCOPE:
				env = env->parent;
				break;
			case OpCode::OP_BIND:
				env->define(dec.read_name(), pop());
				break;
			case OpCode::OP_CALL:
				call(dec.read_u8());
				break;
			case OpCode::OP_RETURN:
			{
				auto addr = return_stack.top();
				return_stack.pop();
				ip = addr;
				break;
			}
			case OpCode::OP_JUMP:
			{
				auto addr = dec.read_addr();
				assert(addr < dec.len);
				ip = addr;
				break;
			}
			case OpCode::OP_JUMP_TRUE:
			{
				auto addr = dec.read_addr();
				assert(addr < dec.len);
				auto value = pop();
				if (!value->_not_())
					ip = addr;
				break;
			}
			case OpCode::OP_JUMP_FALSE:
			{
				auto addr = dec.read_addr();
				assert(addr < dec.len);
				auto value = pop();
				if (value->_not_())
					ip = addr;
				break;
			}
			case OpCode::OP_POP_TOP:
				stack.pop();
				break;
			case OpCode::OP_PUSH_NULL:
				push_new<Null>();
				break;
			case OpCode::OP_PUSH_TRUE:
				push_new<Bool>(true);
				break;
			case OpCode::OP_PUSH_FALSE:
				push_new<Bool>(false);
				break;
			case OpCode::OP_PUSH_INT:
				push_new<Int>(dec.read_u64());
				break;
			case OpCode::OP_PUSH_FLOAT:
				push_new<Float>(dec.read_f64());
				break;
			case OpCode::OP_PUSH_STRING:
				push_new<String>(dec.read_string());
				break;
			case OpCode::OP_PUSH_SYMBOL:
			{
				if (auto value = env->lookup(dec.read_name(), true))
					push(value);
				else
				{
					// todo: error, undefined symbol
				}
				break;
			}
			case OpCode::OP_PUSH_LIST:
			{
				auto len = dec.read_u32();
				auto list = new List();
				for (auto i = 0u; i < len; i++)
					list->append(pop());
				push(list);
				break;
			}
			case OpCode::OP_PUSH_DICT:
			{
				auto len = dec.read_u32();
				auto dict = new Dict();
				for (auto i = 0u; i < len; i++)
				{
					auto key = pop();
					auto value = pop();
					dict->insert(key, value);
				}
				push(dict);
				break;
			}
			case OpCode::OP_PUSH_SLICE:
			{
				auto start = pop();
				auto stop = pop();
				auto step = pop();
				push_new<Slice>(start, stop, step);
				break;
			}
			case OpCode::OP_PUSH_FUNCTION:
			{
				auto closure = new Env(env);
				push_new<Function>(dec.read_addr(), closure);
				break;
			}

			case OpCode::OP_IP_ASSIGN:
			{
				// TODO: look in current scope to make sure its defined
				// then re-bind it with the new value
				break;
			}

//
// Builtin operators
//
#define VM_BINOP_CASE(op, fnc)        \
	case OpCode::OP_##op:             \
	{                                 \
		auto left = pop();            \
		auto right = pop();           \
		push(left->_##fnc##_(right)); \
		break;                        \
	}

#define VM_UNOP_CASE(op, fnc)    \
	case OpCode::OP_##op:        \
	{                            \
		auto left = pop();       \
		push(left->_##fnc##_()); \
		break;                   \
	}

				// clang-format off
			VM_BINOP_CASE(ADD, add)
			VM_BINOP_CASE(SUB, sub)
			VM_BINOP_CASE(MUL, mul)
			VM_BINOP_CASE(DIV, div)
			VM_BINOP_CASE(MOD, mod)
			VM_BINOP_CASE(POW, pow)
			VM_UNOP_CASE(POS, pos)
			VM_UNOP_CASE(NEG, neg)
			VM_BINOP_CASE(LOG_AND, log_and)
			VM_BINOP_CASE(LOG_OR, log_or)
			VM_UNOP_CASE(LOG_NOT, log_not)
			VM_BINOP_CASE(BIT_AND, bit_and)
			VM_BINOP_CASE(BIT_OR, bit_or)
			VM_BINOP_CASE(BIT_XOR, bit_xor)
			VM_UNOP_CASE(BIT_NOT, bit_not)
			VM_BINOP_CASE(LEFT_SHIFT, lshift)
			VM_BINOP_CASE(RIGHT_SHIFT, rshift)
			VM_BINOP_CASE(IP_ADD, ip_add)
			VM_BINOP_CASE(IP_SUB, ip_sub)
			VM_BINOP_CASE(IP_MUL, ip_mul)
			VM_BINOP_CASE(IP_DIV, ip_div)
			VM_BINOP_CASE(IP_MOD, ip_mod)
			VM_BINOP_CASE(IP_POW, ip_pow)
			VM_BINOP_CASE(IP_AND, ip_and)
			VM_BINOP_CASE(IP_OR, ip_or)
			VM_BINOP_CASE(IP_XOR, ip_xor)
			VM_BINOP_CASE(IP_LEFT, ip_lshift)
			VM_BINOP_CASE(IP_RIGHT, ip_rshift)
			VM_UNOP_CASE(IP_PREINC, preinc)
			VM_UNOP_CASE(IP_PREDEC, predec)
			VM_UNOP_CASE(IP_POSTINC, postinc)
			VM_UNOP_CASE(IP_POSTDEC, postdec)
			VM_BINOP_CASE(EQ, eq)
			VM_BINOP_CASE(NE, ne)
			VM_BINOP_CASE(GT, gt)
			VM_BINOP_CASE(GE, ge)
			VM_BINOP_CASE(LT, lt)
			VM_BINOP_CASE(LE, le)
			// clang-format on
			default:
			{
				std::stringstream ss;
				ss << "unknown instruction '" << Uint8(op) << "'";
				throw RuntimeError(ss.str());
				break;
			}
		}
	}

	return exit_code;
}

int VM::execute()
{
	if (dec.code)
		return execute(dec.code, dec.len);
	else
		return EXIT_FAILURE;
}

void VM::pause()
{
	if (running && !paused)
		paused = true;
	exit_code = EXIT_PAUSED;
}

void VM::resume()
{
	if (running && paused)
		paused = false;
}

void VM::exit(int exit_code_)
{
	if (running)
	{
		exit_code = exit_code_;
		running = false;
		paused = false;
	}
}

// namespace Pop
}
