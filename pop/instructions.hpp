// instructions.hpp - This file is part of Pop
// Copyright (c) 2016, Matthew Brush <mbrush@codebrainz.ca>
// All rights reserved.
// Licensed under the 2-clause BSD license, see LICENSE file.

#ifndef POP_INSTRUCTIONS_HPP
#define POP_INSTRUCTIONS_HPP

#include <pop/codebuffer.hpp>
#include <pop/error.hpp>
#include <pop/format.hpp>
#include <pop/opcodes.hpp>
#include <pop/token.hpp>
#include <pop/types.hpp>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace Pop
{

typedef std::unordered_map<std::string, CodeAddr> LabelMap;

struct Instruction
{
	OpCode code;
	CodeAddr addr;
	static constexpr size_t addr_size = sizeof(CodeAddr);

	virtual void list(std::ostream &out)
	{
		out << "\t" << name() << "\n";
	}

	virtual void dis(std::ostream &out) const
	{
		out << format("0x%08X:\t%s\n", addr, name());
	}

	virtual size_t size() const
	{
		return 1;
	}

	virtual void codegen(CodeBuffer &buf, LabelMap &) const
	{
		buf.put_u8(static_cast<Uint8>(code));
	}

	virtual void ccodegen(std::ostream &out) const
	{
		out << "\t" << name() << "();\n";
	}

	const char *name() const
	{
		return opcode_name(code);
	}

protected:
	Instruction(OpCode code, CodeAddr addr = CodeAddr(-1))
	    : code(code), addr(addr)
	{
	}
};

typedef std::unique_ptr<Instruction> InstructionPtr;
typedef std::vector<InstructionPtr> InstructionList;

// label isn't a real instruction, it's used as a placeholder until
// resolved to an address during assembly
struct Label final : public Instruction
{
	std::string name;
	Label(const std::string &name, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_LABEL, addr), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << name << ":\n";
	}
	virtual size_t size() const override final
	{
		return 0;
	}
	virtual void codegen(CodeBuffer &, LabelMap &) const override final
	{
		throw RuntimeError("cannot generate code for labels");
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << name << ":\n";
	}
};

struct Halt final : public Instruction
{
	Halt(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_HALT, addr)
	{
	}
};

struct Nop final : public Instruction
{
	Nop(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_NOP, addr)
	{
	}
};

struct OpenScope final : public Instruction
{
	OpenScope(CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_OPEN_SCOPE, addr)
	{
	}
};

struct CloseScope final : public Instruction
{
	CloseScope(CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_CLOSE_SCOPE, addr)
	{
	}
};

struct Bind final : public Instruction
{
	std::string name;
	Bind(const std::string &name, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_BIND, addr), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tBIND " << name << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tBIND %s\n", addr, name.c_str());
	}
	virtual size_t size() const override final
	{
		return 2 + name.size(); // opcode + length as byte + each byte
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_u8(name.size());
		for (auto ch : name)
			buf.put_u8(ch);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tBIND(" << name << ");\n";
	}
};

struct Call final : public Instruction
{
	Call(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_CALL, addr)
	{
	}
};

struct Return final : public Instruction
{
	Return(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_RETURN, addr)
	{
	}
};

struct Jump final : public Instruction
{
	std::string label;
	Jump(const std::string &label, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_JUMP, addr), label(label)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tJUMP " << label << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tJUMP %s\n", addr, label.c_str());
	}
	virtual size_t size() const override final
	{
		return 1 + sizeof(CodeAddr);
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_addr(labels[label]);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tJUMP(" << label << ");\n";
	}
};

struct JumpTrue final : public Instruction
{
	std::string label;
	JumpTrue(const std::string &label, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_JUMP_TRUE, addr), label(label)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tJUMP_TRUE " << label << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tJUMP_TRUE %s\n", addr, label.c_str());
	}
	virtual size_t size() const override final
	{
		return 1 + sizeof(CodeAddr);
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_addr(labels[label]);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tJUMP_TRUE(" << label << ");\n";
	}
};

struct JumpFalse final : public Instruction
{
	std::string label;
	JumpFalse(const std::string &label, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_JUMP_FALSE, addr), label(label)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tJUMP_FALSE " << label << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tJUMP_FALSE %s\n", addr, label.c_str());
	}
	virtual size_t size() const override final
	{
		return 1 + sizeof(CodeAddr);
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_addr(labels[label]);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tJUMP_FALSE(" << label << ");\n";
	}
};

struct PopTop final : public Instruction
{
	PopTop(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_POP_TOP, addr)
	{
	}
};

struct PushNull final : public Instruction
{
	PushNull(CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_NULL, addr)
	{
	}
};

struct PushTrue final : public Instruction
{
	PushTrue(CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_TRUE, addr)
	{
	}
};

struct PushFalse final : public Instruction
{
	PushFalse(CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_FALSE, addr)
	{
	}
};

struct PushInt final : public Instruction
{
	Uint64 value;
	PushInt(Uint64 value, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_INT, addr), value(value)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_INT " << value << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_INT %llu\n", addr, value);
	}
	virtual size_t size() const override final
	{
		return 1 + sizeof(Uint64);
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_u64(value);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_INT(" << value << "ULL);\n";
	}
};

struct PushFloat final : public Instruction
{
	double value;
	PushFloat(double value, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_FLOAT, addr), value(value)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_FLOAT " << value << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_FLOAT %Lg\n", addr, value);
	}
	virtual size_t size() const override final
	{
		return 1 + sizeof(double);
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_f64(value);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_FLOAT(" << value << "L);\n";
	}
};

struct PushString final : public Instruction
{
	std::string value;
	PushString(const std::string &value, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_STRING, addr), value(value)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_STRING \"" << value << "\"\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_STRING \"%s\"\n", addr, value.c_str());
	}
	virtual size_t size() const override final
	{
		return 5 + value.size(); // opcode + 4-byte length + each byte
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_string(value);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_STRING(\"" << value << "\");\n";
	}
};

struct PushSymbol final : public Instruction
{
	std::string name;
	PushSymbol(const std::string &name, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_SYMBOL, addr), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_SYMBOL " << name << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_SYMBOL %s\n", addr, name.c_str());
	}
	virtual size_t size() const override final
	{
		return 2 + name.size(); // opcode + length as byte + each byte
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_ident(name);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_SYMBOL(" << name << ");\n";
	}
};

struct PushList final : public Instruction
{
	Uint32 len;
	PushList(Uint32 len, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_LIST, addr), len(len)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_LIST " << len << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_LIST %u\n", addr, len);
	}
	virtual size_t size() const override final
	{
		return 5; // opcode + 4-byte length
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_u32(len);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_LIST(" << len << ");\n";
	}
};

struct PushDict final : public Instruction
{
	unsigned int len;
	PushDict(unsigned int len, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_DICT, addr), len(len)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_DICT " << len << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_DICT %u\n", addr, len);
	}
	virtual size_t size() const override final
	{
		return 5; // opcode + 4-byte length
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_u32(len);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_DICT(" << len << ");\n";
	}
};

struct PushSlice final : public Instruction
{
	PushSlice(CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_SLICE, addr)
	{
	}
};

struct PushFunction final : public Instruction
{
	std::string name;
	PushFunction(const std::string &name, CodeAddr addr = CodeAddr(-1))
	    : Instruction(OpCode::OP_PUSH_FUNCTION, addr), name(name)
	{
	}
	virtual void list(std::ostream &out) override final
	{
		out << "\tPUSH_FUNCTION " << name << "\n";
	}
	virtual void dis(std::ostream &out) const override final
	{
		out << format("0x%08X:\tPUSH_FUNCTION %s\n", addr, name.c_str());
	}
	virtual size_t size() const override final
	{
		return 5; // opcode + 4-byte address
	}
	virtual void codegen(CodeBuffer &buf, LabelMap &labels) const override final
	{
		Instruction::codegen(buf, labels);
		buf.put_addr(labels[name]);
	}
	virtual void ccodegen(std::ostream &out) const override final
	{
		out << "\tPUSH_FUNCTION(" << name << ");\n";
	}
};

struct Index final : public Instruction
{
	Index(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_INDEX, addr)
	{
	}
};

struct Member final : public Instruction
{
	Member(CodeAddr addr = CodeAddr(-1)) : Instruction(OpCode::OP_MEMBER, addr)
	{
	}
};

struct UnOp final : public Instruction
{
	UnOp(OpCode code, CodeAddr addr = CodeAddr(-1)) : Instruction(code, addr)
	{
	}
};

struct BinOp final : public Instruction
{
	BinOp(OpCode code, CodeAddr addr = CodeAddr(-1)) : Instruction(code, addr)
	{
	}
};

template <class T, class... Args>
static inline InstructionPtr mkop(Args &&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

// namespace Pop
}

#endif // POP_INSTRUCTIONS_HPP
