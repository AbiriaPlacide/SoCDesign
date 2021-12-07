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


/*
uint32_t getStatusReg()
{
    uint32_t value = *(base+OFS_STATUS);
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


//enable transmit/receiver/brd
void setCtrlEnableBit(bool bit)
{
    //this will turn on transmitter/receiver/brd
	if(bit)
		*(base+OFS_CTRL) |= (1 << 15);
	else
		*(base+OFS_CTRL) &= ~(1 << 15);
}

*/ 


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


//baud_rate
static uint32_t baudrate = 0;
module_param(baudrate, int, S_IRUGO);
MODULE_PARM_DESC(baudrate, "set baudrate");

static ssize_t storeBaudRate(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &baudrate);
	if(result == 0)
		setBaudrateReg(baudrate);

	return count;
}

static ssize_t showBaudRate(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	baudrate = getBaudrateReg();
	return sprintf(buffer, "%u\n", baudrate);
}

//baudrate attr
static struct kobj_attribute baudrateAttr = __ATTR(baudrate, 0664, showBaudRate, storeBaudRate);

//word size
static uint32_t word_size = 0;
module_param(word_size, int, S_IRUGO);
MODULE_PARM_DESC(word_size, "set word_size");

static ssize_t showWordSize(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	word_size = getControlReg();
	word_size &= 0x1F; 
	word_size++;
	return sprintf(buffer, "%u\n", word_size);
} 

static ssize_t storeWordSize(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &word_size);
	unsigned int value = getControlReg();

	if(result == 0)
	{
		word_size  = word_size - 1;
		value &= ~(0x1F); //clear first 5 bits
		value |= word_size;
		iowrite32(value, (base+OFS_CTRL));
	}

	return count;
}

//word size attr
static struct kobj_attribute wordsizeAttr = __ATTR(word_size, 0664, showWordSize, storeWordSize);

//chip select
static uint32_t cs_select = 0;
module_param(cs_select, int, S_IRUGO);
MODULE_PARM_DESC(cs_select, "set cs_select");
static ssize_t showCS_Select(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_select = getControlReg();
	cs_select = (cs_select >> 13) & 0x3;
	return sprintf(buffer, "%u\n", cs_select);
} 

static ssize_t storeCS_Select(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &cs_select);
	unsigned int value = getControlReg();

	if(result == 0)
	{
		value &= ~(0x6000); //clear  bits 14 and 13
		value |= cs_select << 13;
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

//Chip select Attr
static struct kobj_attribute cs_selectAttr = __ATTR(cs_select, 0664, showCS_Select, storeCS_Select);

//tx_data
static uint32_t tx_data = 0;
module_param(tx_data, int, S_IRUGO);
MODULE_PARM_DESC(tx_data, "set tx_data");

static ssize_t storeTxData(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &tx_data);
	if(result == 0)
	{
		setDataReg(tx_data);
	}
	return count;
}

static ssize_t showTxData(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	return sprintf(buffer, "%s\n", "NAN");
}
//TxData attribute
static struct kobj_attribute TxDataAttr = __ATTR(tx_data, 0664, showTxData, storeTxData);


//rx_data
static uint32_t rx_data = 0;
module_param(rx_data, int, S_IRUGO);
MODULE_PARM_DESC(rx_data, "set rx_data");

static ssize_t storeRxData(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &rx_data);
	if(result == 0);

	return count;	
}

static ssize_t showRxData(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	bool p = getRxEmpty();
	if(p)
	{
		return sprintf(buffer, "%s\n", "-1");
	}
	else
	{
		uint32_t data = getDataReg();
		return sprintf(buffer, "%u\n", data);
	}
}
//RxData attribute
static struct kobj_attribute RxDataAttr = __ATTR(rx_data, 0664, showRxData, storeRxData);


//dev 0
//dev0 mode
static uint32_t mode = 0;
module_param(mode, int, S_IRUGO);
MODULE_PARM_DESC(mode, "set mode");

static ssize_t dev0showMode(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	mode = getControlReg();
	mode = (mode >> 16) & 0x3;

	return sprintf(buffer, "%u\n", mode);
}

static ssize_t dev0storeMode(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &mode);
	unsigned int value = getControlReg();

	if(result == 0)
	{
		value &= ~(3 << 16); //clear
		value |= (mode << 16); //set
		iowrite32(value, (base+OFS_CTRL));
	}

	return count;
}
//attributes
static struct kobj_attribute mode0Attr = __ATTR(mode, 0664, dev0showMode, dev0storeMode);


//dev0 cs_enable
static uint32_t cs_enable = 0;
module_param(cs_enable, int, S_IRUGO);
MODULE_PARM_DESC(cs_enable, "set cs_auto");
static ssize_t dev0showcs_enable(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_enable = getControlReg();
	cs_enable = (cs_enable >> 9) & 0x1;

	if(cs_enable)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_enable)
	{
		strcpy(buffer, "false\n");
	}

	return strlen(buffer);
	
}

static ssize_t dev0storecs_enable(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if(strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 9); //set
		iowrite32(value, (base+OFS_CTRL));
	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 9); //clear
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

static struct kobj_attribute cs_enable0Attr = __ATTR(cs_enable, 0664, dev0showcs_enable, dev0storecs_enable);

//dev0 cs_auto
static uint32_t cs_auto = 0;
module_param(cs_auto, int, S_IRUGO);
MODULE_PARM_DESC(cs_auto, "set cs_auto");
static ssize_t dev0showcs_auto(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_auto = getControlReg();
	cs_auto = (cs_auto >> 5) & 0x1;

	if(cs_auto)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_auto)
	{
		strcpy(buffer, "false\n");
	}

	return strlen(buffer);
}

static ssize_t dev0storecs_auto(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if(strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 5); //set
		iowrite32(value, (base+OFS_CTRL));

	}
	else if(strncmp(buffer, "false", count-1)== 0)
	{
		value &= ~(1 << 5); //clear
		iowrite32(value, (base+OFS_CTRL));
	}

	return count;
}

static struct kobj_attribute cs_auto0Attr = __ATTR(cs_auto, 0664, dev0showcs_auto, dev0storecs_auto);

//Dev 1

static ssize_t dev1showMode(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	mode = getControlReg();
	mode = (mode >> 18) & 0x3;

	return sprintf(buffer, "%u\n", mode);
}

static ssize_t dev1storeMode(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &mode);
	unsigned int value = getControlReg();

	if(result == 0)
	{
		value &= ~(3 << 18); //clear
		value |= (mode << 18); //set
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

static struct kobj_attribute mode1Attr = __ATTR(mode, 0664, dev1showMode, dev1storeMode);

static ssize_t dev1showcs_enable(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_enable = getControlReg();
	cs_enable = (cs_enable >> 10) & 0x1;

	if(cs_enable)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_enable)
	{
		strcpy(buffer, "false\n");
	}

	return strlen(buffer);
}

static ssize_t dev1storecs_enable(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if(strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 10); //set
		iowrite32(value, (base+OFS_CTRL));
	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 10); //clear
		iowrite32(value, (base+OFS_CTRL));	
	}

	return count;
}

static struct kobj_attribute cs_enable1Attr = __ATTR(cs_enable, 0664, dev1showcs_enable, dev1storecs_enable);

//Dev 1 CS auto
static ssize_t dev1showcs_auto(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_auto = getControlReg();
	cs_auto = (cs_auto >> 6) & 0x1;

	if(cs_auto)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_auto)
	{
		strcpy(buffer, "false\n");
	}


	return strlen(buffer);
}

static ssize_t dev1storecs_auto(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();


	if(strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 6); //set
		iowrite32(value, (base+OFS_CTRL));
	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 6); //clear
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

static struct kobj_attribute cs_auto1Attr = __ATTR(cs_auto, 0664, dev1showcs_auto, dev1storecs_auto);

//DEV 2 mode

static ssize_t dev2showMode(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	mode = getControlReg();
	mode = (mode >> 20) & 0x3;

	return sprintf(buffer, "%u\n", mode);
}

static ssize_t dev2storeMode(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &mode);
	unsigned int value = getControlReg();

	if(result == 0)
	{
		value &= ~(3 << 20); //clear
		value |= (mode << 20); //set
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

static struct kobj_attribute mode2Attr = __ATTR(mode, 0664, dev2showMode, dev2storeMode);


//DEV 2 cs_enable
static ssize_t dev2showcs_enable(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_enable = getControlReg();
	cs_enable = (cs_enable >> 11) & 0x1;

	if(cs_enable)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_enable)
	{
		strcpy(buffer, "false\n");
	}

	return strlen(buffer);
}

static ssize_t dev2storecs_enable(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if(strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 11); //set
		iowrite32(value, (base+OFS_CTRL));

	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 11); //clear
		iowrite32(value, (base+OFS_CTRL));
	}

	return count;
}

static struct kobj_attribute cs_enable2Attr = __ATTR(cs_enable, 0664, dev2showcs_enable, dev2storecs_enable);


//Dev 2 CS auto
static ssize_t dev2showcs_auto(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_auto = getControlReg();
	cs_auto = (cs_auto >> 7) & 0x1;

	if(cs_auto)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_auto)
	{
		strcpy(buffer, "false\n");
	}

	return strlen(buffer);
}

static ssize_t dev2storecs_auto(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if(strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 7); //set
		iowrite32(value, (base+OFS_CTRL));
	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 7); //clear
		iowrite32(value, (base+OFS_CTRL));
	}

	return count;
}

static struct kobj_attribute cs_auto2Attr = __ATTR(cs_auto, 0664, dev2showcs_auto, dev2storecs_auto);


//DEV 3 mode
static ssize_t dev3showMode(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	mode = getControlReg();
	mode = (mode >> 22) & 0x3;

	return sprintf(buffer, "%u\n", mode);
}

static ssize_t dev3storeMode(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	int result = kstrtouint(buffer, 0, &mode);
	unsigned int value = getControlReg();

	if(result == 0)
	{
		value &= ~(3 << 22); //clear
		value |= (mode << 22); //set
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

static struct kobj_attribute mode3Attr = __ATTR(mode, 0664, dev3showMode, dev3storeMode);

//DEV 3 cs_enable
static ssize_t dev3showcs_enable(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_enable = getControlReg();
	cs_enable = (cs_enable >> 12) & 0x1;

	if(cs_enable)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_enable)
	{
		strcpy(buffer, "false\n");
	}

	return strlen(buffer);
}

static ssize_t dev3storecs_enable(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if( strncmp(buffer, "true", count-1) == 0)
	{
		value |=  (1 << 12); //set
		iowrite32(value, (base+OFS_CTRL));
	
	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 12); //clear
		iowrite32(value, (base+OFS_CTRL));
	}
	return count;
}

static struct kobj_attribute cs_enable3Attr = __ATTR(cs_enable, 0664, dev3showcs_enable, dev3storecs_enable);


//Dev 3 CS auto
static ssize_t dev3showcs_auto(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	cs_auto = getControlReg();
	cs_auto = (cs_auto >> 8) & 0x1;

	if(cs_auto)
	{
		strcpy(buffer, "true\n");
	}

	else if(!cs_auto)
	{
		strcpy(buffer, "false\n");
	}
	return strlen(buffer);
}

static ssize_t dev3storecs_auto(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	unsigned int value = getControlReg();

	if( strncmp(buffer, "true", count-1) == 0)
	{
		value |= (1 << 8); //set
		iowrite32(value, (base+OFS_CTRL));
	}

	else if(strncmp(buffer, "false", count-1) == 0)
	{
		value &= ~(1 << 8); //clear
		iowrite32(value, (base+OFS_CTRL));
	}

	return count;
}

static struct kobj_attribute cs_auto3Attr = __ATTR(cs_auto, 0664, dev3showcs_auto, dev3storecs_auto);


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
