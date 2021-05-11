/****************************************************************
* Programmer: Cody McAntire                                     *
*																*
* Program:    Memory Simulator                  				*
*       		                                                *
* Summary:    This contains the functions for the memory class  *
****************************************************************/

#include "memory.h"
#include "hex.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <string>
#include <stdint.h>
#include <fstream>

/****************************************************************
* The memory constructor saves siz argument in the size member  *
* variable the allocates siz bytes for the mem array and        *
* initializes every byte to 0xa5			       				*
*							        							*
* @param siz is an unsigned int that is a 32bit member var      *
*																*
* @return memory constructor returns nothing					*
****************************************************************/
memory::memory(uint32_t siz)
{
	siz = (siz+15)&0xfffffff0; //may use this to round the length up, mod-16
	size = siz;
	mem = new uint8_t[siz];

	for(int i = 0; i < siz; i++) //allocate siz bytes for the mem array
	{
		mem[i] = 0xa5; //initialize each byte to 0xa5
	}
}

/****************************************************************
* The memory destructors frees memory that was allocated in the *
* constructor to represent the simulated memory					*
*                                                               *
* @param has no params											*
*                                                               *
* @return returns nothing										*
****************************************************************/
memory::~memory()
{
	delete mem; //free memory allocated in the constructor
}

/****************************************************************
* Function check_address returns true if given address is in	*
* the simulated memory. If not, it shows user a warning and		*
* returns false.												*
*                                                               *
* @param an unsigned int i 										*
*                                                               *
* @return check address returns bool true or false accordingly	*
****************************************************************/
bool memory::check_address(uint32_t i) const
{
	if(i < size)
		return true;
	else
                std::cout << "WARNING: Address out of range: " << hex0x32(i) << std::endl;
		return false;
}

/****************************************************************
* Function get_size returns the siz value						*
*                                                               *
* @param none													*
*                                                               *
* @return size which is a unsigned int							*
****************************************************************/
uint32_t memory::get_size() const
{
	return size; //should return (possibly rounded up) siz value
}

/****************************************************************
* Function get8 checks if given addr is in mem by calling		*
* check_address. returns value of byte in simulated memory at	*
* appropriate address if addr is in valid range.				*
*                                                               *
* @param addr which is the byte value of an address 			*
*                                                               *
* @return addr which is an unsigned int or 0 if invalid address *
****************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
	if(!check_address(addr))  //if addr is not in valid range
		return 0; //return 0 if not in valid range
	else
		return mem[addr]; //return value of byte from sim memory
}

/****************************************************************
* Function get16 calls get8 function twice to get two bytes and	*
* combine them in little-endian order to create a 16 bit return	*
* value.														*
*                                                               *
* @param addr which represents bits of an address				*
*                                                               *
* @return a 16 bit unsigned int									*
****************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
	return get8(addr) | (get8(addr+1)<<8);
}

/****************************************************************
* Function get32 calls get16 twice and combines results in		*
* litte-endian order similar to get16							*
*                                                               *
* @param addr, bytes of address									*
*                                                               *
* @return 32 bit unsigned integer								*
****************************************************************/
uint32_t memory::get32(uint32_t addr) const
{
	return get16(addr) | get16(addr+2)<<16;
}

/****************************************************************
* Function set8 calls checks addres to verify if addr argument	*
* is valid. If it is, byte in simulated memory at that address	*
* is set to the given val. If not, discard the data. 			*
*                                                               *
* @param 32bit unsigned int addr								*
*	 8bit unsigned int val										*
*                                                               *
* @return void													*
****************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
	if (!check_address(addr)) //verify addr argument is valid
		return;
	else
		mem[addr] = val; //set byte in mem at the addr to val
}

/****************************************************************
* Function set16 calls set8 twice to store given val in little-	*
* endian order into simulated memory starting at address given	*
* in the addr argument											*
*                                                               *
* @param 32bit unsigned int addr								*
* 	 16bit unisnged int val										*
*                                                               *
* @return void													*
****************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
	set8(addr, val&0x0ff);
	set8(addr +1, val >> 8);
}

/****************************************************************
* Function set32 calls set16 twice to store given val in little-*
* endian order into the simulated memory starting at address	*
* given in the addr argument									*
*                                                               *
* @param 32bit unisgned int addr								*
*        32bit unsigned int val									*
*                                                               *
* @return void													*
****************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
	set16(addr, val&0x0ffff);
	set16(addr+2, val >> 16);
}

/****************************************************************
* Function dump dumps entire contents of simulated memory in hex*
* with ASCII on the right. Prints out the virtual memory		*
*                                                               *
* @param none													*
*                                                               *
* @return void													*
****************************************************************/
void memory::dump() const
{
	char a[17]; ///< a is a 17 byte ascii character array
	a[16] = 0;  // used to collect the 16print characters

	for (uint32_t i = 0; i<size; i++) //loop to fortmat and print dump
	{
		if (i%16 == 0)
		{
			if (i != 0)
			{
				std::cout << " *" << a << "*" << std::endl;
			}
		std::cout << hex32(i) << ":";
		}
		uint8_t ch=get8(i);
                std::cout << (i%16==8?"  ":" ") << hex8(ch);
		a[i%16] = isprint(ch)?ch:'.';
	}
	std::cout << " *" << a << "*" << std::endl;
}

/****************************************************************
* Function load_file opens file named fname in binary mode and	*
* reads its contents into the simulated memory					*
*                                                               *
* @param file fname which is a string							*
*                                                               *
* @return boolean value. false when theres errors otherwise true*
****************************************************************/
bool memory::load_file(const std::string &fname)
{
	std::ifstream infile(fname, std::ios::in|std::ios::binary); //open file to read in binary

	if (!infile)
	{
		std::cerr << "can not open file" << fname << " for reading" << std::endl;
		return false;
	}

	uint8_t i;
	uint32_t addr = 0;

	while(infile.read((char*)&i, 1))
	{
		if (addr>=size)
		{
			std::cerr<<"Program too big" << std::endl;
			return false;
                }
		set8(addr, i);
		++addr;
	}
	return true;
}
