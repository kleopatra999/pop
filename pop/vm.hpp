// vm.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_VM_HPP
#define POP_VM_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/decoder.hpp>
#include <pop/opcodes.hpp>
#include <pop/types.hpp>
#include <pop/value.hpp>
#include <cassert>
#include <memory>
#include <stack>
#include <type_traits>

namespace Pop
{

struct ValueStack
{
	ValueList values;
	Value *pop()
	{
		auto val = values.back();
		values.pop_back();
		return val;
	}
	void push(Value *val)
	{
		values.emplace_back(val);
	}
	Value *top() const
	{
		return values.back();
	}
};

struct VM
{
	static constexpr int EXIT_PAUSED = -1;

	CodeAddr ip;
	Decoder dec;
	ValueStack stack;
	std::stack<CodeAddr> return_stack;
	Env *env;
	bool running;
	bool paused;
	int exit_code;
	int argc;
	char **argv;

	VM();
	VM(int argc, char **argv);
	VM(const Uint8 *code, CodeAddr len, int argc = 0, char **argv = nullptr);

	// Machine control
	int execute(const Uint8 *code, CodeAddr len);
	int execute();
	void pause();
	void resume();
	void exit(int exit_code = 0);

	void dump_stack();

	void call(unsigned int)
	{
		auto callee = pop();
		if (callee->type == ValueType::FUNC)
		{
			auto addr = static_cast<Function *>(callee)->addr;
			return_stack.push(ip);
			ip = addr;
		}
		else
		{
			dump_stack();
			std::stringstream ss;
			ss << "value type '" << callee->type_name()
			   << "' is not callable at '" << std::hex << ip << "'";
			throw RuntimeError(ss.str());
		}
	}

	Value *pop()
	{
		assert(!stack.values.empty());
		auto top = stack.top();
		stack.pop();
		return top;
	}

	void push(Value *value)
	{
		stack.push(value);
	}

	template <class T, class... Args>
	T *push_new(Args &&... args)
	{
		static_assert(std::is_base_of<Value, T>::value,
		              "can only push Pop::Value subclasses to stack");
		auto x = new T(std::forward<Args>(args)...);
		stack.push(x);
		return x;
	}
};

// namespace Pop
}

#endif // POP_VM_HPP
