// GPIO IP Example
// Stop_Go Application (stop_go.c)
//Abiria Placide

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: DE1-SoC Board

// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "spi_ip.h"

// Initialize Hardware
void initHw()
{
    // Initialize GPIO IP
    bool ok = spiOpen();
    if(!ok) 
	{ 
		printf("initialization failed");
	}
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	initHw();

	if(argc > 1)
	{
		//get command

		if(strcmp(argv[1], "get") == 0)
		{
			if( argc < 4)
			{
				if(strcmp(argv[2], "data") == 0)
				{
					printf("Data: %08x \n ", getDataReg());
				}

				else if(strcmp(argv[2], "status") == 0)
				{
					printf("Status Reg: %08x \n", getStatusReg());
				}

				else if(strcmp(argv[2], "control") == 0)
				{
					printf("Control Reg: %08x \n", getControlReg());
				}

				else if(strcmp(argv[2], "brd") == 0)
				{
					printf("BRD Reg: %08x \n", getBaudrateReg());
				}

			}
		}

		else if(strcmp(argv[1], "set") == 0)
		{
			if(argc > 3)
			{
				if(strcmp(argv[2], "data") == 0)
				{

					uint32_t x = strtoull(argv[3], NULL, 16);
					printf("setting data reg: %08x \n", x);
					setDataReg(x);
				}

				else if(strcmp(argv[2], "status") == 0)
				{
					uint32_t x = strtoull(argv[3], NULL, 16);
					printf("setting status reg: with %u\n", x);
					setStatusReg(x);
				}

				else if(strcmp(argv[2], "control") == 0)
				{
					uint32_t x = strtoull(argv[3], NULL, 16);
					printf("setting control reg with %u\n", x);
					setControlReg(atoi(argv[3]));
				}

				else if(strcmp(argv[2], "brd") == 0)
				{
					uint32_t x = strtoull(argv[3], NULL, 10);
					printf("setting baud reg with %u\n", x);
					setBaudrateReg(x);
				}

				else if(strcmp(argv[2], "word_size") == 0)
				{
					if(atoi(argv[3]) > 32)
					{
						printf("word size must be less than 33\n");
					}

					else
					{
						printf("setting word size\n");
						clearControlReg(31); //clears first 5 bits
						uint32_t x = strtoull(argv[3], NULL, 10);
						modControlReg(x); //sets first 5
					}
				}

				else
				{
					printf("invalid set command %s \n" , argv[2]);
				}
			}

			if(argc == 3)
			{

				if(strcmp(argv[2], "cs0_enable") == 0)
				{
					printf("setting cs0_enable\n");
					modControlReg(1 << 9);
				}

				else if(strcmp(argv[2], "cs1_enable") == 0)
				{
					printf("setting cs1_enable\n");
					modControlReg(2 << 9);
				}

				else if(strcmp(argv[2], "cs2_enable") == 0)
				{
					printf("setting cs2_enable\n");
					modControlReg(4 << 9);
				}

				else if(strcmp(argv[2], "cs3_enable") == 0)
				{
					printf("setting cs3_enable\n");
					modControlReg(8 << 9);
				}

				else if(strcmp(argv[2], "cs0_auto") == 0)
				{
					printf("setting cs0_auto\n");
					modControlReg(1 << 5);
				}

				else if(strcmp(argv[2], "cs1_auto") == 0)
				{
					printf("setting cs1_enable\n");
					modControlReg(2 << 5);
				}

				else if(strcmp(argv[2], "cs2_auto") == 0)
				{
					printf("setting cs2_enable\n");
					modControlReg(4 << 5);
				}

				else if(strcmp(argv[2], "cs3_auto") == 0)
				{
					printf("setting cs3_enable\n");
					modControlReg(8 << 5);
				}
			}

		}

		else if(strcmp(argv[1], "reset") == 0)
		{
			if(strcmp(argv[2], "txfo") == 0)
			{
				printf("resetting txfo bit\n");
				setStatusReg(8);
			}

			else if(strcmp(argv[2], "rxfo") == 0)
			{
				printf("resetting rxfo bit\n");
				setStatusReg(1);
			}

			else if(strcmp(argv[2], "cs_auto") == 0)
			{
				printf("clearing cs_auto bits\n");
				clearControlReg(15 << 5);
			}

			else if(strcmp(argv[2], "cs_enable") == 0)
			{
				printf("clearing cs_enable bits\n");
				clearControlReg(15 << 9);
			}

		}


		else if(strcmp(argv[1], "enable") == 0)
		{
			if(argc == 3)
			{
				if(strcmp(argv[2], "control_bit") ==0)
				{
					printf("enabling tx/rx/baudrate\n");
					setCtrlEnableBit(1);
				}
			}
		}

		else if(strcmp(argv[1], "disable") == 0)
		{
			if(argc == 3)
			{
				if(strcmp(argv[2], "control_bit") == 0)
				{
					printf("enabling tx/rx/baudrate\n");
					setCtrlEnableBit(0);
				}
			}
		}
	}
}
