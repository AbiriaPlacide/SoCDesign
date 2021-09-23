module OneHzOut(
	input CLK_MHZ_50, Clear
	output reg ONE_HZ_PULSE,
);


wire TEN_MHZ_PULSE, ONE_MHZ_PULSE, TEN_KHZ_PULSE, ONE_KHZ_PULSE, ONEHUNDRED_HZ_PULSE, TEN_HZ_PULSE;
// 50 mhz -> 10 Mhz -> 1 MHZ ->  1 Hz.

//instantuations

divideByFifty(
	.CLEAR(Clear),
	.CLOCK(CLK_MHZ_50)
);


endmodule
