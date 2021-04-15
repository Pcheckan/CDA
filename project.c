#include "spimcore.h"


// Helper Functions

int Check_valid_jump_multiple_four(unsigned PC)
{
	// Check to ensure we are at an address that is a multiple of 4
	if (PC % 4 == 0)
	{
		// if divisible, return 1
		return 1;
	}
		// if not divisible, return 0
	else
		return 0;
}


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
	// Using a switch statement to determine which operation takes place

	switch ((int) ALUControl)
	{
		// 000 | Z = A + B
		case 0:
			*ALUresult = A + B;
		break;
	
		// 001 | Z = A - B
		case 1:
			*ALUresult = A - B;
		break;

		// 010 | if A < B, Z = 1; Else Z = 0
		case 2:
			if ((signed) A < (signed) B) 
			*ALUresult = 1;

			else
			*ALUresult = 0;
		break;

		// 011 | If A < B, Z = 1; Else Z = 0 (A and B are unsigned)
		case 3:
			if ((unsigned) A < (unsigned) B)
			*ALUresult = 1;

			else
			*ALUresult = 0;
		break;

		// 100 | Z = A AND B
		case 4:
			*ALUresult = A & B;
		break;

		// 101 | Z = A OR B
		case 5:
			*ALUresult = A | B;
		break;

		// 110 | Shift left B by 16 bits
		case 6:
			*ALUresult = B << 16;
		break;

		// 111 | Z = NOT A
		case 7: 
			*ALUresult = ~A;
		break;
	}

	// Assign Zero to 1 if the result is zero; otherwise, assign 0. 

	// If the result is zero set Zero to 1
	if (*ALUresult == 0)
	{
		*Zero = 1;
	}
	// Otherwise set Zero to 0
	else
	{
		*Zero = 0;
	}

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{	
	unsigned memLocation = PC >> 2;

	// If the check returns false, we hit a halt condition.
	int check = Check_valid_jump_multiple_four(PC);
	if (check == 0)
		return 1;

	// Set the instruction
	*instruction = Mem[memLocation];
	return 0;


}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
	// Fill data1 with value from r1
	*data1 = Reg[r1];
	// Fill data2 with value from r2
	*data2 = Reg[r2];

}


/* Sign Extend */
//Michael Tsang
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	// read
	if (MemRead == 1)
	{
		// check to ensure mem address is valid (halt condition)
		if ((ALUresult % 4) == 0)
		{
			// read the content of mem location addressed by ALUResult to memdata
			*memdata = Mem[ALUresult >> 2];
		}
		// If we hit a halt condition, return 1
		else
			return 1;
	}

	// Writing
	if (MemWrite == 1)
	{
		// check halt condition
		if ((ALUresult % 4) == 0)
		{
			// write to the memory the value in data2
			Mem[ALUresult >> 2] = data2;
		}
		// If halt condition occurs, return 1
		else
			return 1;
	}

	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}


