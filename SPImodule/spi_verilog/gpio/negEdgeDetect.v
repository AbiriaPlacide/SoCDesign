module negEdgeDetect(input clk, input reset, input signal, output out_pulse);
	 //posedge detector
	 reg old_signal;
	 always @(posedge clk)
	 begin
		if(reset)
			old_signal <= 1'b0;
		else
			old_signal <= signal;
	 end
	 
	 assign out_pulse = ~signal & old_signal;

endmodule
