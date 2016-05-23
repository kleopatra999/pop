// value.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_VALUE_HPP
#define POP_VALUE_HPP

#if !defined(POP_COMPILING) && !defined(POP_HPP_INCLUDED)
#error "Invalid individual include, include only the <pop/pop.hpp> header"
#endif

#include <pop/error.hpp>
#include <pop/types.hpp>
#include <sstream>
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
	SLICE,
	ENV,
	OBJECT,
	FUNC,
};

enum class ValueFlag : Uint8
{
	NONE = 0,
	MARK = (1 << 0),
};

const char *value_type_name(ValueType type);

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
	const char *type_name() const
	{
		return value_type_name(type);
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
	virtual std::string _repr_() const = 0;
	virtual size_t _hash_() const
	{
		// identity
		return std::hash<std::uintptr_t>()(
		    reinterpret_cast<std::uintptr_t>(this));
	}
	virtual bool _not_() const = 0;
	Value *_add_(Value *right) const;
	Value *_sub_(Value *right) const;
	Value *_mul_(Value *right) const;
	Value *_div_(Value *right) const;
	Value *_mod_(Value *right) const;
	Value *_pow_(Value *right) const;
	Value *_pos_() const;
	Value *_neg_() const;
	Value *_log_and_(Value *right) const;
	Value *_log_or_(Value *right) const;
	Value *_log_not_() const;
	Value *_bit_and_(Value *right) const;
	Value *_bit_or_(Value *right) const;
	Value *_bit_xor_(Value *right) const;
	Value *_bit_not_() const;
	Value *_lshift_(Value *right) const;
	Value *_rshift_(Value *right) const;
	Value *_ip_add_(Value *right);
	Value *_ip_sub_(Value *right);
	Value *_ip_mul_(Value *right);
	Value *_ip_div_(Value *right);
	Value *_ip_mod_(Value *right);
	Value *_ip_pow_(Value *right);
	Value *_ip_and_(Value *right);
	Value *_ip_or_(Value *right);
	Value *_ip_xor_(Value *right);
	Value *_ip_lshift_(Value *right);
	Value *_ip_rshift_(Value *right);
	Value *_preinc_();
	Value *_predec_();
	Value *_postinc_();
	Value *_postdec_();
	Value *_eq_(Value *right) const;
	Value *_ne_(Value *right) const;
	Value *_gt_(Value *right) const;
	Value *_ge_(Value *right) const;
	Value *_lt_(Value *right) const;
	Value *_le_(Value *right) const;
};

struct ValueHasher
{
	size_t operator()(const Value *value) const
	{
		return value->_hash_();
	}
};

typedef std::unordered_map<Value *, Value *, ValueHasher> ValueMap;
typedef std::vector<Value *> ValueList;

struct Null final : public Value
{
	Null() : Value(ValueType::NUL)
	{
	}
	virtual std::string _repr_() const override final
	{
		return "Null";
	}
	virtual bool _not_() const override final
	{
		return true;
	}
};

struct Bool final : public Value
{
	bool value;
	Bool(bool value = false) : Value(ValueType::BOOL), value(value)
	{
	}
	virtual std::string _repr_() const override final
	{
		return value ? "True" : "False";
	}
	virtual bool _not_() const override final
	{
		return !value;
	}
};

struct Int final : public Value
{
	long long int value;
	Int(long long int value = 0) : Value(ValueType::INT), value(value)
	{
	}
	virtual std::string _repr_() const override final
	{
		return std::to_string(value);
	}
	virtual size_t _hash_() const override final
	{
		return std::hash<long long int>()(value);
	}
	virtual bool _not_() const override final
	{
		return (value == 0);
	}
};

struct Float final : public Value
{
	Float64 value;
	Float(Float64 value = 0.0) : Value(ValueType::FLOAT), value(value)
	{
	}
	virtual std::string _repr_() const override final
	{
		return std::to_string(value);
	}
	virtual size_t _hash_() const override final
	{
		return std::hash<Float64>()(value);
	}
	virtual bool _not_() const override final
	{
		return (value == 0.0);
	}
};

struct String final : public Value
{
	std::string value;
	String(const std::string &value = std::string())
	    : Value(ValueType::STRING), value(value)
	{
	}
	virtual std::string _repr_() const override final
	{
		return "'" + value + "'";
	}
	virtual size_t _hash_() const override final
	{
		return std::hash<std::string>()(value);
	}
	virtual bool _not_() const override final
	{
		return value.empty();
	}
};

struct Symbol final : public Value
{
	std::string name;
	Symbol(const std::string &name = std::string())
	    : Value(ValueType::SYMBOL), name(name)
	{
	}
	virtual std::string _repr_() const override final
	{
		return name;
	}
	virtual size_t _hash_() const override final
	{
		return std::hash<std::string>()(name);
	}
	virtual bool _not_() const override final
	{
		return name.empty();
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
	virtual std::string _repr_() const override final
	{
		std::stringstream ss;
		ss << "[";
		for (auto elem : elements)
			ss << elem->_repr_();
		ss << "]";
		return ss.str();
	}
	// FIXME: runtime error if List::_hash_() is called
	virtual bool _not_() const override final
	{
		return elements.empty();
	}
	void append(Value *val)
	{
		elements.emplace_back(val);
	}
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
	virtual std::string _repr_() const override final
	{
		std::stringstream ss;
		ss << "{";
		for (auto &pair : table)
		{
			ss << pair.first->_repr_() << ": " << pair.second->_repr_() << ",";
		}
		ss << "}";
		return ss.str();
	}
	// FIXME: runtime error if List::_hash_() is called
	virtual bool _not_() const override final
	{
		return table.empty();
	}
	void insert(Value *key, Value *value)
	{
		table.emplace(key, value);
	}
};

struct Slice final : public Value
{
	Value *start;
	Value *stop;
	Value *step;
	Slice(Value *start, Value *stop, Value *step)
	    : Value(ValueType::SLICE), start(start), stop(stop), step(step)
	{
	}
	virtual void trace() override final
	{
		set_mark();
		if (start)
			start->trace();
		if (stop)
			stop->trace();
		if (step)
			step->trace();
	}
	virtual std::string _repr_() const override final
	{
		std::stringstream ss;
		ss << "<Slice start='" << start->_repr_() << "' stop='"
		   << stop->_repr_() << "' step='" << step->_repr_() << "'>";
		return ss.str();
	}
	virtual bool _not_() const override final
	{
		return (!start && !stop && !step);
	}
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
	virtual std::string _repr_() const override final
	{
		std::stringstream ss;
		ss << "<Env at='" << static_cast<const void *>(this) << "'>";
		return ss.str();
	}
	virtual bool _not_() const override final
	{
		return table.empty();
	}
	void define(Value *name, Value *value)
	{
		table.emplace(name, value);
	}
	void define(const std::string &name, Value *value)
	{
		table.emplace(new Pop::String(name), value);
	}
	Value *lookup(Value *key, bool search_parent = true)
	{
		auto found = table.find(key);
		if (found != table.end())
			return found->second;
		if (search_parent && parent)
			return parent->lookup(key, search_parent);
		return nullptr;
	}
	Value *lookup(const std::string &key, bool search_parent = true)
	{
		return lookup(new Pop::String(key), search_parent);
	}
	bool is_defined(Value *key, bool search_parent = true)
	{
		return (lookup(key, search_parent) != nullptr);
	}
	bool is_defined(const std::string &key, bool search_parent = true)
	{
		return is_defined(new Pop::String(key), search_parent);
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
	virtual std::string _repr_() const override final
	{
		std::stringstream ss;
		ss << "<Object at='" << static_cast<const void *>(this) << "'>";
		return ss.str();
	}
	Value *getattr(Value *name) const
	{
		auto found = members.find(name);
		if (found != members.end())
			return found->second;
		return nullptr;
	}
	Value *getattr(const std::string &name) const
	{
		return getattr(new Pop::String(name));
	}
	virtual bool _not_() const override final
	{
		return false; // TODO: _not_() should look in members for special
		              // operator function (eg. _not_())
	}
};

struct Function final : public Value
{
	CodeAddr addr;
	Env *env;
	Function(CodeAddr addr, Env *env)
	    : Value(ValueType::FUNC), addr(addr), env(env)
	{
	}
	virtual void trace() override final
	{
		set_mark();
		env->trace();
	}
	virtual std::string _repr_() const override final
	{
		std::stringstream ss;
		ss << "<Function addr='" << addr << "' at='"
		   << static_cast<const void *>(this) << "'> ";
		return ss.str();
	}
	virtual bool _not_() const override final
	{
		return false;
	}
};

// namespace Pop
}

#endif // POP_VALUE_HPP
