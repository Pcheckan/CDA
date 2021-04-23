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
	*op     = (instruction & 0xfc000000) >>26;	// [31-26]
	*r1     = (instruction & 0x3e00000) >> 21;	// [25-21]
	*r2     = (instruction & 0x1f0000) >> 16;	// [20-16]
	*r3     = (instruction & 0xf800) >> 11;		// [15-11]
	*funct  = (instruction & 0x1f);				// [5-0]
	*offset = (instruction & 0xffff);			// [15-0]
	*jsec   = (instruction & 0x3ffffff);		// [25-0]
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
	switch (op)
	{
		case 0://000000;R-type instruction
		{
			controls->RegDst   	= 1;
			controls->Jump     	= 0; 
			controls->Branch   	= 0; 
			controls->MemRead  	= 0; 
			controls->MemtoReg 	= 0; 
			controls->ALUOp    	= 7;// 111
			controls->MemWrite 	= 0; 
			controls->ALUSrc   	= 0; 
			controls->RegWrite 	= 1;
			break;
		}
		case 2://000010; "j" jump
		{
			controls->RegDst 	= 2; 
			controls->Jump 		= 1;
			controls->Branch 	= 0;  
			controls->MemRead 	= 0;  
			controls->MemtoReg 	= 2;  
			controls->ALUOp 	= 0;// don't care  
			controls->MemWrite 	= 0;
			controls->ALUSrc 	= 2;// don't care 
			controls->RegWrite 	= 0; 
			break;	
		}
		case 4://000100; "beq" branch on equal 
		{
			controls->RegDst 	= 2;// don't care  
			controls->Jump 		= 0;
			controls->Branch 	= 1;
			controls->MemRead 	= 0;
			controls->MemtoReg 	= 2;
			controls->ALUOp 	= 1;//subtraction
			controls->MemWrite 	= 0;
			controls->ALUSrc 	= 0;
			controls->RegWrite 	= 0;
			break;
		}
		case 8://001000; "addi" add immediate
		{
			controls->RegDst 	= 0; 
			controls->Jump 		= 0;
			controls->Branch 	= 0;
			controls->MemRead 	= 0;
			controls->MemtoReg 	= 0;
			controls->ALUOp 	= 0;//addition
			controls->MemWrite 	= 0;
			controls->ALUSrc 	= 1;
			controls->RegWrite 	= 1;
			break;
		}	
		case 10://001010; "slti" set less than immediate
		{ 
			controls->RegDst 	= 0; 
			controls->Jump 		= 0;
			controls->Branch 	= 0; 
			controls->MemRead 	= 0;  
			controls->MemtoReg 	= 0; 
			controls->ALUOp 	= 2;
			controls->MemWrite 	= 0;
			controls->ALUSrc 	= 1; 
			controls->RegWrite 	= 1;
			break;
		}
		case 11://001011; "sltiu" set less than immediate unsigned
		{
			controls->RegDst  	= 0;
			controls->Jump 		= 0; 
			controls->Branch 	= 0; 
			controls->MemRead 	= 0;  
			controls->MemtoReg 	= 0; 
			controls->ALUOp 	= 3;
			controls->MemWrite 	= 0; 
			controls->ALUSrc 	= 1; 
			controls->RegWrite 	= 1;
			break;
		}
		case 15://001111; "lui" load uppepr immediate
		{
			controls->RegDst 	= 0; 
			controls->Jump 		= 0; 
			controls->Branch 	= 0; 
			controls->MemRead 	= 1; 
			controls->MemtoReg 	= 1; 
			controls->ALUOp 	= 6;// 110 shift constant into upper 16 bits
			controls->MemWrite 	= 0;
			controls->ALUSrc 	= 1; 
			controls->RegWrite 	= 1;
			break;
		}
		case 35://100011; "lw" load word
		{
			controls->RegDst 	= 0;  
			controls->Jump 		= 0;    
			controls->Branch 	= 0;  
			controls->MemRead 	= 1; 
			controls->MemtoReg 	= 1; 
			controls->ALUOp 	= 0;
			controls->MemWrite 	= 0; 
			controls->ALUSrc 	= 1; 
			controls->RegWrite 	= 1;
			break;
		}
		case 43://101011; "sw" store word
		{
			controls->RegDst 	= 2;// don't care 
			controls->Jump 		= 0;    
			controls->Branch 	= 0;  
			controls->MemRead 	= 1; 
			controls->MemtoReg 	= 2;// don't care  
			controls->ALUOp 	= 0;
			controls->MemWrite 	= 1; 
			controls->ALUSrc 	= 1; 
			controls->RegWrite 	= 0;
			break;
		}
		default:// illegal instruction, return 1 for halt condition
			return 1;
	}
	return 0;
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
	unsigned temp = ((offset>>15)&1);
	
	
	
	//printf("%d\n", temp);
	
	
	
	if(temp == 1)
	{
		//printf("Negative\n");
		
		offset = offset | 0xFFFF0000;
		
	}
	
	else if (temp == 0)
	{
		//printf("Positive\n");
		
		offset = offset & 0x0000FFFF;
	}		
	
	

	
	//printf("%lx\n", offset);
	
	extended_value = &offset;
}


/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
	char ALUControl;

	switch ((int) ALUSrc)
	{
		case 0:// Read data 2
		{
			switch ((int) ALUOp)
			{
				case 1:// Subtract, Branch
				{
					ALUControl = (int) 1;// 001 subtraction
					ALU(data1, data2, ALUControl, ALUresult, Zero);
					break;
				}
				case 7:// R-types
				{
					switch (funct)
					{
						case 32:// 100000 "Add"
						{	
							ALUControl = (int) 0; //000
							ALU(data1, data2, ALUControl, ALUresult, Zero);
							break;
						}
						case 34:// 100010 "Subtract"
						{
							ALUControl = (int) 1; //001
							ALU(data1, data2, ALUControl, ALUresult, Zero);
							break;
						}
						case 36:// 100100 "AND"
						{
							ALUControl = (int) 4;// 100
							ALU(data1, data2, ALUControl, ALUresult, Zero);	
							break;
						}
						case 37:// 100101 "OR"
						{
							ALUControl = (int) 5;// 101
							ALU(data1, data2, ALUControl, ALUresult, Zero);	
							break;
						}
						case 42:// 101010 "set less than"
						{
							ALUControl = (int) 2;// 010
							ALU(data1, data2, ALUControl, ALUresult, Zero);
							break;
						}
						case 43:// 101011 "set less than unsigned"
						{
							ALUControl = (int) 3;// 011
							ALU(data1, data2, ALUControl, ALUresult, Zero);
							break;					
						}
						//***This instruction is not listed in  Appendix A figure 1
						case 39:// 100111 "NOT"
						{
							ALUControl = (int) 7;// 111		
							break;
						}
						default:
							return 1;//illegal Function
					}
					break;
				}
				// default:// return 1 for halt condition, illegal ALUOp
				// 	return 1;
			}
			break;
		}
		case 1:// Read Sign-extended
		{
			switch ((int) ALUOp)
			{
				case 0:// add/don't care, LW SW
				{	
					ALUControl = (int) 0; //000 Addition
					ALU(data1, extended_value, ALUControl, ALUresult, Zero);
					break;
				}
				case 2:// 010 "Set less than immediate"
				{
					ALUControl = (int) 2;// 010 set less than
					ALU(data1, extended_value, ALUControl, ALUresult, Zero);			
					break;
				}
				case 3:// 010 "Set less than immediate unsigned"
				{
					ALUControl = (int) 3;// 011 "set less than unsigned"
					ALU(data1, extended_value, ALUControl, ALUresult, Zero);			
					break;
				}
				//***Not clear if this instruction is legal, Appendix A figure 1 does not show it
				case 4:// 100 "AND immediate"
				{
					ALUControl = (int) 4;// 100 "AND"
					ALU(data1, extended_value, ALUControl, ALUresult, Zero);			
					break;
				}
				//***Not clear if this instruction is legal either. Same as AND
				case 5:// 101 "OR immediate"
				{
					ALUControl = (int) 5;// 101 "OR"
					ALU(data1, extended_value, ALUControl, ALUresult, Zero);		
					break;
				}
				case 6://110, Load upper immediate, "shift left 16"
				{
					ALUControl = (int) 6; //110 "Shift left b by 16"
					ALU(data1, extended_value, ALUControl, ALUresult, Zero);
					break;
				}
				// default://illegal ALUOp
				// 	return 1;
			}
			break;
		}
		case 2:// Don't care
			break;
		// default:// illegal ALUSrc
		// 	return 1;
	}
	return 0;
	/* 
	halts should only result from an illegal function, 
	commented return 1;'s are for debugging
	*/
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
	// check if writing
	if (RegWrite == 1)
	{
		// Set memory to register
		if (MemtoReg == 1 && RegDst == 0)
		{
			// set reg to the memdata
			Reg[r2] = memdata;
		}

		else if (MemtoReg == 1 && RegDst == 1)
		{
			Reg[r3] = memdata;
		}

		// Store ALUResults accordingly

		else if (MemtoReg == 0 && RegDst == 0)
		{	
			// aluresult in r2
			Reg[r2] = ALUresult;
		}

		else if (MemtoReg == 0 && RegDst == 1)
		{
			// aluresult in r3
			Reg[r3] = ALUresult;
		}
	}
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}


