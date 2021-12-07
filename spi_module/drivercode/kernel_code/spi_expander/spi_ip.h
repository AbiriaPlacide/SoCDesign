//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: DE1-SoC Board

// Hardware configuration:
//   Mapped to offset of 0 in light-weight MM interface aperature
//   IRQ80 is used as the interrupt interface to the HPS

//-----------------------------------------------------------------------------

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include <stdbool.h>

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

#define BAUD_115200 (217 << 7)
#define BAUD_9600   (2604 << 7)

void setDataReg(uint32_t value);
void setControlReg(uint32_t value);
void modControlReg(uint32_t value);
void clearControlReg(uint32_t value);
void setStatusReg(uint32_t value);
void setBaudrateReg(uint32_t value);

uint32_t getDataReg();
uint32_t getControlReg();
uint32_t getStatusReg();
uint32_t getBaudrateReg();

//STATUS reg flags
bool getRxOverflow();
bool getRxFull();
bool getRxEmpty();
bool getTxOverflow();
bool getTxFull();
bool getTxEmpty();

//set ENABLE bit of CTRL_REG
void setCtrlEnableBit(bool bit);
#endif
