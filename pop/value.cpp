// value.cpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifdef HAVE_CONFIG_H
#include <pop/config.h>
#endif

#include <pop/value.hpp>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

namespace Pop
{

const char *value_type_name(ValueType type)
{
	switch (type)
	{
		case ValueType::NUL:
			return "Null";
		case ValueType::BOOL:
			return "Bool";
		case ValueType::INT:
			return "Int";
		case ValueType::FLOAT:
			return "Float";
		case ValueType::STRING:
			return "String";
		case ValueType::SYMBOL:
			return "Symbol";
		case ValueType::LIST:
			return "List";
		case ValueType::DICT:
			return "Dict";
		case ValueType::SLICE:
			return "Slice";
		case ValueType::ENV:
			return "Env";
		case ValueType::OBJECT:
			return "Object";
		case ValueType::FUNC:
			return "Func";
	}
	return "Unknown";
}

bool Value::_equal_(const Value *right) const
{
	if (type == ValueType::NUL && right->type == ValueType::NUL)
	{
		return true;
	}
	else if (type == ValueType::BOOL && right->type == ValueType::BOOL)
	{
		return (static_cast<const Bool*>(this)->value ==
				static_cast<const Bool*>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return (static_cast<const Int*>(this)->value ==
				static_cast<const Int*>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return (static_cast<const Int*>(this)->value ==
				static_cast<const Float*>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return (static_cast<const Float*>(this)->value ==
				static_cast<const Int*>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return (static_cast<const Float*>(this)->value ==
				static_cast<const Float*>(right)->value);
	}
	else if (type == ValueType::STRING && right->type == ValueType::STRING)
	{
		return (static_cast<const String*>(this)->value ==
				static_cast<const String*>(right)->value);
	}
	else if (type == ValueType::FUNC && right->type == ValueType::FUNC)
	{
		return (static_cast<const Function*>(this)->addr ==
				static_cast<const Function*>(right)->addr);
	}
	else if (type == ValueType::OBJECT && right->type == ValueType::OBJECT)
	{
		return (static_cast<const void*>(this) == static_cast<const void*>(right));
	}
	else
	{
		std::stringstream ss;
		ss << "cannot test equality of types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		throw RuntimeError(ss.str());
	}
	return false;
}

Value *Value::_add_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value +
		               static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Int *>(this)->value +
		                 static_cast<const Float *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return new Float(static_cast<const Float *>(this)->value +
		                 static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Float *>(this)->value +
		                 static_cast<const Float *>(right)->value);
	}
	else if (type == ValueType::STRING && right->type == ValueType::STRING)
	{
		return new String(static_cast<const String *>(this)->value +
		                  static_cast<const String *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot add types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_sub_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value -
		               static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Int *>(this)->value -
		                 static_cast<const Float *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return new Float(static_cast<const Float *>(this)->value -
		                 static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Float *>(this)->value -
		                 static_cast<const Float *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot subtract types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_mul_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value *
		               static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Int *>(this)->value *
		                 static_cast<const Float *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return new Float(static_cast<const Float *>(this)->value *
		                 static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Float *>(this)->value *
		                 static_cast<const Float *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot multiply types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_div_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value /
		               static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Int *>(this)->value /
		                 static_cast<const Float *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return new Float(static_cast<const Float *>(this)->value /
		                 static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return new Float(static_cast<const Float *>(this)->value /
		                 static_cast<const Float *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot divide types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_mod_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value %
		               static_cast<const Int *>(right)->value);
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return new Float(std::fmod(static_cast<const Int *>(this)->value,
		                           static_cast<const Float *>(right)->value));
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return new Float(std::fmod(static_cast<const Float *>(this)->value,
		                           static_cast<const Int *>(right)->value));
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return new Float(std::fmod(static_cast<const Float *>(this)->value,
		                           static_cast<const Float *>(right)->value));
	}
	else
	{
		std::stringstream ss;
		ss << "cannot modulo types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_pow_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(std::pow(static_cast<const Int *>(this)->value,
		                        static_cast<const Int *>(right)->value));
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		return new Float(std::pow(static_cast<const Int *>(this)->value,
		                          static_cast<const Float *>(right)->value));
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		return new Float(std::pow(static_cast<const Float *>(this)->value,
		                          static_cast<const Int *>(right)->value));
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		return new Float(std::pow(static_cast<const Float *>(this)->value,
		                          static_cast<const Float *>(right)->value));
	}
	else
	{
		std::stringstream ss;
		ss << "cannot raise type '" << type_name() << "' to power of type '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_pos_() const
{
	if (type == ValueType::INT)
		return new Int(+(static_cast<const Int *>(this)->value));
	else if (type == ValueType::FLOAT)
		return new Float(+(static_cast<const Float *>(this)->value));
	else
	{
		std::stringstream ss;
		ss << "cannot make type '" << type_name() << "' positive";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_neg_() const
{
	if (type == ValueType::INT)
		return new Int(-(static_cast<const Int *>(this)->value));
	else if (type == ValueType::FLOAT)
		return new Float(-(static_cast<const Float *>(this)->value));
	else
	{
		std::stringstream ss;
		ss << "cannot make type '" << type_name() << "' negative";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_log_and_(Value *right) const
{
	auto left_true = !this->_not_();
	auto right_true = !right->_not_();
	return new Bool(left_true && right_true);
}

Value *Value::_log_or_(Value *right) const
{
	auto left_true = !_not_();
	auto right_true = !right->_not_();
	return new Bool(left_true || right_true);
}

Value *Value::_log_not_() const
{
	return new Bool(_not_());
}

Value *Value::_bit_and_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value &
		               static_cast<const Int *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot perform bitwise-and on types '" << type_name()
		   << "' and '" << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_bit_or_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value |
		               static_cast<const Int *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot perform bitwise-or on types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_bit_xor_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value ^
		               static_cast<const Int *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot perform bitwise-xor on types '" << type_name()
		   << "' and '" << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_bit_not_() const
{
	if (type == ValueType::INT)
		return new Int(~(static_cast<const Int *>(this)->value));
	else
	{
		std::stringstream ss;
		ss << "cannot perform bitwise-not on type '" << type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_lshift_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value
		               << static_cast<const Int *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot perform left-shift on types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_rshift_(Value *right) const
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		return new Int(static_cast<const Int *>(this)->value >>
		               static_cast<const Int *>(right)->value);
	}
	else
	{
		std::stringstream ss;
		ss << "cannot perform right-shift on types '" << type_name()
		   << "' and '" << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_add_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value +=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		static_cast<Int *>(this)->value +=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		static_cast<Float *>(this)->value +=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		static_cast<Float *>(this)->value +=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else if (type == ValueType::STRING && right->type == ValueType::STRING)
	{
		static_cast<String *>(this)->value +=
		    static_cast<const String *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place add types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_sub_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value -=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		static_cast<Int *>(this)->value -=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		static_cast<Float *>(this)->value -=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		static_cast<Float *>(this)->value -=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place subtract types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_mul_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value *=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		static_cast<Int *>(this)->value *=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		static_cast<Float *>(this)->value *=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		static_cast<Float *>(this)->value *=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place multiply types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_div_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value /=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		static_cast<Int *>(this)->value /=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		static_cast<Float *>(this)->value /=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		static_cast<Float *>(this)->value /=
		    static_cast<const Float *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place divide types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_mod_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value %=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		static_cast<Int *>(this)->value =
		    std::fmod(static_cast<const Int *>(this)->value,
		              static_cast<const Float *>(right)->value);
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		static_cast<Float *>(this)->value =
		    std::fmod(static_cast<const Float *>(this)->value,
		              static_cast<const Int *>(right)->value);
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		static_cast<Float *>(this)->value =
		    std::fmod(static_cast<const Float *>(this)->value,
		              static_cast<const Float *>(right)->value);
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place modulo types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_pow_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value =
		    std::pow(static_cast<const Int *>(this)->value,
		             static_cast<const Int *>(right)->value);
		return this;
	}
	else if (type == ValueType::INT && right->type == ValueType::FLOAT)
	{
		static_cast<Int *>(this)->value =
		    std::pow(static_cast<const Int *>(this)->value,
		             static_cast<const Float *>(right)->value);
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::INT)
	{
		static_cast<Float *>(this)->value =
		    std::pow(static_cast<const Float *>(this)->value,
		             static_cast<const Int *>(right)->value);
		return this;
	}
	else if (type == ValueType::FLOAT && right->type == ValueType::FLOAT)
	{
		static_cast<Float *>(this)->value =
		    std::pow(static_cast<const Float *>(this)->value,
		             static_cast<const Float *>(right)->value);
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place raise type '" << type_name()
		   << "' to power of type '" << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_and_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value &=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place bitwise-and types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_or_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value |=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place bitwise-or types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_xor_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value ^=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place bitwise-xor types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_lshift_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value <<=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place left-shift types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_ip_rshift_(Value *right)
{
	if (type == ValueType::INT && right->type == ValueType::INT)
	{
		static_cast<Int *>(this)->value >>=
		    static_cast<const Int *>(right)->value;
		return this;
	}
	else
	{
		std::stringstream ss;
		ss << "cannot in-place right-shift types '" << type_name() << "' and '"
		   << right->type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_preinc_()
{
	if (type == ValueType::INT)
		return new Int(++(static_cast<Int *>(this)->value));
	else if (type == ValueType::FLOAT)
		return new Float(++(static_cast<Float *>(this)->value));
	else
	{
		std::stringstream ss;
		ss << "cannot pre-increment type '" << type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_predec_()
{
	if (type == ValueType::INT)
		return new Int(--(static_cast<Int *>(this)->value));
	else if (type == ValueType::FLOAT)
		return new Float(--(static_cast<Float *>(this)->value));
	else
	{
		std::stringstream ss;
		ss << "cannot pre-decrement type '" << type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_postinc_()
{
	if (type == ValueType::INT)
		return new Int((static_cast<Int *>(this)->value)++);
	else if (type == ValueType::FLOAT)
		return new Float((static_cast<Float *>(this)->value)++);
	else
	{
		std::stringstream ss;
		ss << "cannot post-increment type '" << type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_postdec_()
{
	if (type == ValueType::INT)
		return new Int((static_cast<Int *>(this)->value)--);
	else if (type == ValueType::FLOAT)
		return new Float((static_cast<Float *>(this)->value)--);
	else
	{
		std::stringstream ss;
		ss << "cannot post-decrement type '" << type_name() << "'";
		new RuntimeError(ss.str());
	}
	return nullptr;
}

Value *Value::_eq_(const Value *right) const
{
	return new Bool(_equal_(right));
}

Value *Value::_ne_(const Value *right) const
{
	return nullptr;
}

Value *Value::_gt_(const Value *right) const
{
	return nullptr;
}

Value *Value::_ge_(const Value *right) const
{
	return nullptr;
}

Value *Value::_lt_(const Value *right) const
{
	return nullptr;
}

Value *Value::_le_(const Value *right) const
{
	return nullptr;
}

// namespace Pop
}
