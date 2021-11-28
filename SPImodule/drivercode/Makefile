CC=gcc
TARGET=spi
TARGET2=mcp

all: spi_utility.c
	@echo "--building spi utility & MCP-23SO8--"
	$(CC) spi_utility.c spi_ip.c -o $(TARGET)
	$(CC) MCP23S08.c spi_ip.c -o $(TARGET2)

