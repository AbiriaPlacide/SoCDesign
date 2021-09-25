module RegisterFile #(parameter WIDTH = 4, DEPTH = 8) (
	
	input [WIDTH-1:0] Din,
	input [2:0] WA, RA,
	input RST, CLK, WR_ENABLE,
	output [WIDTH-1:0] Dout,
	output [0:6] Hex0

	);



/* wires and registers holding data being read */

//wire Decode0, Decode1, Decode3;
wire [7:0] SelectReg;
wire [3:0] Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7; //register outputs if selected

/* Instantiations */
Decoder3by8 decoder(
	.usr_input(WA),
	.ENABLE(WR_ENABLE),
	.Dout(SelectReg) //wrong?
);


NbitRegister Reg0(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[0]),
	.Data_out(Q0)
);

NbitRegister Reg1(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[1]),
	.Data_out(Q1)
);

NbitRegister Reg2(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[2]),
	.Data_out(Q2)
);

NbitRegister Reg3(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[3]),
	.Data_out(Q3)
);

NbitRegister Reg4(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[4]),
	.Data_out(Q4)
);

NbitRegister Reg5(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[5]),
	.Data_out(Q5)
);


NbitRegister Reg6(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[6]),
	.Data_out(Q6)
);
NbitRegister Reg7(
	.Data_in(Din),
	.Clock(CLK),
	.Clear(RST),
	.WR_ENABLE(SelectReg[7]),
	.Data_out(Q7)
);


EightxOneMux Mux0(
	.Din0(Q0),
	.Din1(Q1),
	.Din2(Q2),
	.Din3(Q3),
	.Din4(Q4),
	.Din5(Q5),
	.Din6(Q6),
	.Din7(Q7),
	.RA(RA),
	.RDout(Dout)
);

binary2seven hex0(
	.BIN(Dout), 
	.SEV(Hex0)
);


//4  y
endmodule
