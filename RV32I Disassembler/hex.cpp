/****************************************************************
* Programmer: Cody McAntire                                     *
*                                                               *
* Summary:    This program loads and disassembles a binary      *
*             RV32I executable file.                            *
****************************************************************/

#include "hex.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ostream>
#include <stdint.h>

/****************************************************************
* Function hex8 formats and prints the hex number 		*
*                                                               *
* @param i which is an 8bit unsighned int			*
*                                                               *
* @return string						*
****************************************************************/
std::string hex8(uint8_t i)
{
        std::ostringstream os;
        os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
        return os.str();
}

/****************************************************************
* Function hex32 formats/prints the hex number with all 32bits  *
*                                                               *
* @param i which is a 32bit unsigned int                        *
*                                                               *
* @return string                                                *
****************************************************************/
std::string hex32(uint32_t i)
{

        std::ostringstream os;
        os << std::hex << std::setfill('0') << std::setw(8) << i;
        return os.str();
}

/****************************************************************
* Function hex0x32 formats/prints the hex number with all 32bits*
* with a preceding 0x for correct format			*
*                                                               *
* @param i which is a 32bit unsigned int                        *
*                                                               *
* @return string                                                *
****************************************************************/
std::string hex0x32(uint32_t i)
{
        return std::string("0x")+hex32(i);
}


