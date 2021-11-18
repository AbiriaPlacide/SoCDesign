    /*assignment bits for CTR_REG
    wire [4:0] WORD_SIZE  = DATA_REG[4:0];
    wire CS0_AUTO             = DATA_REG[5];
    wire CS1_AUTO           = DATA_REG[6];
    wire CS2_AUTO             = DATA_REG[7];
    wire CS3_AUTO           = DATA_REG[8];
    wire CS0_ENABLE         = DATA_REG[9];
    wire CS1_ENABLE          = DATA_REG[10];
    wire CS2_ENABLE          = DATA_REG[11];
    wire CS3_ENABLE          = DATA_REG[12];
    wire [1:0]CS_SELECT   = DATA_REG[14:13];
    wire ENABLE              = DATA_REG[15];
    wire [1:0] MODE0      = DATA_REG[17:16];
    wire [1:0] MODE1      = DATA_REG[19:18];
    wire [1:0] MODE2      = DATA_REG[21:20];
    wire [1:0] MODE3      = DATA_REG[23:22];
    wire [7:0] DEBUG_OUT  = DATA_REG[31:24];
    */



module spi_module(clk, reset, read, write, chipselect, address, writedata,readdata, byteenable, baudrate,spi_clk, spi_tx, spi_rx,spi_cs0,spi_cs1,spi_cs2,spi_cs3);
    
    
    //clock(50mhz in top.v), reset
    
    input clk, reset;

    //avalon memory interface
    input read, write, chipselect;
    input [1:0] address;
    input [31:0] byteenable;
    input [31:0]  writedata;
    output reg [31:0]  readdata;
    //spi output pins
    
    output reg baudrate; //GPIO_0[1]
    output reg spi_tx;   //GPIO_0[7]
    output reg spi_clk;  //GPIO_0[11]
    output reg spi_cs0;  //GPIO_0[13]
    output reg spi_cs1;  //GPIO_0[15]
    output reg spi_cs2;  //GPIO_0[17]
    output reg spi_cs3;  //GPIO_0[19]
    
    //spi input pins
    input      spi_rx;   //GPIO_0[9]
    
    
    // internal registers
    reg [31:0] data; //gpio interface for spi ( TX,RX, , CSn)
    reg [31:0] status;     //r,W1C
    reg [31:0] control;    //rw
    reg [31:0] brd;        //rw
    
    
    //register numbers
    parameter DATA_REG    = 2'b00;
    parameter STATUS_REG  = 2'b01;
    parameter CTRL_REG    = 2'b10;
    parameter BRD_REG     = 2'b11;
        
    //altera_edge_detector write_pulse_edge(.clk(clk), .rst_n(reset), .signal_in(write), .pulse_out(write_pulse));
    
    //altera_edge_detector read_pulse_edge(.clk(clk), .rst_n(reset), .signal_in(read), .pulse_out(read_pulse));
    
    //reading from the registers
    always @(*)
    begin
        if(read && chipselect)
        begin
            case(address)
                DATA_REG:
                    readdata = data;
                STATUS_REG:
                    readdata = status;
                CTRL_REG:
                    readdata = control;
                BRD_REG:
                    readdata = brd;
            endcase
        end
        
        else
            readdata = 32'b0;
    end
    
    //write to regisers
    always @(posedge clk)
    begin
        if(reset)
        begin
            //data <= 32'b0;
            //status <= 32'b0;
            control <= 32'b0;
            brd <= 32'b0;
        end
        
        else
        begin
            if(write && chipselect)
            begin
                case(address)
                    //DATA_REG:
                        //data = writedata;
                    //STATUS_REG:
                         //status = writedata;
                    CTRL_REG:
                        control = writedata;
                    BRD_REG:
                        brd = writedata;
                endcase
            end
        end
    
    
    end
	 
	     //Rx, Tx fifo status bits
    wire  TXFF, TXFO, TXFE;
    wire  RXFF, RXFO, RXFE;

	 always @ (posedge clk)
	 begin
		status[5:0] <= {TXFE, TXFF, TXFO, RXFE, RXFF, RXFO};
    end

	
    //write 1 clear to status register logic for overflow bit
    
	 //positive edge pulse logic (write pulse)
	 reg old_write;
	 always @(posedge clk)
	 begin
		if(write && chipselect)
		begin
			if(address == DATA_REG)
				old_write <= write;
		end
	 end
	 assign write_pulse = write & ~old_write;
	 
	 
	 //positive edge pulse logic (read pulse)
	 reg old_read;
	 always @(posedge clk)
	 begin
		if(read && chipselect)
		begin
			if(address == DATA_REG)   
				old_read <= read;
		end
	 end
	 assign read_pulse = read & ~old_read;
    
	 
	 reg clear_overflow; //will clear the overflow in fifo if set
	 
	 always @(posedge clk)
	 begin
		if(TXFO)
			clear_overflow <= 1;
		else
			clear_overflow <= 0;
	 end
	 
    //4 spi components
    // BRD divider instance
	 
	 
    
	 baudratedivider baud_out(.clock(clk), .enable(control[15]), .reset(reset), .N(brd[31:0]), .Nout(spi_clk));
	 
	 //altera_edge_detector(.clk(spi_clk), .rst_n(~reset), .signal_in(write), .pulse_out(write_pulse_clk));
        
		  
	assign fifo_clk = spi_clk;
        //Transmit FIFO
    FIFO txFIFO(.Clock(fifo_clk), .Full(TXFF), .EMPTY(TXFE), .OV(TXFO), .Read(read_pulse), .Write(write_pulse), .DataIn(writedata), .DataOut(data), .Reset(reset), .ClearOV(clear_overflow));
    
        
        
endmodule




