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

struct VMTrace
{
	static size_t ind_level;
	std::string name;
	CodeAddr &ip;
	std::string ind_str() const
	{
		return std::string(ind_level * 2, ' ');
	}
	VMTrace(const std::string &name, CodeAddr &ip) : name(name), ip(ip)
	{
		std::cerr << ind_str() << "entering '" << name << "' with IP at "
		          << std::hex << ip << std::endl;
		ind_level++;
	}
	~VMTrace()
	{
		ind_level--;
		std::cerr << ind_str() << "leaving '" << name << "' with IP at "
		          << std::hex << ip << std::endl;
	}
};

size_t VMTrace::ind_level = 0;

//#define VM_TRACE 1

#ifdef VM_TRACE
#define VM_TRACE_ENTER(op)                        \
	{                                             \
		VMTrace _trace_##__COUNTER__##_(#op, ip); \
		{

#define VM_TRACE_LEAVE() \
	}                    \
	}
#else
#define VM_TRACE_ENTER(op) {
#define VM_TRACE_LEAVE() }
#endif

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
		// std::cerr << ".";
		switch (op)
		{
			case OpCode::OP_HALT:
				VM_TRACE_ENTER(HALT)
				running = false;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_NOP:
				VM_TRACE_ENTER(NOP)
				// do nothing
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PRINT:
				VM_TRACE_ENTER(PRINT)
				std::cout << pop()->_repr_() << std::endl;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_OPEN_SCOPE:
				VM_TRACE_ENTER(OPEN_SCOPE)
				env = new Env(env);
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_CLOSE_SCOPE:
				VM_TRACE_ENTER(CLOSE_SCOPE)
				env = env->parent;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_BIND:
				VM_TRACE_ENTER(BIND)
				auto name = dec.read_name();
				auto value = pop();
				env->define(name, value);
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_CALL:
				VM_TRACE_ENTER(CALL)
				call(dec.read_u8());
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_RETURN:
				VM_TRACE_ENTER(RETURN)
				auto addr = return_stack.top();
				return_stack.pop();
				ip = addr;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_JUMP:
				VM_TRACE_ENTER(JUMP)
				auto addr = dec.read_addr();
				assert(addr < dec.len);
				ip = addr;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_JUMP_TRUE:
				VM_TRACE_ENTER(JUMP_TRUE)
				auto addr = dec.read_addr();
				assert(addr < dec.len);
				auto value = pop();
				if (!value->_not_())
					ip = addr;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_JUMP_FALSE:
				VM_TRACE_ENTER(JUMP_FALSE)
				auto addr = dec.read_addr();
				assert(addr < dec.len);
				auto value = pop();
				if (value->_not_())
					ip = addr;
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_POP_TOP:
				VM_TRACE_ENTER(POP_TOP)
				stack.pop();
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_NULL:
				VM_TRACE_ENTER(PUSH_NULL)
				push_new<Null>();
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_TRUE:
				VM_TRACE_ENTER(PUSH_TRUE)
				push_new<Bool>(true);
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_FALSE:
				VM_TRACE_ENTER(PUSH_FALSE)
				push_new<Bool>(false);
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_INT:
				VM_TRACE_ENTER(PUSH_INT)
				push_new<Int>(dec.read_s64());
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_FLOAT:
				VM_TRACE_ENTER(PUSH_FLOAT)
				push_new<Float>(dec.read_f64());
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_STRING:
				VM_TRACE_ENTER(PUSH_STRING)
				push_new<String>(dec.read_string());
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_SYMBOL:
				VM_TRACE_ENTER(PUSH_SYMBOL)
				auto name = dec.read_name();
				if (auto value = env->lookup(name, true))
					push(value);
				else
				{
					std::stringstream ss;
					ss << "undefined symbol '" << name << "'";
					throw RuntimeError(ss.str());
				}
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_LIST:
				VM_TRACE_ENTER(PUSH_LIST)
				auto len = dec.read_u32();
				auto list = new List();
				for (auto i = 0u; i < len; i++)
					list->append(pop());
				push(list);
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_DICT:
				VM_TRACE_ENTER(PUSH_DICT)
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
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_SLICE:
				VM_TRACE_ENTER(PUSH_SLICE)
				auto start = pop();
				auto stop = pop();
				auto step = pop();
				push_new<Slice>(start, stop, step);
				break;
				VM_TRACE_LEAVE()
			case OpCode::OP_PUSH_FUNCTION:
				VM_TRACE_ENTER(PUSH_FUNCTION)
				auto closure = new Env(env);
				push_new<Function>(dec.read_addr(), closure);
				break;
				VM_TRACE_LEAVE()

			case OpCode::OP_IP_ASSIGN:
				VM_TRACE_ENTER(IP_ASSIGN)
				// TODO: look in current scope to make sure its defined
				// then re-bind it with the new value
				break;
				VM_TRACE_LEAVE()

//
// Builtin operators
//
#define VM_BINOP_CASE(op, fnc)        \
	case OpCode::OP_##op:             \
		VM_TRACE_ENTER(op)            \
		auto left = pop();            \
		auto right = pop();           \
		push(left->_##fnc##_(right)); \
		break;                        \
		VM_TRACE_LEAVE()

#define VM_UNOP_CASE(op, fnc)    \
	case OpCode::OP_##op:        \
		VM_TRACE_ENTER(op)       \
		auto left = pop();       \
		push(left->_##fnc##_()); \
		break;                   \
		VM_TRACE_LEAVE()

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

void VM::dump_stack()
{
	if (stack.values.empty())
	{
		std::cerr << "StackEmpty;\n";
	}
	else
	{
		for (size_t i = 0; i < stack.values.size(); i++)
		{
			std::cerr << "stack[" << i << "]=" << stack.values[i]->_repr_()
			          << std::endl;
		}
	}
}

// namespace Pop
}
