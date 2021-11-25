// GPIO IP Example
// GPIO IP Library (gpio_ip.c)
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

//-----------------------------------------------------------------------------

#include <stdint.h>          // C99 integer types -- uint32_t
#include <stdbool.h>         // bool
#include <fcntl.h>           // open
#include <sys/mman.h>        // mmap
#include <unistd.h>          // close
#include "address_map.h"  // address map
#include "spi_ip.h"         // gpio
#include "spi_regs.h"       // registers

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

uint32_t *base = NULL;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

bool spiOpen()
{
    // Open /dev/mem
    int file = open("/dev/mem", O_RDWR | O_SYNC);
    bool bOK = (file >= 0);
    if (bOK)
    {
        // Create a map from the physical memory location of
        // /dev/mem at an offset to LW avalon interface
        // with an aperature of SPAN_IN_BYTES bytes
        // to any location in the virtual 32-bit memory space of the process
        base = mmap(NULL, SPAN_IN_BYTES, PROT_READ | PROT_WRITE, MAP_SHARED,
                    file, LW_BRIDGE_BASE + SPI_BASE_OFFSET);
        bOK = (base != MAP_FAILED);

        // Close /dev/mem
        close(file);
    }
    return bOK;
}


uint32_t getDataReg()
{
    uint32_t value = *(base+OFS_DATA);
    return value;
}

uint32_t getControlReg()
{
    uint32_t value = *(base+OFS_CTRL);
    return value;
}

uint32_t getStatusReg()
{
    uint32_t value = *(base+OFS_STATUS);
    return value;
}

uint32_t getBaudrateReg() 
{
    uint32_t value = *(base+OFS_BRD);
    return value;
}

//STATUS reg flags

bool getRxOverflow()
{
    uint8_t pin = 0;
    uint32_t value = *(base+OFS_STATUS);
    return (value >> pin) & 1;
}

bool getRxFull()
{
    uint8_t pin = 1;
    uint32_t value = *(base+OFS_STATUS);
    return (value >> pin) & 1;

}

bool getRxEmpty()
{
    uint8_t pin = 2;
    uint32_t value = *(base+OFS_STATUS);
    return (value >> pin) & 1;

}

bool getTxOverflow()
{
    uint8_t pin = 3;
    uint32_t value = *(base+OFS_STATUS);
    return (value >> pin) & 1;
}

bool getTxFull()
{
    uint8_t pin = 4;
    uint32_t value = *(base+OFS_STATUS);
    return (value >> pin) & 1;

}

bool getTxEmpty()
{
    uint8_t pin = 5 ;
    uint32_t value = *(base+OFS_STATUS);
    return (value >> pin) & 1;

}


void setDataReg(uint32_t value)
{
	//we dont want a read modify write, so we directly change the value
     *(base+OFS_DATA) = value;
}

void setControlReg(uint32_t value)
{
	*(base+OFS_CTRL) = value;
}

void modControlReg(uint32_t value)
{
	uint32_t temp = getControlReg();

	temp |= value;
	*(base+OFS_CTRL) = temp;
}

void clearControlReg(uint32_t value)
{
	*(base+OFS_CTRL) &= ~value;
}

void setStatusReg(uint32_t value)
{
	*(base+OFS_STATUS) = value;
}

void setBaudrateReg(uint32_t value) 
{
	uint32_t res = 25000000/value;
     *(base+OFS_BRD) = res << 7;
}

//enable transmit/receiver/brd
void setCtrlEnableBit(bool bit)
{
    //this will turn on transmitter/receiver/brd
	if(bit)
		*(base+OFS_CTRL) |= (1 << 15);
	else
		*(base+OFS_CTRL) &= ~(1 << 15);
}

