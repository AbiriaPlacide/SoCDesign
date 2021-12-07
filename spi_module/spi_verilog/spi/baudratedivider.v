// Divide by N clock generator
module baudratedivider (enable, clock, reset, N, Nout, count, target, State, idle_mode);
    input clock, reset;
    input [31:0] N;
    input enable;
	 input [3:0] State;
	 input idle_mode;
    output reg Nout;
    output reg [31:0] count;
    output reg [31:0] target;
    
    wire SOURCE_CLK 		= enable && clock;
	 parameter IDLE      = 4'b1010; //10 0xA
    
    always @ (posedge SOURCE_CLK)
    begin
        if (reset)
        begin
            Nout <= 0;
            count <= 32'b0;
            target <= N;
        end
        else
        begin
            count <= count + 8'b10000000; // only add to the integer portion not the fractional bits, bit 7. So comparison is for bits [31:6] = integerpart
            if (count[31:7] == target[31:7])
                begin
                target <= target + N;
                Nout <= ~Nout;
                end
            else Nout <= Nout;
        end
    end    
endmodule


