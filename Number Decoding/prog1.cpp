/***********************************************************************
*								      *
*  Author: Cody McAntire					      *
*								      *
*  Bitwise Operators & IEEE-754 Floating Point Number Decoding        *
*								      *
*  This program reads in one or more 32-bit HEX representations of    *
*  an IEEE-754 Floating Point Number, decodes them by extracting the  *
*  sign, exponent, and significand, and prints their compenents.      *
**********************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <ostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/****************************************************************************
* Function printBinFloat uses bitwise operators to extract and shift the    *
* fields of the IEEE numbers as needed to render the output. 		    *
* 									    *
* @param x is an unsigned int that is read in as a 32-bit hexadecimal value *
*									    *
* @return x which contains the input from user				    *
****************************************************************************/
uint32_t  printBinFloat(uint32_t x)
{

	uint32_t sig; ///< sig is a unsigned int that represents the significand.
	int32_t ex;   ///< ex is a signed int that represents the exponent.

        ex = ((x & 0x7f800000) >> 23) -0x7F;  //exponent is found from bits 30-23 minus 127 (0x7F)
        sig =  ( x & 0x007fffff);  //sig is 32bit unsigned hex value. bits 22-0

        // Print the first line of output: thehexadecimal value of the input = to its binary value
        std::cout << std::hex << "0x" << std::setw(8) << std::setfill('0') << x; //prints hex value first
        std::cout << " = ";
        for(int i = 31; i > -1; i--) //loop backwards to get the right bits from hex number for binary number
        {
		std::cout << (i<31 && (i%4)==3 ? " " : "") << (x & 1 << i ? '1' : '0'); //print hex number in binary and a space after every 4 bits
        }
        std::cout << std::endl;

        //print second line: sign bit
        std::cout << "sign: " << (x &0x80000000 ? '1' : '0') << std::endl; //if LSB is 1 floating point positive. 0 negative. 0x80 is 31st bit

        //print third line: exponent as 32 bit signed int and as its (decimal value)
        std::cout << " exp: 0x" << std::setw(8) << ex; //prints exponent
        //print the decimal value
        std::cout << " (" << std::dec << ex << ")" << std::hex << std::endl; //converts hex to dec and prints

        //print fourth line: significand as 32 bit unsigned hex value
        std::cout << " sig: 0x" << std::setw(8) << std::setfill('0') << sig << std::endl; //sig is bits 22-0

        //print the fifth line: the full value of the number in binary
        std::cout << (x &0x80000000 ? '-' : '+'); //prints '-' if sign is negative and '+' if sign is positive
        //special case: inf
        if (ex == 128 && sig == 00000000) //if exponent is all ones AND significand is all zeros
        {
                std::cout << "inf" << std::endl;
        }
        //special case: 0
        else if (ex == -127 && sig == 00000000) //0 when exponent is -127 and sig is all zeros
        {
                std::cout << "0" << std::endl;
        }
        //print binary float when exponent is positive
	else if ((int)ex >= 0)  //loop what exponent is positive which includes 0
        {

                int digits = (int)ex + 23;; //initialize digits to be printed
		std::cout << '1' ; //one always leads sig
		sig = sig << 9;
                for (int i = 0; i < digits; i++) //loop until amount of digits to print is reached
		{
                        if (i == (int)ex) //once i = exponent
                        {
                                std::cout << '.'; //insert a '.'
                        }


			std::cout << ((sig & 0x80000000) ? '1' : '0');//check if bit of sig is 1 and print accordingly
			sig = sig << 1; //shift to the left and let loop repeat till all numbers printed

                        if(i >= (int)ex && i > 23) //if value of i is greater or equal to exp and sig size
                        {
                                break; //break to avoid printing insignificant zeros beyond decimal
                        }

                }
        std::cout << std::endl;
        }
        //print binary float when exponent is negative
        else if (ex < 0)
        {
                std::cout << "0."; //start with zero and decimal

                sig = sig << 9; //shift so bits that make up sig are the first bits in sig.u

                //store the zeros that come before the significand
                int leadingZeros; //initialize zeros to be printed
                leadingZeros = abs(ex) - 1  ; //absolute value of exponent - 1

                for(int i = 0; i < leadingZeros; i++) //loop to print correct amount of leading zeros
                {
                        std::cout << '0'; //print out leading zeros
                }
                std::cout << '1'; //print the leading 1
                for(int i = 0; i < 23; i++) //loop through size of the significand
                {

                        if   (sig & 0x80000000) //if ith bit of sig is set
                                std::cout << '1';
                        else
                                std::cout << '0';

                        sig = sig << 1; //iterate through significand
                }
	std::cout << std::endl;
        }
	return x;
}


/**********************************************************
*  Function main has a read loop that calls printBinFloat *
*  to do the decoding and printing                        *
*                                                         *
*  @param main does not have any parameters               *
*							  *
*  @return main does not return anything		  *
**********************************************************/
int main()
{
	uint32_t x; ///< x is a 32-bit hexadecimal value to be read in.
        while(std::cin >> std::hex >> x) //read loop
        {
                printBinFloat(x); //call printbinfloat to do the decoding and printing

        }
}
