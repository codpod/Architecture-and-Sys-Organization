/****************************************************************
* Programmer: Cody McAntire                                     *
*                                                               *
* Summary:    This program loads and disassembles a binary      *
*             RV32I executable file.                            *
****************************************************************/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>

#include "rv32i.h"

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ios;

//save the m argument in the mem member variable for use later when disassembling
rv32i::rv32i(memory *m)
{
	mem = m;
}


//extract and return the opcode field from the given instruction
uint32_t rv32i::get_opcode(uint32_t insn)
{
	return insn & 0x7f;
}

//extract and return the rd field from the given instruction
uint32_t rv32i::get_rd(uint32_t insn)
{
	return(insn & 0xf80)>>(7-0); //return rd and shift over 7 bits
}

//extract and return the funct3 field from the given instruction
uint32_t rv32i::get_funct3(uint32_t insn)
{
	return(insn & 0x7000) >>(12-0); //return funct3 and shift over 12 bits
}

//extract and return the rs1 field from the given instruction
uint32_t rv32i::get_rs1(uint32_t insn)
{
	return(insn & 0xf8000) >> (15-0); //return rs1 and shift over 15 bits
}

//extract and return the rs2 field from the given instruction
uint32_t rv32i::get_rs2(uint32_t insn)
{
	return(insn & 0x1f00000) >> (20-0); //return rs2 and shift over 20 bits
}

//extract and return the funct7 field from the given instruction
uint32_t rv32i::get_funct7(uint32_t insn)
{
	return(insn & 0xfe000000) >> (25-0); //return funct7 and shift over 25 bits
}

//extract and return the imm_i field from the given instruction
int32_t rv32i::get_imm_i(uint32_t insn)
{
	int32_t imm_i = (insn & 0xfff00000) >> (20-0); //extract imm_t and shift over 20 bits

	if(imm_i & 0x800) //sign extend
		imm_i |= 0xfffff000;

	return imm_i;
}

//extract and return the imm_u field from the given instruction
int32_t rv32i::get_imm_u(uint32_t insn)
{
	int32_t imm_u = (insn & 0xfffff000);

	if(insn& 0x80000000) //if msb
		imm_u |= 0xfffff000;

	return imm_u;
}

//extract and return the imm_b field from the given instruction
int32_t rv32i::get_imm_b(uint32_t insn)
{
	int32_t imm_b = (insn & 0x80000000) >> (31-12); //move the msb to 12
	imm_b |= (insn & 0x7e000000) >> (25-5); //move the 5 bits next to msb to 5-10
	imm_b |= (insn & 0xf00) >> (8-1); //moves uvwx to 1-4
	imm_b |= (insn & 0x80) << (7-4); //moves y to 11

	if(insn& 0x80000000)
		imm_b |= 0x1ffe; //maybe 0xffffe000 ???

	return imm_b;
}

//extract and return the imm_s field from the given instruction
int32_t rv32i::get_imm_s(uint32_t insn)
{
	int32_t imm_s = (insn& 0xfe000000) >>(25-5); //extract left most bunch of bits and shift bits to 5-11
	imm_s |= (insn& 0x00000f80) >> (7-0); //extract and shift bits 0-4

	if(insn& 0x80000000) //sign - extend
		imm_s |= 0xfffff000;

	return imm_s;
}

//extract and return the imm_j field from the given instruction
int32_t rv32i::get_imm_j(uint32_t insn)
{
	int32_t imm_j = insn & (insn & 0x80000000) >> (31-20); //move msb to 20
	imm_j |= (insn & 0x7fe00000) >> (21-1); //move the group of 10 bits to 1-10
	imm_j |= (insn & 0x100000) >> (20-11); //move bit in 20 to 11
	imm_j |= (insn & 0xff000);

	if(insn& 0x80000000)
		imm_j |= 0x1ffffe;

	return imm_j;


}

//return a std::string containing the hex instruction value and disassembled instruction text
std::string rv32i::decode(uint32_t insn) const
{
	
	std::ostringstream os;
	os << std::hex << (insn) << "  "; //the instruction hex value
	
	switch(get_opcode(insn))
	{
	//set register rd to the imm_u value in figure
	case 0b0110111: //U-type LUI
		os << "lui    x" << std::dec << get_rd(insn) << ",0x" << std::hex
		   << ((get_imm_u(insn) >> 12) &0x0fffff);
		break;
	

	//add address of instruction to the imm_u as shown in figure and store result in register rd
	case 0b0010111: //U-type auipc
		os << "auipc  x" << std::dec << get_rd(insn) << ",0x" << std::hex << ((get_imm_u(insn) >> 12) &0x0fffff);
		break;

	//set rd to address of jalr instruction +4 then jump to address given by sum of rs1 register
	//and the imm_i value as decoded from the instruction show in figure
	case 0b1101111: //J-type jal
		os << "jal    x" << std::dec << get_rd(insn) << ",0x" << std::hex << pc+get_imm_j(insn);
		break;

	case 0b1100111: // I-type jalr
		os << "jalr   x" << std::dec << get_rd(insn) << "," << std::dec<<get_imm_i(insn) << "(x" << get_rs1(insn) << ")";
		break;

	case 0b1100011: // B-type store instructions
		switch (get_funct3(insn))
		{
		case 0b000: os<< "beq    "; //beq
		break;
                case 0b001: os<< "bne    "; //bne
		break;
                case 0b100: os<< "blt    "; //blt
		break;
                case 0b101: os<< "bge    "; //bge
		break;
                case 0b110: os<< "bltu   "; //bltu
		break;
                case 0b111: os<< "bgeu   "; //bgeu
		break;
		default:
			os << "unhandled funct3";
			return os.str();
		}
		os << "x"; os << std::dec << get_rs1(insn) << ",x" << get_rs2(insn) << ",0x"<< std::hex << pc+get_imm_b(insn);
		break;

	case 0b0000011: //I-type store instruction
		switch (get_funct3(insn))
		{
		case 0b000: os<< "lb     "; //lb
		break;
                case 0b001: os<< "lh     "; //lb
                break;
                case 0b010: os<< "lw     "; //lw
                break;
                case 0b100: os<< "lbu    "; //lbu
                break;
                case 0b101: os<< "lhu    "; //lhu
                break;
		default:
			os << "ERROR";
			return os.str();
		}
		os << "x" << std::dec << get_rd(insn) << "," << get_imm_i(insn) << "(x" << get_rs1(insn) << ")";
		break;

	case 0b0100011: //S-type store instructions
		switch(get_funct3(insn))
		{
		case 0b000: os << "sb     "; break; //sb
		case 0b001: os << "sh     "; break; //sh
		case 0b010: os << "sw     "; break; //sw
		default:
			os << "ERROR";
			return os.str();
		}
		os << "x"; os << std::dec << get_rs2(insn) << "," << get_imm_s(insn) << "(x"
		   << get_rs1(insn) << ")";
		break;

	case 0b0010011: //more I-type instructions (alu)
		{
			int32_t imm_i = get_imm_i(insn);
			switch (get_funct3(insn))
			{
			case 0b000: os << "addi   ";
			break;
                        case 0b010: os << "slti   ";
                        break;
                        case 0b011: os << "sltiu  ";
                        break;
                        case 0b100: os << "xori   ";
                        break;
                        case 0b110: os << "ori    ";
                        break;
                        case 0b111: os << "andi   ";
                        break;
                        case 0b001: os << "slli   ";
                        break;
			case 0b101: //srai srli case. have to shift right
				imm_i = imm_i%XLEN;
				if (get_funct7(insn) == 0b0100000)
					os << "srai   ";
				else
					os << "srli   ";
				break;
			default:
				os << "ERROR";
				return os.str();
			}
			os << "x" << std::dec<< get_rd(insn) << ",x"; os << get_rs1(insn) << "," << imm_i;
		}
		break;

	case 0b0110011: //R-type instructions
		switch(get_funct7(insn))
		{
                case 0b000:// rd = rs1 + rs2; //add set rs1+rs2 and sub set reg rd to rs1-rs2
			if(get_funct7(insn)&0b0100000)
				os << "sub    ";
			else
				os << "add    ";
			break;
                case 0b111: //and: set register rd to the bitwisers1 + rs2
			os << "and    ";
			break;
                case 0b110: //or: set red rd to the bitwise or of rs1 and rs2
			os << "or     ";
			break;
		case 0b001: //sll: shift rs1 left by number of bits specified in lease sig 5 bits of rs2 and store result in rd
                        os << "sll    ";
                        break;
		case 0b010: //slt: if signed in val in rs1 < signed in val in rs2 then set rd to 1. otherwise 0
                        os << "slt    ";
                        break;
		case 0b011: //sltu if unsigned int val in rs1 < unsigned in val in rs2 then set rd to 1. otherwise 0
                        os << "sltu   ";
                        break;
		case 0b101: //sra: arithmetic shift rs1 right by number of bits given in rs 2 and store result in rd and srl: logic shift rs1 right by number of bits given in rs2 and store result in rd
			if(get_funct7(insn)&0b0100000)
				os << "sra    ";
			else
				os << "srl    ";
			break;
		case 0b100: //xor: set register rd to the bitwise xor of rs1 and rs2
			os << "xor    ";
			break;
		}
		os << "x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << ",x" << get_rs2(insn);
		break;

	case 0b0001111: //fence
		os << "fence  "
		   << (insn & 0x08000000 ? "i" : "")
                   << (insn & 0x04000000 ? "o" : "")
                   << (insn & 0x02000000 ? "r" : "")
                   << (insn & 0x01000000 ? "w" : "")
		   << ","
                   << (insn & 0x00800000 ? "i" : "")
                   << (insn & 0x00400000 ? "o" : "")
                   << (insn & 0x00200000 ? "r" : "")
                   << (insn & 0x00100000 ? "w" : "");
		break;

	case 0b1110011: //ecall and ebreak
		if(get_imm_i(insn) & 0x01)
			os << "ebreak    ";
		else
			os << "ecall    ";
		break;

	default:
		os<< "ERROR";
	}
	return os.str();
}



//execute given rv32i instruction word
std::string rv32i::exec(uint32_t insn)
{
	pc += 4;
	return "ERROR: UNIMPLEMENTED INSTRUCTION";
}



void rv32i::dump() const
{
	//dump();
	std::cout << " pc " << std::hex << (pc);
	std::cout << std::endl;
}



void rv32i::disasm()
{
//	regs.set(2, mem->get_size());
	uint32_t limit;

	while(insn_counter < limit && !halt)
	{
		dump();
		uint32_t insn = mem -> get32(pc);
		std:: cout << std::hex << (pc) << ": ";
		std:: cout << std:: setw(35) << std:: setfill(' ') << std:: left << decode (insn) << " // ";
		std:: cout << exec(insn) << std:: endl;
		++insn_counter;
	}
	std:: cout << insn_counter << "instruction executed."; std:: cout<< endl;
	dump();
} 

