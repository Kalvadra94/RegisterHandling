/*
 * RegisterHandling.h
 *
 *  Created on: Mar 9, 2022
 *      Author: MyPC
 */

#ifndef REGISTERHANDLING_H_
#define REGISTERHANDLING_H_
#include <string>
#include <vector>

/*Use sub registers to split up register into specific groups.
 * A sub-register is defined by the length of the register and offset from LSB.
 *
 * The member functions allow the user to manipulate the sub-registers individually.
 *
 * For example, you are declaring a control register.
 * Bits 0-2 sets clock speed. Bit 3 sets rising falling edge. Bits 3-7 set communications.
 * Using sub-registers you can declare 3 separate entities for each and write them individually.
*/
struct sub_register
{
	//Member data
	int length;
	int offset;
	std::string name;

	//Constructor
	sub_register(std::string set_name, int set_length, int set_offset)
	{
		name = set_name;
		length = set_length;
		offset = set_offset;
	}
	//Destructor
	~sub_register()
	{
		delete &name;
		delete &length;
		delete &offset;
	}
};

class Register {
public:
	Register(int register_length, long int register_value, int reg_address);
	virtual ~Register();

	//Setters
	void setAddress(int register_address);//Set I2C address of target
	void setValue(long int register_value);//Set data

	//Getters
	int getLength();
	long int getValue();
	int getAddress();

	//Bit handling
	void setBit(int offset);//Set individual bit
	void clrBit(int offset);//Clear individual bit
	int getBit(int offset);//Return status of bit

	//Sub-register handling
	void add_subregister(std::string set_name, int set_length, int set_offset);//Add a new sub-register
	void setSubregister(std::string subreg_to_write, long int write_value);//Set the value of selected sub-register
	long int getSubregister(std::string subreg_to_read);//Retrieve data from selected sub-register

	//Printing register parameters
	void print_value();
	void print_inv_value();
	void print_address();
	void print_inv_address();
	void print_subregister();

private:
	//Member data
	int reg_length;//Register length in bits
	long int reg_value;//Up to 32 bits of data
	int reg_address;//Register I2C address
	int reg_addr_length;//Stores length of addresses - in I2C convention this has a maximum of 10, but is usually 7

	std::vector<sub_register> sub_registers;//Allows addition of sub-registers for easy and clear configuration
};

class Trim_Register:Register
{
public:
	//Constructors/Destructor
	Trim_Register(int set_register_length, long int set_register_value, int set_reg_address, double set_trim_target, double set_trim_weight, std::string set_filename);
	Trim_Register(int set_register_length, long int set_register_value, int set_reg_address, std::string set_filename);
	~Trim_Register();

private:

	double trim_target;
	double trim_weight;
	long int closest_trim;
	std::string filname;
};

#endif /* REGISTERHANDLING_H_ */
