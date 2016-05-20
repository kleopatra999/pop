#ifndef POP_OPCODES_HPP
#define POP_OPCODES_HPP

#include <pop/token.hpp>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

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

struct Operation
{
	OpCode code;

	virtual void list(std::ostream &out)
	{
		out << "\t" << name() << "\n";
	}

	const char *name() const
	{
		return opcode_name(code);
	}

protected:
	Operation(OpCode code) : code(code)
	{
	}
};

typedef std::unique_ptr<Operation> OpPtr;
typedef std::vector<OpPtr> OpList;

struct Label final : public Operation
{
	std::string name;
	Label(const std::string &name) : Operation(OpCode::OP_LABEL), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << name << ":\n";
	}
};

struct Halt final : public Operation
{
	Halt() : Operation(OpCode::OP_HALT)
	{
	}
};

struct Nop final : public Operation
{
	Nop() : Operation(OpCode::OP_NOP)
	{
	}
};

struct OpenScope final : public Operation
{
	OpenScope() : Operation(OpCode::OP_OPEN_SCOPE)
	{
	}
};

struct CloseScope final : public Operation
{
	CloseScope() : Operation(OpCode::OP_CLOSE_SCOPE)
	{
	}
};

struct Bind final : public Operation
{
	std::string name;
	Bind(const std::string &name) : Operation(OpCode::OP_BIND), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tBIND " << name << "\n";
	}
};

struct Call final : public Operation
{
	Call() : Operation(OpCode::OP_CALL)
	{
	}
};

struct Return final : public Operation
{
	Return() : Operation(OpCode::OP_RETURN)
	{
	}
};

struct Jump final : public Operation
{
	std::string label;
	Jump(const std::string &label) : Operation(OpCode::OP_JUMP), label(label)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tJUMP " << label << "\n";
	}
};

struct JumpTrue final : public Operation
{
	std::string label;
	JumpTrue(const std::string &label)
	    : Operation(OpCode::OP_JUMP_TRUE), label(label)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tJUMP_TRUE " << label << "\n";
	}
};

struct JumpFalse final : public Operation
{
	std::string label;
	JumpFalse(const std::string &label)
	    : Operation(OpCode::OP_JUMP_FALSE), label(label)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tJUMP_FALSE " << label << "\n";
	}
};

struct PopTop final : public Operation
{
	PopTop() : Operation(OpCode::OP_POP_TOP)
	{
	}
};

struct PushNull final : public Operation
{
	PushNull() : Operation(OpCode::OP_PUSH_NULL)
	{
	}
};

struct PushTrue final : public Operation
{
	PushTrue() : Operation(OpCode::OP_PUSH_TRUE)
	{
	}
};

struct PushFalse final : public Operation
{
	PushFalse() : Operation(OpCode::OP_PUSH_FALSE)
	{
	}
};

struct PushInt final : public Operation
{
	std::uint64_t value;
	PushInt(std::uint64_t value) : Operation(OpCode::OP_PUSH_INT), value(value)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_INT " << value << "\n";
	}
};

struct PushFloat final : public Operation
{
	double value;
	PushFloat(double value) : Operation(OpCode::OP_PUSH_FLOAT), value(value)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_FLOAT " << value << "\n";
	}
};

struct PushString final : public Operation
{
	std::string value;
	PushString(const std::string &value)
	    : Operation(OpCode::OP_PUSH_STRING), value(value)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_STRING \"" << value << "\"\n";
	}
};

struct PushSymbol final : public Operation
{
	std::string name;
	PushSymbol(const std::string &name)
	    : Operation(OpCode::OP_PUSH_SYMBOL), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_SYMBOL " << name << "\n";
	}
};

struct PushList final : public Operation
{
	unsigned int len;
	PushList(unsigned int len) : Operation(OpCode::OP_PUSH_LIST), len(len)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_LIST " << len << "\n";
	}
};

struct PushDict final : public Operation
{
	unsigned int len;
	PushDict(unsigned int len) : Operation(OpCode::OP_PUSH_DICT), len(len)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_DICT " << len << "\n";
	}
};

struct PushSlice final : public Operation
{
	PushSlice() : Operation(OpCode::OP_PUSH_SLICE)
	{
	}
};

struct PushFunction final : public Operation
{
	std::string name;
	PushFunction(const std::string &name)
	    : Operation(OpCode::OP_PUSH_FUNCTION), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_FUNCTION " << name << "\n";
	}
};

struct Index final : public Operation
{
	Index() : Operation(OpCode::OP_INDEX)
	{
	}
};

struct Member final : public Operation
{
	Member() : Operation(OpCode::OP_MEMBER)
	{
	}
};

struct UnOp final : public Operation
{
	UnOp(OpCode code) : Operation(code)
	{
	}
};

struct BinOp final : public Operation
{
	BinOp(OpCode code) : Operation(code)
	{
	}
};

// namespace Pop
}

#endif // POP_OPCODES_HPP
