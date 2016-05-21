#include <pop/disassembler.hpp>
#include <pop/format.hpp>
#include <pop/opcodes.hpp>
#include <string>
#include <sstream>

namespace Pop
{

struct ByteCodeReader
{
	std::istream &inp;

	ByteCodeReader(std::istream &inp) : inp(inp)
	{
	}

	Uint8 read_u8()
	{
		return static_cast<Uint8>(inp.get());
	}

	Uint16 read_u16()
	{
		Uint16 v = 0;
		v |= Uint16(read_u8()) << 8;
		v |= Uint16(read_u8());
		return v;
	}

	Uint32 read_u32()
	{
		Uint32 v = 0;
		v |= Uint32(read_u8()) << 24;
		v |= Uint32(read_u8()) << 16;
		v |= Uint32(read_u8()) << 8;
		v |= Uint32(read_u8());
		return v;
	}

	Uint64 read_u64()
	{
		Uint64 v = 0;
		v |= Uint64(read_u8()) << 56ull;
		v |= Uint64(read_u8()) << 48ull;
		v |= Uint64(read_u8()) << 40ull;
		v |= Uint64(read_u8()) << 32ull;
		v |= Uint64(read_u8()) << 24ull;
		v |= Uint64(read_u8()) << 16ull;
		v |= Uint64(read_u8()) << 8ull;
		v |= Uint64(read_u8());
		return v;
	}

	Float32 read_f32()
	{
		union
		{
			Uint32 i;
			Float32 f;
		} v;
		v.i = read_u32();
		return v.f;
	}

	Float64 read_f64()
	{
		union
		{
			Uint64 i;
			Float64 f;
		} v;
		v.i = read_u64();
		return v.f;
	}

	CodeAddr read_addr()
	{
		if (sizeof(CodeAddr) == 4)
			return read_u32();
		else
			return read_u64();
	}

	std::string read_string()
	{
		Uint32 len = read_u32();
		std::string s;
		s.reserve(len);
		for (Uint32 i = 0; i < len; i++)
			s += char(read_u8());
		return s;
	}

	std::string read_name()
	{
		Uint8 len = read_u8();
		std::string s;
		s.reserve(len);
		for (Uint8 i = 0; i < len; i++)
			s += char(read_u8());
		return s;
	}

	OpCode read_op()
	{
		return static_cast<OpCode>(read_u8());
	}
};

static inline std::string format_addr(CodeAddr addr)
{
	return format("0x%08X", addr);
}

void disassemble(std::istream &inp, InstructionList &out)
{
	ByteCodeReader reader(inp);
	bool done = false;
	while (!done)
	{
		CodeAddr addr = inp.tellg();
		auto op = reader.read_op();
		switch (op)
		{
			case OpCode::OP_HALT:
				out.push_back(mkop<Halt>(addr));
				done = true;
				break;
			case OpCode::OP_NOP:
				out.push_back(mkop<Nop>(addr));
				break;
			case OpCode::OP_OPEN_SCOPE:
				out.push_back(mkop<OpenScope>(addr));
				break;
			case OpCode::OP_CLOSE_SCOPE:
				out.push_back(mkop<CloseScope>(addr));
				break;
			case OpCode::OP_BIND:
				out.push_back(mkop<Bind>(reader.read_name(), addr));
				break;
			case OpCode::OP_CALL:
				out.push_back(mkop<Call>(addr));
				break;
			case OpCode::OP_RETURN:
				out.push_back(mkop<Return>(addr));
				break;
			case OpCode::OP_JUMP:
				out.push_back(
				    mkop<Jump>(format_addr(reader.read_addr()), addr));
				break;
			case OpCode::OP_JUMP_TRUE:
				out.push_back(
				    mkop<JumpTrue>(format_addr(reader.read_addr()), addr));
				break;
			case OpCode::OP_JUMP_FALSE:
				out.push_back(
				    mkop<JumpFalse>(format_addr(reader.read_addr()), addr));
				break;
			case OpCode::OP_POP_TOP:
				out.push_back(mkop<PopTop>(addr));
				break;
			case OpCode::OP_PUSH_NULL:
				out.push_back(mkop<PushNull>(addr));
				break;
			case OpCode::OP_PUSH_TRUE:
				out.push_back(mkop<PushTrue>(addr));
				break;
			case OpCode::OP_PUSH_FALSE:
				out.push_back(mkop<PushFalse>(addr));
				break;
			case OpCode::OP_PUSH_INT:
				out.push_back(mkop<PushInt>(reader.read_u64(), addr));
				break;
			case OpCode::OP_PUSH_FLOAT:
				out.push_back(mkop<PushFloat>(reader.read_f64(), addr));
				break;
			case OpCode::OP_PUSH_STRING:
				out.push_back(mkop<PushString>(reader.read_string(), addr));
				break;
			case OpCode::OP_PUSH_SYMBOL:
				out.push_back(mkop<PushSymbol>(reader.read_name(), addr));
				break;
			case OpCode::OP_PUSH_LIST:
				out.push_back(mkop<PushList>(reader.read_u32(), addr));
				break;
			case OpCode::OP_PUSH_DICT:
				out.push_back(mkop<PushDict>(reader.read_u32(), addr));
				break;
			case OpCode::OP_PUSH_SLICE:
				out.push_back(mkop<PushSlice>(addr));
				break;
			case OpCode::OP_PUSH_FUNCTION:
				out.push_back(
				    mkop<PushFunction>(format_addr(reader.read_addr()), addr));
				break;
			case OpCode::OP_INDEX:
				out.push_back(mkop<Index>(addr));
				break;
			case OpCode::OP_MEMBER:
				out.push_back(mkop<Member>(addr));
				break;
			case OpCode::OP_ADD:
			case OpCode::OP_SUB:
			case OpCode::OP_MUL:
			case OpCode::OP_DIV:
			case OpCode::OP_MOD:
			case OpCode::OP_POW:
			case OpCode::OP_LOG_AND:
			case OpCode::OP_LOG_OR:
			case OpCode::OP_BIT_AND:
			case OpCode::OP_BIT_OR:
			case OpCode::OP_BIT_XOR:
			case OpCode::OP_LEFT_SHIFT:
			case OpCode::OP_RIGHT_SHIFT:
			case OpCode::OP_IP_ADD:
			case OpCode::OP_IP_SUB:
			case OpCode::OP_IP_MUL:
			case OpCode::OP_IP_DIV:
			case OpCode::OP_IP_MOD:
			case OpCode::OP_IP_AND:
			case OpCode::OP_IP_OR:
			case OpCode::OP_IP_XOR:
			case OpCode::OP_IP_NOT:
			case OpCode::OP_IP_LEFT:
			case OpCode::OP_IP_RIGHT:
			case OpCode::OP_IP_ASSIGN:
			case OpCode::OP_IP_PREINC:
			case OpCode::OP_IP_PREDEC:
			case OpCode::OP_IP_POSTINC:
			case OpCode::OP_IP_POSTDEC:
			case OpCode::OP_EQ:
			case OpCode::OP_NE:
			case OpCode::OP_GT:
			case OpCode::OP_GE:
			case OpCode::OP_LT:
			case OpCode::OP_LE:
				out.push_back(mkop<BinOp>(op, addr));
				break;
			case OpCode::OP_POS:
			case OpCode::OP_NEG:
			case OpCode::OP_LOG_NOT:
			case OpCode::OP_BIT_NOT:
				out.push_back(mkop<UnOp>(op, addr));
				break;
			default:
				break;
		}
	}
}

// namespace Pop
}
