
#ifndef RV32I_H
#define RV32I_H

#include <stdint.h>
#include <getopt.h>
#include "memory.h"

class rv32i
{
	public:
		memory * mem;
		rv32i(memory *m);
		void disasm(void);

	private:
        std::string decode(uint32_t insn) const;
		std::string render_lui(uint32_t insn) const;

		void dump() const;
		static uint32_t get_opcode(uint32_t insn);	//{ return (insn & 0x0000007f); }
		static uint32_t get_rd(uint32_t insn);		//{ return (insn & 0x00000f80) >> 7; }
		static uint32_t get_funct3(uint32_t insn);	//{ return (insn & 0x00007000) >> 12; }
		static uint32_t get_rs1(uint32_t insn);		//{ return (insn & 0x000f8000) >> 15; }
		static uint32_t get_rs2(uint32_t insn);		//{ return (insn & 0x01f00000) >> 20; }
		static uint32_t get_funct7(uint32_t insn);	//{ return (insn & 0xfe000000) >> 25; }

		static int32_t get_imm_i(uint32_t insn);
		static int32_t get_imm_u(uint32_t insn);
		static int32_t get_imm_b(uint32_t insn);
		static int32_t get_imm_s(uint32_t insn);
		static int32_t get_imm_j(uint32_t insn);

		static constexpr uint32_t XLEN = 32;
		static constexpr int mnemonic_width = 8;
		static constexpr int instruction_width = 35;

		std::string exec(uint32_t insn);
//		uint32_t		regs[32];
//		registerfile regs;
		bool halt = { false };
		uint64_t insn_counter = { 0 };
//		memory * mem;
		uint32_t pc = { 0 };
};
#endif

