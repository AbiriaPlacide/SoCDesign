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
					printf("setting data reg: %08x \n", atoi(argv[3]));
					setDataReg(atoi(argv[3]));
				}

				else if(strcmp(argv[2], "status") == 0)
				{
					printf("setting status reg: %08x \n", atoi(argv[3]));
					setStatusReg(atoi(argv[3]));
				}

				else if(strcmp(argv[2], "control") == 0)
				{
					printf("setting control reg: %08x \n", atoi(argv[3]));
					setControlReg(atoi(argv[3]));
				}

				else if(strcmp(argv[2], "brd") == 0)
				{
					printf("setting baud reg: %08x \n", atoi(argv[3]));
					setBaudrateReg(atoi(argv[3]));
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
		}

		else
		{
			printf("unknown command\n");
		}
	}
}
