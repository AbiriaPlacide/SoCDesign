// Divide by N clock generator
module baudratedivider (enable, clock, reset, N, Nout, count, target);
    input clock, reset;
    input [31:0] N;
    input enable;
    output reg Nout;
    output reg [31:0] count;
    output reg [31:0] target;
    
    wire SOURCE_CLK = enable && clock; //& or &&
    
    always @ (posedge SOURCE_CLK, posedge reset)
    begin
        if (reset)
        begin
            Nout <= 1'b0;
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


