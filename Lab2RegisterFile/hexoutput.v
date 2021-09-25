module hexoutput (X0, Enter0, Reset_Lock0, Open0, State0);
parameter WIDTH0 = 4; //

//inputs, outputs
input X0, Enter0, Reset_Lock0;
output Open0;
output [WIDTH0-1:0] State0;

//wires needed to see output

wire [WIDTH0-1:0] dut_out;
wire dut_open;


DigitalLockController lock(   .X(X0), 
										.Enter(Enter0), 
										.Reset_Lock( Reset_Lock0), 
										.Open(  dut_open), 
										.State( dut_out)
									);
assign Open0 = dut_open;
assign State0 = dut_out;

//binary2seven hex0(.BIN(State));

endmodule
