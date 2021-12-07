// SPI IP Library (spi_ip.c)
// Abiria Placide

//-----------------------------------------------------------------------------
// Hardware Target
//-----------------------------------------------------------------------------

// Target Platform: DE1-SoC Board

// Hardware configuration:
// HPS interface:
//   Mapped to offset of 0x8000 in light-weight MM interface aperature
//   IRQ80 is used as the interrupt interface to the HPS

//-----------------------------------------------------------------------------

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <asm/io.h> //iowrite, ioread, ioremap_nocache(platform specific)
#include "address_map.h"  // address map
#include "spi_regs.h"       // registers offsets

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------

//-----------
//kernel info
//

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Abiria Placide");
MODULE_DESCRIPTION("SPI IP Driver");

//global vars
static unsigned int *base = NULL;

//-----------------------------------------------------------------------------
// Subroutines
//-----------------------------------------------------------------------------

uint32_t getDataReg(void)
{
    uint32_t value = ioread32(base+OFS_DATA);
    return value;
}

uint32_t getControlReg(void)
{
    uint32_t value = ioread32(base+OFS_CTRL);
    return value;
}

void setCtrlEnableBit(bool bit)
{
    //this will turn on transmitter/receiver/brd

	uint32_t value = getControlReg();

	if(bit)
	{
		value |= (1 << 15);
		iowrite32( value, (base+OFS_CTRL));
	}
	else
	{
		value &= ~(1 << 15);
		iowrite32(value, base+OFS_CTRL);
	}

}

void setControlReg(uint32_t value)
{
	iowrite32(value, base+OFS_CTRL);
}

void modControlReg(uint32_t value)
{
	uint32_t temp = getControlReg();
	temp |= value;
	iowrite32(temp, base+OFS_CTRL);
}

void clearControlReg(uint32_t value)
{
	iowrite32(value, base+OFS_CTRL);
}

void setBaudrateReg(uint32_t value)
{
     iowrite32(value, (base+OFS_BRD)); 
	 setCtrlEnableBit(1); //also enable brd to be outputed
}

uint32_t getBaudrateReg(void)
{
    uint32_t value = ioread32(base+OFS_BRD);
    return value;
}

bool getRxEmpty(void)
{
    uint8_t pin = 2;
    uint32_t value = ioread32(base+OFS_STATUS);
    return (value >> pin) & 1;
}

void setDataReg(uint32_t value)
{
	//we dont want a read modify write, so we directly change the value
     iowrite32(value, base+OFS_DATA);
}
//kernal objects

//addresses
//slave address 0x40
//0x00 - dir (if 0, output)
//0x06 - pull up(if 1, pull up resistor enabled)
//0x09 - GPIO data (if 1, logic high), will turn on LED

//format [device address, register address, data]

//set dir register
static uint32_t dir = 0;
module_param(dir, int, S_IRUGO);
MODULE_PARM_DESC(dir, "set dir");

static ssize_t storeDir0(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{

			value |= (dir << 0); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 0); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showDir0(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = value & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction0 attr
static struct kobj_attribute dir0_attr = __ATTR(dir, 0664, showDir0, storeDir0);


//pin1 dir

static ssize_t storeDir1(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 1); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 1); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showDir1(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 1) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction1 attr
static struct kobj_attribute dir1_attr = __ATTR(dir, 0664, showDir1, storeDir1);

//pin 2 dir
static ssize_t storeDir2(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 2); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 2); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showDir2(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 2) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction2 attr
static struct kobj_attribute dir2_attr = __ATTR(dir, 0664, showDir2, storeDir2);

//pin 3 dir
static ssize_t storeDir3(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 3); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 3); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showDir3(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 3) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction3 attr
static struct kobj_attribute dir3_attr = __ATTR(dir, 0664, showDir3, storeDir3);


//pin4 dir
static ssize_t storeDir4(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 4); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 4); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showDir4(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 4) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction4 attr
static struct kobj_attribute dir4_attr = __ATTR(dir, 0664, showDir4, storeDir4);

//pin 5 dir
static ssize_t storeDir5(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 5); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 5); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showDir5(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 5) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction5 attr
static struct kobj_attribute dir5_attr = __ATTR(dir, 0664, showDir5, storeDir5);

//pin 6 dir
static ssize_t storeDir6(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 6); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 6); //clear bit
		}
		setDataReg(value);
	}

	return count;
}

static ssize_t showDir6(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 6) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction6 attr
static struct kobj_attribute dir6_attr = __ATTR(dir, 0664, showDir6, storeDir6);

//pin 7 dir
static ssize_t storeDir7(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
	{
		uint32_t value = (0x004000FF);
		if(dir == 0)
		{
			value |= (dir << 7); //set bit
		}

		else if(dir == 1)
		{
		 	value &= ~(dir << 7); //clear bit
		}
		setDataReg(value);
	}

	return count;
}

static ssize_t showDir7(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410000);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value > 7) & 0x1;
	return sprintf(buffer, "dir: %u\n", value);
}

//direction7 attr
static struct kobj_attribute dir7_attr = __ATTR(dir, 0664, showDir7, storeDir7);

//pin0 pull up
static uint32_t pullup = 0;
module_param(pullup, int, S_IRUGO);
MODULE_PARM_DESC(pullup, "set pullup");

static ssize_t storepullup0(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{

			value |= (pullup << 0); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 0); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showpullup0(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 0) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup0 attr
static struct kobj_attribute pullup0_attr = __ATTR(pullup, 0664, showpullup0, storepullup0);


//pin1 pullup
static ssize_t storepullup1(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 1); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 1); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup1(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 1) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup1 attr
static struct kobj_attribute pullup1_attr = __ATTR(pullup, 0664, showpullup1, storepullup1);


//pin 2 pull up
static ssize_t storepullup2(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 2); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 2); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup2(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 2) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup2 attr
static struct kobj_attribute pullup2_attr = __ATTR(pullup, 0664, showpullup2, storepullup2);

//pin 3 pullup
static ssize_t storepullup3(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 3); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 3); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup3(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 3) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup3 attr
static struct kobj_attribute pullup3_attr = __ATTR(pullup, 0664, showpullup3, storepullup3);


//pin 4 pull up
static ssize_t storepullup4(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 4); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 4); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup4(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 4) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup4 attr
static struct kobj_attribute pullup4_attr = __ATTR(pullup, 0664, showpullup4, storepullup4);


//pin 5 pull up
static ssize_t storepullup5(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 5); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 5); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup5(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 5) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup5 attr
static struct kobj_attribute pullup5_attr = __ATTR(pullup, 0664, showpullup5, storepullup5);

//pin 6 pullup
static ssize_t storepullup6(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 6); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 6); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup6(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 6) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup6 attr
static struct kobj_attribute pullup6_attr = __ATTR(pullup, 0664, showpullup6, storepullup6);

//pin 7 pullup
static ssize_t storepullup7(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &pullup);
	if(result == 0)
	{
		uint32_t value = (0x00400600);
		if(pullup == 1)
		{
			value |= (pullup << 7); //set bit
		}

		else if(pullup == 0)
		{
		 	value &= ~(pullup << 7); //clear bit
		}

		setDataReg(value);
	}
	return count;
}

static ssize_t showpullup7(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410600);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 7) & 0x1;
	return sprintf(buffer, "pullup: %u\n", value);
}

//pullup7 attr
static struct kobj_attribute pullup7_attr = __ATTR(pullup, 0664, showpullup7, storepullup7);

//pin0 data

static uint32_t data = 0;
module_param(data, int, S_IRUGO);
MODULE_PARM_DESC(data, "set data");

static ssize_t storedata0(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 0); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 0); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata0(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 0) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data0 attr
static struct kobj_attribute data0_attr = __ATTR(data, 0664, showdata0, storedata0);


//pin 1 data

static ssize_t storedata1(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 1); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 1); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata1(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 1) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data1 attr
static struct kobj_attribute data1_attr = __ATTR(data, 0664, showdata1, storedata1);

//pin2 data

static ssize_t storedata2(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 2); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 2); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata2(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 2) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data2 attr
static struct kobj_attribute data2_attr = __ATTR(data, 0664, showdata2, storedata2);

//pin 3 data
static ssize_t storedata3(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 3); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 3); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata3(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 3) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data3 attr
static struct kobj_attribute data3_attr = __ATTR(data, 0664, showdata3, storedata3);


//pin 4 data

static ssize_t storedata4(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 4); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 4); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata4(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 4) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data4 attr
static struct kobj_attribute data4_attr = __ATTR(data, 0664, showdata4, storedata4);

//pin 5 data
static ssize_t storedata5(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 5); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 5); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata5(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 5) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data5 attr
static struct kobj_attribute data5_attr = __ATTR(data, 0664, showdata5, storedata5);

//pin 6 data

static ssize_t storedata6(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 6); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 6); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata6(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 6) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data6 attr
static struct kobj_attribute data6_attr = __ATTR(data, 0664, showdata6, storedata6);

//pin 7 data
static ssize_t storedata7(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &data);
	if(result == 0)
	{
		uint32_t value = (0x00400900);
		if(data == 1)
		{

			value |= (pullup << 7); //set bit
		}

		else if(data == 0)
		{
		 	value &= ~(pullup << 7); //clear bit
		}

		setDataReg(value);
	}

	return count;
}

static ssize_t showdata7(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	uint32_t value = (0x00410900);
	setDataReg(value); //read from receive fifo.
	value = getDataReg(); //read received data. 
	value = (value >> 7) & 0x1;
	return sprintf(buffer, "data: %u\n", value);
}

//data7 attr
static struct kobj_attribute data7_attr = __ATTR(data, 0664, showdata7, storedata7);

//pin0 attrs
static struct attribute *attrs0[] = { &dir0_attr.attr, &pullup0_attr.attr, &data0_attr.attr, NULL};
//pin1 attrs
static struct attribute *attrs1[] = { &dir1_attr.attr, &pullup1_attr.attr, &data1_attr.attr, NULL};
//pin2 attrs
static struct attribute *attrs2[] = { &dir2_attr.attr, &pullup2_attr.attr, &data2_attr.attr, NULL};
//pin3 attrs
static struct attribute *attrs3[] = { &dir3_attr.attr, &pullup3_attr.attr, &data3_attr.attr, NULL};
//pin4 attrs
static struct attribute *attrs4[] = { &dir4_attr.attr, &pullup4_attr.attr, &data4_attr.attr, NULL};
//pin5 attrs
static struct attribute *attrs5[] = { &dir5_attr.attr, &pullup5_attr.attr, &data5_attr.attr, NULL};
//pin6 attrs
static struct attribute *attrs6[] = { &dir6_attr.attr, &pullup6_attr.attr, &data6_attr.attr, NULL};
//pin7 attrs
static struct attribute *attrs7[] = { &dir7_attr.attr, &pullup7_attr.attr, &data7_attr.attr, NULL};

static struct attribute_group group0 = 
{
	.name = "pin0",
	.attrs = attrs0
};

static struct attribute_group group1 =
{
	.name = "pin1",
	.attrs = attrs1
};

static struct attribute_group group2 =
{
	.name = "pin2",
	.attrs = attrs2
};

static struct attribute_group group3 =
{
	.name = "pin3",
	.attrs = attrs3
};

static struct attribute_group group4 =
{
	.name = "pin4",
	.attrs = attrs4
};

static struct attribute_group group5 =
{
	.name = "pin5",
	.attrs = attrs5
};

static struct attribute_group group6 =
{
	.name = "pin6",
	.attrs = attrs6
};

static struct attribute_group group7 =
{
	.name = "pin7",
	.attrs = attrs7
};

static struct kobject *kobj;

//init kernel moduel
static int __init initialize_module(void)
{
	int result;
	printk(KERN_INFO "SPI driver: starting\n");
	kobj = kobject_create_and_add("spi_expander", kernel_kobj);
	
	if(!kobj)
	{
		printk(KERN_ALERT "SPI driver: failed to create and add kobj\n");
		return -ENOENT;
	}

	result = sysfs_create_group(kobj, &group0);

	if(result != 0)
	{
		return result;
	}

	result = sysfs_create_group(kobj, &group1);

	if(result != 0)
	{
		return result;
	}

	result = sysfs_create_group(kobj, &group2);

	if(result != 0)
	{
		return result;
	}

	result = sysfs_create_group(kobj, &group3);

	if(result != 0)
	{
		return result;
	}


	result = sysfs_create_group(kobj, &group4);

	if(result != 0)
	{
		return result;
	}

	result = sysfs_create_group(kobj, &group5);

	if(result != 0)
	{
		return result;
	}

	result = sysfs_create_group(kobj, &group6);
	if(result != 0)
	{
		return result;
	}

	result = sysfs_create_group(kobj, &group7);
	if(result != 0)
	{
		return result;
	}

	base = (unsigned int*)ioremap_nocache(LW_BRIDGE_BASE + SPI_BASE_OFFSET, SPAN_IN_BYTES);

	if(base == NULL)
		return -ENODEV;
	printk(KERN_INFO "spi expander: initialized\n");

	return 0;

}

static void __exit exit_module(void)
{
	kobject_put(kobj);
	printk(KERN_INFO "spi_expander : exiting");
}

module_init(initialize_module);
module_exit(exit_module);
