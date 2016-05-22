// value.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_VALUE_HPP
#define POP_VALUE_HPP

#include <pop/types.hpp>
#include <string>
#include <unordered_map>
#include <vector>

namespace Pop
{

enum class ValueType : Uint8
{
	NUL,
	BOOL,
	INT,
	FLOAT,
	STRING,
	SYMBOL,
	LIST,
	DICT,
	ENV,
	OBJECT,
	FUNC,
};

enum class ValueFlag : Uint8
{
	NONE = 0,
	MARK = (1 << 0),
};

struct Value
{
	ValueType type;
	ValueFlag flags;
	Value(ValueType type, ValueFlag flags = ValueFlag::NONE)
	    : type(type), flags(flags)
	{
	}
	virtual ~Value()
	{
	}
	void set_mark()
	{
		flags = ValueFlag(Uint8(flags) | Uint8(ValueFlag::MARK));
	}
	void clear_mark()
	{
		flags = ValueFlag(Uint8(flags) & ~(Uint8(ValueFlag::MARK)));
	}
	virtual void trace()
	{
		set_mark();
	}
	virtual size_t hash() const
	{
		// identity
		return std::hash<std::uintptr_t>()(
		    reinterpret_cast<std::uintptr_t>(this));
	}
};

struct ValueHasher
{
	size_t operator()(const Value *value) const
	{
		return value->hash();
	}
};

typedef std::unordered_map<Value *, Value *, ValueHasher> ValueMap;
typedef std::vector<Value *> ValueList;

struct Null final : public Value
{
	Null() : Value(ValueType::NUL)
	{
	}
};

struct Bool final : public Value
{
	bool value;
	Bool(bool value = false) : Value(ValueType::BOOL), value(value)
	{
	}
};

struct Int final : public Value
{
	Uint64 value;
	Int(Uint64 value = 0) : Value(ValueType::INT), value(value)
	{
	}
	virtual size_t hash() const override final
	{
		return std::hash<Uint64>()(value);
	}
};

struct Float final : public Value
{
	Float64 value;
	Float(Float64 value = 0.0) : Value(ValueType::FLOAT), value(value)
	{
	}
	virtual size_t hash() const override final
	{
		return std::hash<Float64>()(value);
	}
};

struct String final : public Value
{
	std::string value;
	String(const std::string &value = std::string())
	    : Value(ValueType::STRING), value(value)
	{
	}
	virtual size_t hash() const override final
	{
		return std::hash<std::string>()(value);
	}
};

struct Symbol final : public Value
{
	std::string name;
	Symbol(const std::string &name = std::string())
	    : Value(ValueType::SYMBOL), name(name)
	{
	}
	virtual size_t hash() const override final
	{
		return std::hash<std::string>()(name);
	}
};

struct List final : public Value
{
	ValueList elements;
	List() : Value(ValueType::LIST)
	{
	}
	virtual void trace() override final
	{
		set_mark();
		for (auto elem : elements)
			elem->trace();
	}
	// FIXME: runtime error if List::hash() is called
};

struct Dict final : public Value
{
	ValueMap table;
	Dict() : Value(ValueType::DICT)
	{
	}
	virtual void trace() override final
	{
		set_mark();
		for (auto &pair : table)
		{
			pair.first->trace();
			pair.second->trace();
		}
	}
	// FIXME: runtime error if List::hash() is called
};

struct Env final : public Value
{
	Env *parent;
	ValueMap table;
	Env(Env *parent = nullptr) : Value(ValueType::ENV), parent(parent)
	{
	}
	virtual void trace() override final
	{
		set_mark();
		for (auto &pair : table)
		{
			pair.first->trace();
			pair.second->trace();
		}
	}
};

struct Object final : public Value
{
	Env *env;
	ValueMap members;
	Object(Env *env = nullptr) : Value(ValueType::OBJECT), env(new Env(env))
	{
	}
	virtual void trace() override final
	{
		set_mark();
		for (auto &pair : members)
		{
			pair.first->trace();
			pair.second->trace();
		}
		env->trace();
	}
};

struct Function final : public Value
{
	CodeAddr addr;
	Env *env;
	Function(CodeAddr addr, Env *env = nullptr)
	    : Value(ValueType::FUNC), addr(addr), env(new Env(env))
	{
	}
	virtual void trace() override final
	{
		set_mark();
		env->trace();
	}
};

// namespace Pop
}

#endif // POP_VALUE_HPP
