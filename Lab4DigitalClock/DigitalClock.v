module DigitalClock #(parameter M = 8) 
(
	output [0:6] Hex0, Hex1, Hex2, Hex3, Hex4, Hex5,
	input Reset,CLK_50_MHZ_GEN,
	input Time_Stop, Time_Set,
	input  [9:4] SW,
	output [3:0] COUNT_TenthsOfSecond,
	output [3:0] COUNT_SixthofTenthsOfSecond, 
	output [3:0] COUNT_TensOfMinutes,
	output [3:0] COUNT_SixTenthsOfMinutes,
	output [3:0] COUNT_TenthOfMinutes,
	output [3:0] COUNT_TenthofHours,
	output [3:0] COUNT_SixTenthofMinutes,
	output seconds
	
);



wire CLK_50MHZ;
assign CLK_50MHZ = (Time_Stop & CLK_50_MHZ_GEN);


wire TenthsOfseconds_CLK;
wire SixthOfTenthsOfSecond_CLK;
wire TenthOfMinutes_CLK;
wire SixTenthsOfMinutes_CLK;
wire TenthofHours_CLK;
wire SixTenthofHours_CLK;

OneHzOut onehertz (
	.CLK_MHZ_50(CLK_50MHZ),
	.Clear(Reset),
	.ONE_HZ_PULSE(seconds),
//	.COUNT(COUNT),
);

/*start of seconds*/
divideByTen tenthsOfseconds(
	.CLEAR(Reset),
	.CLK(seconds),
	.OUT(TenthsOfseconds_CLK),
	.COUNT(COUNT_TenthsOfSecond)
);

divideBySix sixthOf10thsOfSecond(
	.CLK(TenthsOfseconds_CLK),
	.CLEAR(Reset),
	.OUT(SixthOfTenthsOfSecond_CLK),
	.COUNT(COUNT_SixthofTenthsOfSecond)
);

/*start of minutes*/

divideByTen tenthofminutes(
	.CLEAR(Reset),
	.CLK(SixthOfTenthsOfSecond_CLK),
	.OUT(TenthOfMinutes_CLK),
	.COUNT(COUNT_TenthOfMinutes)
);

divideBySix SixTenthsOfMinutes(
	.CLEAR(Reset),
	.CLK(TenthOfMinutes_CLK),
	.OUT(SixTenthsOfMinutes_CLK),
	.COUNT(COUNT_SixTenthsOfMinutes)
);

/*start of hours*/

divideByTen tenthofhours (
	.CLEAR(Reset),
	.CLK(COUNT_SixTenthsOfMinutes),
	.OUT(TenthofHours_CLK),
	.COUNT(COUNT_TenthofHours)
	
);

divideBySix sixTenthofhours (
	.CLEAR(Reset),
	.CLK(TenthofHours_CLK),
	.OUT(SixTenthofHours_CLK),
	.COUNT(COUNT_SixTenthofMinutes)
);

/*start of display outputs */

/*seconds out */
binary2seven Hex0Display(
	.BIN(COUNT_TenthsOfSecond),
	.SEV(Hex0)

);

binary2seven Hex1Display(
	.BIN(COUNT_SixthofTenthsOfSecond),
	.SEV(Hex1)
);

/*minutes outs */
binary2seven Hex2Display(
	.BIN(COUNT_TenthOfMinutes),
	.SEV(Hex2)
);

binary2seven Hex3Display(
	.BIN(COUNT_SixTenthsOfMinutes),
	.SEV(Hex3)
);

/*hours out*/
binary2seven Hex4Display(
	.BIN(COUNT_TenthofHours),
	.SEV(Hex4)
);

binary2seven Hex5Display(
	.BIN(COUNT_SixTenthofMinutes),
	.SEV(Hex5)
);

endmodule

