// GPIO IP Library Registers
// Jason Losh

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: DE1-SoC Board
// Target uC:       -
// System Clock:    -

// Hardware configuration:
// GPIO IP core connected to light-weight Avalon bus

//-----------------------------------------------------------------------------
// Device includes, defines, and assembler directives
//-----------------------------------------------------------------------------

#ifndef GPIO_REGS_H_
#define GPIO_REGS_H_

#define OFS_DATA             0
#define OFS_STATUS           1
#define OFS_CTRL             2
#define OFS_BRD              3


#define SPAN_IN_BYTES 16  //total number of byes so...4 byes per register * 4 registers = 16 bytes total

#define GPIO_IRQ 80

#endif

