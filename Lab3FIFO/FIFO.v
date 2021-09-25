/* Author: Abiria Placide
	

*/

module FIFO #(parameter FIFOSIZE = 16, FIFOWIDTH = 9)
(				input Read, 
				input Write, 
				input Clock, 
				input Reset, 
				input ClearOV,
				input [FIFOWIDTH-1:0] DataIn, 
				output reg [FIFOWIDTH-1:0] DataOut, 
				output Full,
				output reg OV,
				output EMPTY,
				output reg [3:0] ReadPtr,
				output reg [3:0] WritePtr,
				output [0:6] hexOutput0, 
				output [0:6] hexOutput2,
				output [0:6] hexOutput3
);


reg [FIFOWIDTH-1:0] STACK [FIFOSIZE-1:0]; //Storage elements
reg [4:0] FIFO_Counter = 0;


assign Full  = (FIFO_Counter >= 5'd16 ) ? 1'b1:1'b0; // if full counter >= 16
assign EMPTY = (FIFO_Counter == 1'b0) ? 1'b1 : 1'b0; //if empty , counter = 0

always @(negedge Clock, negedge Reset)
begin
	if(Reset == 0)
		begin 
			FIFO_Counter <= 0;
			DataOut <= 0; //clear output buffer
			ReadPtr <= 0; //reset readpointer
			WritePtr <= 0; // reset writeptr
		end
		
	else
		begin //beginning of state machine
			if(!Read & !EMPTY) //!READ because KEY2 is active-low| on a read and not empty
				begin 
					if(!OV)
						begin
							DataOut <= STACK[ReadPtr];
							ReadPtr <= ReadPtr + 1'b1;
							FIFO_Counter <= FIFO_Counter - 1'b1; //subtract 1 count on each read
						end
					else
						begin //not sure if this is necessary? do we want to do anything on read while overflowing
							DataOut <= STACK[ReadPtr];
							ReadPtr <= ReadPtr + 1'b1;		
							FIFO_Counter <= FIFO_Counter - 2'd2; //subtract 2 count on read when in overflow since counter  == 17
						end
				end
			 else if(!Write) //what to do on a write. !Write because active-low
				 begin
					if(!Full)
						begin
							STACK[WritePtr] <= DataIn;
							WritePtr <= WritePtr + 1'b1;
							FIFO_Counter <= FIFO_Counter + 1'b1;
						end
					else
						begin
							FIFO_Counter <= 5'd17; //if writing to full FIFO, then trigger overflow
						end
				 end
				 
				 else if(ClearOV) //clears overflow
					begin
						FIFO_Counter <= FIFO_Counter - 1'b1;
					end
			
		end //end of else wrapper
end

always @( * ) //overflow case
begin
	OV = 0;
	if (FIFO_Counter == 5'd17)
		begin
			OV = 1;
		end
end

//module instantiation
binary2seven hex0 (
	.BIN({Full,OV,EMPTY}),
	.SEV(hexOutput0)
);

binary2seven hex2 (
	.BIN(ReadPtr),
	.SEV(hexOutput2)
);

binary2seven hex3 (
	.BIN(WritePtr),
	.SEV(hexOutput3)
);

endmodule



