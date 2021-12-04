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

//baud_rate
static uint32_t dir = 0;
module_param(dir, int, S_IRUGO);
MODULE_PARM_DESC(dir, "set dir");

static ssize_t storeDir(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &dir);
	if(result == 0)
		setDataReg(0x400000);
		setDataReg(dir);

	return count;
}

static ssize_t showDir(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	return sprintf(buffer, "%s", "NAN");
}

//baudrate attr
static struct kobj_attribute baudrateAttr = __ATTR(baudrate, 0664, showBaudRate, storeBaudRate);



static struct attribute *attrs0[] = { &mode0Attr.attr, &cs_auto0Attr.attr, &cs_enable0Attr.attr, NULL};
static struct attribute *attrs1[] = { &mode1Attr.attr, &cs_auto1Attr.attr, &cs_enable1Attr.attr, NULL};
static struct attribute *attrs2[] = { &mode2Attr.attr, &cs_auto2Attr.attr, &cs_enable2Attr.attr, NULL};
static struct attribute *attrs3[] = { &mode3Attr.attr, &cs_auto3Attr.attr, &cs_enable3Attr.attr, NULL};

static struct attribute_group group0 = 
{
	.name = "device0",
	.attrs = attrs0
};

static struct attribute_group group1 =
{
	.name = "device1",
	.attrs = attrs1
};

static struct attribute_group group2 =
{
	.name = "device2",
	.attrs = attrs2
};

static struct attribute_group group3 =
{
	.name = "device3",
	.attrs = attrs3
};


static struct kobject *kobj;

//init kernel moduel
static int __init initialize_module(void)
{
	int result;
	printk(KERN_INFO "SPI driver: starting\n");
	kobj = kobject_create_and_add("spi", kernel_kobj);
	
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

	//add single sysfs files
	result = sysfs_create_file(kobj, &baudrateAttr.attr); //baudrate
	result = sysfs_create_file(kobj, &wordsizeAttr.attr); //wordSize
	result = sysfs_create_file(kobj, &cs_selectAttr.attr); //wordSize
	result = sysfs_create_file(kobj, &TxDataAttr.attr); //Txdata
	result = sysfs_create_file(kobj, &RxDataAttr.attr); //Rxdata
	
	//add grouped sysfs files

	base = (unsigned int*)ioremap_nocache(LW_BRIDGE_BASE + SPI_BASE_OFFSET, SPAN_IN_BYTES);

	if(base == NULL)
		return -ENODEV;
	printk(KERN_INFO "SPI driver: initialized\n");

	return 0;

}

static void __exit exit_module(void)
{
	kobject_put(kobj);
	sysfs_remove_file(kernel_kobj, &baudrateAttr.attr);
	sysfs_remove_file(kernel_kobj, &wordsizeAttr.attr);
	sysfs_remove_file(kernel_kobj, &cs_selectAttr.attr);
	sysfs_remove_file(kernel_kobj, &TxDataAttr.attr);
	sysfs_remove_file(kernel_kobj, &RxDataAttr.attr);
	printk(KERN_INFO "SPI driver: exiting");
}

module_init(initialize_module);
module_exit(exit_module);
