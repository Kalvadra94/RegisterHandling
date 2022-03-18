/*
 * RegisterHandling.cpp
 *
 *  Created on: Mar 9, 2022
 *      Author: MyPC
 */

#include "RegisterHandling.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

//Constructor/Destructor
Register::Register(int register_length,long int register_value, int register_address)
{
	reg_length = register_length;
	reg_value = register_value;
	reg_address = register_address;
	reg_addr_length = 10;
}

Register::~Register() {
	delete &reg_length;
	delete &reg_value;
	delete &reg_address;
}

//Setters
void Register::setAddress(int register_address)
{
	reg_address = register_address;
	return;
}

void Register::setValue(long int register_value)
{
	reg_value = register_value;
	return;
}

//Getters
int Register::getLength()
{
	return reg_length;
}

long int Register::getValue()
{
	return reg_value;
}

int Register::getAddress()
{
	return reg_address;
}

//Print Functions
void Register::print_value()
{
	cout << "Register Value ";
	for(int i = 0;i<reg_length;i++)
	{
		cout << ((reg_value>>i)&1);
	}
	cout << "\n";
	return;
}

void Register::print_inv_value()
{
	cout << "Register Value ";
	for(int i = reg_length;i>0;i--)
	{
		cout << ((reg_value>>(i-1))&1);
	}
	cout << "\n";
	return;
}

void Register::print_address()
{
	cout << "Address Value ";
	for(int i = 0;i<reg_addr_length;i++)
	{
		cout << ((reg_address>>i)&1);
	}
	cout << "\n";
	return;
}

void Register::print_inv_address()
{
	cout << "Address Value ";
	for(int i = 16;i>0;i--)
	{
		cout << ((reg_address>>(i-1))&1);
	}
	cout << "\n";
	return;
}

//Sub-register Operators
void Register::add_subregister(std::string set_name, int set_length, int set_offset)
{
	sub_register temp_sub_reg(set_name,set_length,set_offset);
	sub_registers.push_back(temp_sub_reg);
	delete &temp_sub_reg;
	return;
}

void Register::setSubregister(std::string subreg_to_write, long int write_value)
{
	long int write_mask = 0;//Set to all 0s
	int target_subreg = -1;

	//Find sub-register in structure with matching name
	for(int j = 0; j<sub_registers.size();j++)
	{
		if(sub_registers[j].name.compare(subreg_to_write)==0)
			target_subreg = j;
	}

	//Error handling in case no matching name is found
	if(target_subreg == -1)
	{
		cout << "Target sub-register not found!";
		return;
	}

	//Create a mask for desired bits - set 1s to the write mask
	for(int i = 0; i<sub_registers[target_subreg].length;i++)
	{
		write_mask =  write_mask|(1<<i);
	}

	//Clear bits in sub-register
	setValue(getValue()&~(write_mask<<sub_registers[target_subreg].offset));

	//Set bits in sub-register to data
	setValue(getValue()|(write_value<<sub_registers[target_subreg].offset));

	return;
}

long int Register::getSubregister(std::string subreg_to_read)
{
	long int read_mask = 0;
	int target_subreg = -1;

	//Find sub-register in structure with matching name
	for(int j = 0; j<sub_registers.size();j++)
	{
		if(sub_registers[j].name.compare(subreg_to_read)==0)
			target_subreg = j;
	}

	//Error handling for no-match case
	if(target_subreg == -1)
	{
		cout << "Target sub-register not found!";
		return -1;
	}

	//Create a mask for desired bits - write a 1 for every bit in the subregister
	for(int i = 0; i<sub_registers[target_subreg].length;i++)
	{
		read_mask =  read_mask|(1<<i);
	}
	//Copy number of bits shifted by subregister offset onto read_mask
	read_mask=getValue()&(read_mask<<sub_registers[target_subreg].offset);

	//Report value with leading zeros removed
	return ( (read_mask>>sub_registers[target_subreg].offset));
}

//Bit Operators
void Register::setBit(int offset)
{
	setValue(getValue()|(1<<offset));
	return;
}

void Register::clrBit(int offset)
{
	setValue(getValue()&(~(1<<offset)));
	return;
}

int Register::getBit(int offset)
{
	return((getValue()>>offset)&1);
}
