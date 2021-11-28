


module spi_module(clk, reset, read, write, chipselect, address, writedata,readdata, byteenable, baudrate,spi_clk, spi_tx, spi_rx,spi_cs0,spi_cs1,spi_cs2,spi_cs3, HEX0, HEX1, HEX2, HEX3, HEX4, LEDR);

	 //signals for the control regiser
    wire [4:0] WORD_SIZE  = control[4:0];
    wire CS0_AUTO         = control[5];
    wire CS1_AUTO         = control[6];
    wire CS2_AUTO         = control[7];
    wire CS3_AUTO         = control[8];
	 assign CS_AUTO = {CS3_AUTO, CS2_AUTO, CS1_AUTO, CS0_AUTO};
	 
    wire CS0_ENABLE       = control[9];
    wire CS1_ENABLE       = control[10];
    wire CS2_ENABLE       = control[11];
    wire CS3_ENABLE       = control[12];
	 assign CS_ENABLE = {CS3_ENABLE, CS2_ENABLE, CS1_ENABLE, CS0_ENABLE};
    
	 wire [1:0]CS_SELECT   = control[14:13];
    wire ENABLE           = control[15];
    wire [1:0] MODE0      = control[17:16];
    wire [1:0] MODE1      = control[19:18];
    wire [1:0] MODE2      = control[21:20];
    wire [1:0] MODE3      = control[23:22];
	 assign MODE = {MODE3, MODE2, MODE1, MODE0};
    wire [7:0] DEBUG_OUT  = control[31:24];
	 //end of control reg signals
	 
	 wire [3:0] readptr;
	 wire [3:0] writeptr;
	 
	 //debug outputs
	 //hex ooutput
	 output [0:6] HEX0; //read pointer
	 output [0:6] HEX1; //write pointer
	 output [0:6] HEX2; //Status bits
	 output [0:6] HEX3; //data out of fifo
	 output [0:6] HEX4;
	 output [9:0] LEDR;
	 
    //clock(50mhz in top.v), reset
    input clk, reset;
	 
    //avalon memory interface
    input read, write, chipselect;
    input [1:0] address;
    input [31:0] byteenable;
    input [31:0]  writedata;
    output reg [31:0]  readdata;
    //spi output pins
    output reg baudrate; //GPIO_0[1]   Waveform Dx
    output reg spi_tx;   //GPIO_0[7]   Waveform D0
    output reg spi_clk;  //GPIO_0[11]  Waveform D2
    output reg spi_cs0;  //GPIO_0[13]  Waveform D3
    output reg spi_cs1;  //GPIO_0[15]  Waveform D4
    output reg spi_cs2;  //GPIO_0[17]  Waveform D5
    output reg spi_cs3;  //GPIO_0[19]  Waveform D6
    
    //spi input pins
    input      spi_rx;   //GPIO_0[9] Waveform D1
    
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
    
    //reading from the registers
	 
    always @(*)
    begin
        if(read && chipselect)
        begin
            case(address)
                DATA_REG:
                    readdata = data_temp;
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
	 reg [31:0] status_clear_req; //request to clear register
    always @(posedge clk)
    begin
        if(reset)
        begin
            data <= 32'b0;
            status_clear_req <= 32'b0;
            control <= 32'b0;
            brd <= 32'b0;
        end
        
        else
        begin
            if(write && chipselect)
            begin
                case(address)
                    DATA_REG:
                        data <= writedata;
                    STATUS_REG:
                         status_clear_req <= writedata;
                    CTRL_REG:
                        control <= writedata;
                    BRD_REG:
                        brd <= writedata;
                endcase
            end
				
				else
				begin
					status_clear_req <= 1'b0;
				end
        end
    
    end
	 
	 //Rx, Tx fifo status bits
    wire  TXFF, TXFO, TXFE;
    wire  RXFF, RXFO, RXFE;
	 
	 always @ (posedge clk or posedge reset)
	 begin
		if(reset)
			status[5:0] <= 1'b0;
		else
		begin
			status[5:0] <= {TXFE, TXFF, TXFO, RXFE, RXFF, RXFO};
		end
    end
	
    //write 1 clear to status register logic for txfo overflow bit
	 reg clear_overflow;

	 always @(posedge clk)
	 begin 
		if(reset)
			clear_overflow <=1'b0;
		else
		begin
			if(status_clear_req[3])
				clear_overflow <= 1'b1;
			else
				clear_overflow <= 1'b0;
		end
	 end
    
    
	 
	
	//BAUD OUT CLK.
	 baudratedivider baud_out(.clock(clk), .enable(control[15]), .reset(reset), .N(brd[31:0]), .Nout(spi_clk));
	
	
	//edge detect logic
	 wire read_pulse_clk;
	 wire write_pulse_clk;
	 edgeDetect readBlock(.clk(clk), .reset(reset), .signal(read), .chipselect(chipselect), .out_pulse(read_pulse_clk));
	 edgeDetect writeBlock(.clk(clk), .reset(reset), .signal(write), .chipselect(chipselect), .out_pulse(write_pulse_clk));

	
	 //Transmit FIFO
    reg [31:0] data_temp2 = 32'b11001110; //for testing purposes only
    reg [31:0] data_temp;
    FIFO txFIFO(.Clock(clk), 
					 .Full(TXFF), 
					 .EMPTY(TXFE), 
					 .OV(TXFO), 
					 .READ_DONE(READ_DONE), 
					 .Read(READ_DONE), 
					 .Write(write_pulse_clk), 
					 .DataIn(writedata), 
					 .DataOut(data_temp), 
					 .Reset(reset), 
					 .ClearOV(clear_overflow), 
					 .ReadPtr(readptr), 
					 .WritePtr(writeptr), 
					 .address(address), 
					 .chipselect(chipselect)
					 );
   
	//spi serializer logic
	
	 wire LOAD, DECREMENT;//, READ_DONE;
	 assign LOAD = (State == IDLE);
	 assign DECREMENT = (State == TX_RX) && (~spi_clk); //update count on the negedge of spi_clk= baud_out
	 //assign READ_DONE = (State == TX_RX) && (COUNT==0);
	
	 reg READ_DONE;

	  //counter block
	 
	 reg [4:0] COUNT;
	 
	 always @(posedge spi_clk)
	 begin
		if(reset)
			COUNT <= 1'b0;
		else
		begin 
			if(LOAD)
				COUNT <= control[4:0];
			else if(DECREMENT)
				COUNT <= COUNT- 1'b1;
		end
	end

	//spi serializer tx states
	 parameter IDLE      = 4'b1010; //10 0xA
	 parameter CS_ASSERT = 4'b1011; //11 0xB
	 parameter TX_RX     = 4'b1100; //12 0xC
	
	
	 reg [3:0] State;
	 reg [3:0] nextState;
	
	 always @(posedge clk)
	 begin
	 	if(reset || !ENABLE)
			State <= IDLE;
		else
			State <= nextState;
	 end
	
	 
	//functional block that will output cs output
	//enable control
	 always @(posedge spi_clk)
	 begin
		case(State)
			IDLE:
			begin
				//if in idle, cs idles high
				spi_cs0 <= 1'b1;
				spi_cs1 <= 1'b1;
				spi_cs2 <= 1'b1;
				spi_cs3 <= 1'b1;
			
				if(TXFE)
				begin
					nextState <= IDLE;
				end
				
				else if(!TXFE && !CS_AUTO) //if CS_ENABLE, then we know its in manual mode
				begin	
					nextState <= TX_RX;
				end
				
				else //automatic mode
					nextState <= CS_ASSERT;
			end
			
			TX_RX:
			begin
				//when count goes to  zero and in manual and empty go back to zero
				if(READ_DONE)
				begin
					nextState <= IDLE;
				end
				else
				begin
					if(CS0_ENABLE)
						spi_cs0 <= 1'b0;
					if(CS1_ENABLE)
						spi_cs1 <= 1'b0;
					if(CS2_ENABLE)
						spi_cs2 <= 1'b0;
					if(CS3_ENABLE)
						spi_cs3 <= 1'b0;
						
					nextState <= TX_RX;
					
				end
			end
			
			CS_ASSERT:
			begin
				//check which chip-selects needs to be driven low and do drive it low
				if(CS0_AUTO)
				begin
					spi_cs0 <= 1'b0;
				end
				
				else if(CS1_AUTO)
				begin
					spi_cs1 <= 1'b0;
				end
				
				else if(CS2_AUTO)
				begin
					spi_cs2 <= 1'b0;
				end
					
				else if(CS3_AUTO)
				begin
					spi_cs3 <= 1'b0;
				end
				
				nextState <= TX_RX; //after CS_ASSERT, go to tx_rx state
			end
			
		 endcase
	 end

	 //transmit logic to spi_tx pin
	 always @(negedge spi_clk)
	 begin
		 //read done is always 0 unless count == 0;
	 	 READ_DONE <= 1'b0;
		 
		 if(State == TX_RX)
		 begin
			spi_tx <= data_temp[COUNT];// if not +1, that last bit is lost
			
			if(COUNT == 1'b0)
			begin
				READ_DONE <= 1'b1;
				spi_tx <= data_temp[0];
			end
		 end
		 else
			spi_tx <= 0;
	 end
	 
	 //hex out for read ptr and write ptr and status bits for debugging
	 binary2seven hex0(.BIN(readptr), .SEV(HEX1)); //read ptr
	 binary2seven hex1(.BIN(writeptr),.SEV(HEX0)); //write ptr
	 binary2seven hex2 (.BIN({TXFF,TXFO,TXFE}), .SEV(HEX2) ); //
	 binary2seven hex3(.BIN(data_temp[3:0]),.SEV(HEX3));
	 binary2seven hex4(.BIN(State[3:0]), .SEV(HEX4));
	 
endmodule





