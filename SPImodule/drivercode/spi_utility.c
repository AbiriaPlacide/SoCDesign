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
#include <unistd.h>
#include "spi_ip.h"

// Initialize Hardware
void initHw()
{
    // Initialize SPI IP
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

				else if(strcmp(argv[2], "led") == 0)
				{
					uint32_t x = strtoull(argv[3], NULL, 16);

					if(x == 0)
					{

						setDataReg(0x00400000); //address
						sleep(0.2); //allow time address to be written and transmitted, or else it doesn't work?
						setDataReg(0x00400901); //led 0
					}

					else if(x == 1)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400902); //led 1 
					}

					else if(x == 2)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400903); //led 1 
					}
					else if(x == 3)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400904); //led 1 
					}
					else if(x == 4)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400905); //led 1 
					}
					else if(x == 5)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400906); //led 1 
					}
					else if(x == 6)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400907); //led 1 
					}
					else if(x == 7)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x00400908); //led 1 
					}

					else if(x == 8)
					{
						setDataReg(0x00400000); //address
						sleep(0.2);
						setDataReg(0x004009ff); //led 1 
					}
	
					
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
					setControlReg(x);
				}

				else if(strcmp(argv[2], "brd") == 0)
				{
					uint32_t x = strtoull(argv[3], NULL, 10);
					printf("setting baud reg with %u\n", x);
					printf("enabling control bit in ctrl reg\n");
					setBaudrateReg(x);
					setCtrlEnableBit(1); //allows signal to be generated
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
						modControlReg(x-1); //sets first 5
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
					printf("clearing cs_auto\n");
					clearControlReg(15 << 5); //clear cs auto
					sleep(0.2); //
					modControlReg(1 << 9);
				}

				else if(strcmp(argv[2], "cs1_enable") == 0)
				{
					printf("setting cs1_enable\n");
					printf("clearing cs_auto\n");
					clearControlReg(15 << 5); //clear cs auto
					sleep(0.2); //
					modControlReg(2 << 10);
				}

				else if(strcmp(argv[2], "cs2_enable") == 0)
				{
					printf("setting cs2_enable\n");
					printf("clearing cs_auto\n");
					clearControlReg(15 << 5); //clear cs auto
					sleep(0.2); //
					modControlReg(4 << 11);
				}

				else if(strcmp(argv[2], "cs3_enable") == 0)
				{
					printf("setting cs3_enable\n");
					printf("clearing cs_auto\n");
					clearControlReg(15 << 5); //clear cs auto
					sleep(0.2); //
					modControlReg(8 << 12);
				}

				else if(strcmp(argv[2], "cs0_auto") == 0)
				{
					printf("setting cs0_auto\n");
					printf("clearing cs_enable\n");
					clearControlReg(15 << 9); //clear cs_enable
					sleep(0.2);
					modControlReg(1 << 5);
				}

				else if(strcmp(argv[2], "cs1_auto") == 0)
				{
					printf("setting cs1_auto\n");
					printf("clearing cs_enable\n");
					clearControlReg(15 << 9); //clear cs_enable
					sleep(0.2);
					modControlReg(2 << 5);
				}

				else if(strcmp(argv[2], "cs2_auto") == 0)
				{
					printf("setting cs2_enable\n");
					printf("clearing cs_enable\n");
					clearControlReg(15 << 9); //clear cs_enable
					sleep(0.2);
					modControlReg(4 << 5);
				}

				else if(strcmp(argv[2], "cs3_auto") == 0)
				{
					printf("setting cs3_enable\n");
					printf("clearing cs_enable\n");
					clearControlReg(15 << 9); //clear cs_enable
					sleep(0.2);
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

			else if(strcmp(argv[2], "cs0_auto") == 0)
			{
				printf("clearing cs0_auto bit\n");
				clearControlReg(1 << 5);
			}

			else if(strcmp(argv[2], "cs1_auto") == 0)
			{
				printf("clearing cs1_auto bit\n");
				clearControlReg(1 << 6);
			}

			else if(strcmp(argv[2], "cs2_auto") == 0)
			{
				printf("clearing cs2_auto bit\n");
				clearControlReg(1 << 7);
			}

			else if(strcmp(argv[2], "cs3_auto") == 0)
			{
				printf("clearing cs3_auto bit\n");
				clearControlReg(1 << 8);
			}

			else if(strcmp(argv[2], "cs_auto") == 0)
			{
				printf("clearing cs_auto bits\n");
				clearControlReg(15 << 5);
			}

			else if(strcmp(argv[2], "cs0_enable") == 0)
			{
				printf("clearing cs0_enable bit\n");
				clearControlReg(1 << 9);
			}
			else if(strcmp(argv[2], "cs1_enable") == 0)
			{
				printf("clearing cs1_enable bit\n");
				clearControlReg(1 << 10);
			}
			else if(strcmp(argv[2], "cs2_enable") == 0)
			{
				printf("clearing cs2_enable bit\n");
				clearControlReg(1 << 11);
			}
			else if(strcmp(argv[2], "cs3_enable") == 0)
			{
				printf("clearing cs3_enable bit\n");
				clearControlReg(1 << 12);
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
					printf("disabling tx/rx/baudrate\n");
					setCtrlEnableBit(0);
				}
			}
		}
	}
}
