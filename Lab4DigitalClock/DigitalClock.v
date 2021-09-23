module DigitalClock(
	output [0:6] Hex0, Hex1, Hex2, Hex3, Hex4, Hex5,
	input Reset,CLK_50_MHZ_GEN,
	input Time_Stop, Time_Set
	
);



reg [7:0] minutes;
reg [7:0] hours;
reg [7:0] seconds;

wire CLK_50MHZ;

assign CLK_50MHZ = (Time_Stop & CLK_50_MHZ_GEN);



//need divide by n counters


endmodule

