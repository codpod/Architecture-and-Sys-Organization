/****************************************************************
* Programmer: Cody McAntire                                     *
*                                                               *
* Summary:    this is the header file for hex that calls the	*
*	          functions in hex.cpp	        	                *
****************************************************************/

#ifndef hex_H
#define hex_H

#include <cstdint>
#include <stdint.h>
#include <string>

std::string hex8(uint8_t i);
std::string hex32(uint32_t i);
std::string hex0x32(uint32_t i);

#endif

