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

/*

void selectPinPushPullOutput(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_OD)  &= ~mask;
    *(base+OFS_OUT) |=  mask;
}

void selectPinOpenDrainOutput(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_OD)  |= mask;
    *(base+OFS_OUT) |= mask;
}

void selectPinDigitalInput(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_OUT) &= ~mask;
}

void selectPinInterruptRisingEdge(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_POSITIVE)  |=  mask;
    *(base+OFS_INT_NEGATIVE)  &= ~mask;
    *(base+OFS_INT_EDGE_MODE) |=  mask;
}

void selectPinInterruptFallingEdge(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_POSITIVE)  &= ~mask;
    *(base+OFS_INT_NEGATIVE)  |=  mask;
    *(base+OFS_INT_EDGE_MODE) |=  mask;
}

void selectPinInterruptBothEdges(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_POSITIVE)  |= mask;
    *(base+OFS_INT_NEGATIVE)  |= mask;
    *(base+OFS_INT_EDGE_MODE) |= mask;
}

void selectPinInterruptHighLevel(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_POSITIVE)  |=  mask;
    *(base+OFS_INT_NEGATIVE)  &= ~mask;
    *(base+OFS_INT_EDGE_MODE) &= ~mask;
}

void selectPinInterruptLowLevel(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_POSITIVE)  &= ~mask;
    *(base+OFS_INT_NEGATIVE)  |=  mask;
    *(base+OFS_INT_EDGE_MODE) &= ~mask;
}

void enablePinInterrupt(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_ENABLE) |= mask;
}

void disablePinInterrupt(uint8_t pin)
{
    uint32_t mask = 1 << pin;
    *(base+OFS_INT_ENABLE) &= ~mask;
}

void setPinValue(uint8_t pin, bool value)
{
    uint32_t mask = 1 << pin;
    if (value)
        *(base+OFS_DATA) |= mask;
    else
        *(base+OFS_DATA) &= ~mask;
}

bool getPinValue(uint8_t pin)
{
    uint32_t value = *(base+OFS_DATA);
    return (value >> pin) & 1;
}
*/

void setPortValue(uint32_t value)
{
     *(base+OFS_DATA) = value;
}

void setControlReg(uint32_t value)
{
	*(base+OFS_CTRL) = value;
}

void setStatusReg(uint32_t value)
{
	*(base+OFS_STATUS) = value;
}

void setBaudRateReg(uint32_t value) //remember to change func arguments to convert frequency to brd value
{
    //should be frequency converted to brd value. change later
     *(base+OFS_BRD) = value;
}

uint32_t getPortValue()
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

uint32_t getBaudRateReg() 
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

//enable transmit/receiver/brd

void setCtrlEnableBit(bool bit)
{
    //this will turn on transmitter/receiver/brd
	if(bit)
		*(base+OFS_CTRL) |= (1 << 15);
	else
		*(base+OFS_CTRL) &= ~(1 << 15);
}

