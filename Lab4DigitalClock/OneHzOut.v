module OneHzOut(
	input CLK_MHZ_50, Clear,
	output [7:0] COUNT,
	output ONE_HZ_PULSE
);


wire TEN_MHZ_PULSE, ONE_MHZ_PULSE, TEN_KHZ_PULSE, ONE_KHZ_PULSE, ONEHUNDRED_KHZ_PULSE, TEN_HZ_PULSE, ONEHUNDRED_HZ_PULSE;
// 50 mhz -> 10 Mhz -> 1 MHZ ->  1 Hz.

//instantuations

divideByFifty(
	.CLEAR(Clear),
	.CLK(CLK_MHZ_50),
	.OUT(ONE_MHZ_PULSE),
);

divideByTen(
	.CLEAR(Clear),
	.CLK(ONE_MHZ_PULSE),
	.OUT(ONEHUNDRED_KHZ_PULSE),
);



divideByTen(
	.CLEAR(Clear),
	.CLK(ONEHUNDRED_KHZ_PULSE),
	.OUT(TEN_KHZ_PULSE),

);


divideByTen(
	.CLEAR(Clear),
	.CLK(TEN_KHZ_PULSE),
	.OUT(ONE_KHZ_PULSE)
);

divideByTen(
	.CLEAR(Clear),
	.CLK(ONE_KHZ_PULSE),
	.OUT(ONEHUNDRED_HZ_PULSE)
);

divideByTen(
	.CLEAR(Clear),
	.CLK(ONEHUNDRED_HZ_PULSE),
	.OUT(TEN_HZ_PULSE)
);

divideByTen(
	.CLEAR(Clear),
	.CLK(TEN_HZ_PULSE),
	.OUT(ONE_HZ_PULSE),
	.COUNT(COUNT) 					//1 HZ output
);


endmodule
