// GPIO IP Example
// Stop_Go Application (stop_go.c)
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: DE1-SoC Board

// Hardware configuration:
// GPIO Port:
//   GPIO_1[31-0] is used as a general purpose GPIO port
// HPS interface:
//   Mapped to offset of 0 in light-weight MM interface aperature
//   IRQ80 is used as the interrupt interface to the HPS
// Red LED:
//   Pin GPIO_1_0 drives a red LED through a 470 ohm resistror
// Green LED:
//   Pin GPIO_1_1 drives a green LED through a 470 ohm resistror
// Pushbutton:
//   Pin GPIO_1_2 pulled low by switch, pulled high through 10k ohm resistor

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "spi_ip.h"

// Initialize Hardware
void initHw()
{
    // Initialize GPIO IP
    bool ok = spiOpen();

    if(ok) 
	{ 
		printf("initialized\n");
		printf("initializing brd\n");
    	setBaudRateReg(BAUD_115200);
		setCtrlEnableBit(1);
		printf("done\n");

	}
	else{ printf("failed initialization\n"); }
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------

int main(void)
{
	// Initialize hardware
	initHw();
    setStatusReg(30);

	printf("got inside main\n");

    uint32_t a = getRxOverflow();
    uint32_t b = getRxFull();
    uint32_t c = getRxEmpty();
    uint32_t d = getTxOverflow(); 
    uint32_t e = getTxFull();
    uint32_t f = getTxEmpty();
    printf("Rx Overflow: %u \n", a);
    printf("Rx Full: %u \n", b);
    printf("Rx Empty: %u \n", c);
    printf("Tx Overflow: %u \n", d);
    printf("Tx Full: %u \n", e);
    printf("Tx Empty: %u \n", f);

	printf("baudrateReg Value: %08x \n", getBaudRateReg());
	printf("ctrl reg value: %08x \n", getControlReg());
}
