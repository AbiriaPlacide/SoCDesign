module FIFO #(parameter FIFOSIZE = 16, FIFOWIDTH = 32)
(				input Read, 
				input Write, 
				input Clock, 
				input Reset, 
				input ClearOV,
				input [FIFOWIDTH-1:0] DataIn, 
				output reg [FIFOWIDTH-1:0] DataOut, 
				output Full,
				output reg OV, //registered
				output EMPTY,
				output reg [3:0] ReadPtr,
				output reg [3:0] WritePtr,
				input [1:0] address,
				input chipselect,
				input READ_DONE
);

reg [FIFOWIDTH-1:0] STACK [FIFOSIZE-1:0]; //Storage elements
reg [4:0] FIFO_Counter = 0;


assign Full  = (FIFO_Counter >= 5'd16 ) ? 1'b1:1'b0; // if full counter >= 16
assign EMPTY = (FIFO_Counter == 1'b0) ? 1'b1 : 1'b0; //if empty , counter = 0

//overflow bit set when writing to an already full fifo
reg Overflow;

always @(posedge Clock, posedge Reset)
begin
	if(Reset)
		begin 
			FIFO_Counter <= 0;
			ReadPtr <= 0; //reset readpointer
			WritePtr <= 0; // reset writeptr
			Overflow <= 0;
		end
	else
		begin //beginning of state machine				
			if( ((Read) && (!EMPTY) ))//(address == 2'b00) && chipselect == 1'b1) )
				begin 
					if(!OV)
						begin
							ReadPtr <= ReadPtr + 1'b1;
							FIFO_Counter <= FIFO_Counter - 1'b1; //subtract 1 count on each read
						end
				end
			 else if((Write && !Overflow) ) //what to do on a write
				 begin
					if(!Full)
						begin
							STACK[WritePtr] <= DataIn;
							WritePtr <= WritePtr + 1'b1;
							FIFO_Counter <= FIFO_Counter + 1'b1;
						end
					else
						begin
							//if writing to full FIFO, then trigger overflow
							Overflow <= 1;
						end
				 end 
			else
			begin
				Overflow <= 0;
			end
		end //end of else
end //end of block

//always have data on coming out of FIFO or its gonna latch zeros. 
always @(posedge Clock)
begin
	if(Reset)
		DataOut <= 0;
	else 
		DataOut <= STACK[ReadPtr];
end
//overflow logic
always @(posedge Clock) 
begin
	if (Overflow)
		begin
			OV <= 1;
		end
	else 
	begin
		if(ClearOV)
		begin
			OV <= 0;
		end
		else
		begin
			OV <= OV;
		end
	end

end

endmodule


