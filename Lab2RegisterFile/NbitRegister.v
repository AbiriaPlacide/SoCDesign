module NbitRegister #(parameter N = 4)
( input [N-1:0] Data_in,
  input Clock, Clear, WR_ENABLE,
  output reg [N-1:0] Data_out
);

always @(negedge Clock )
begin
    if(WR_ENABLE == 1'b1)
    begin
        if(Clear == 1'b0) Data_out <= 0;
        else if(Clock== 1'b0) Data_out <= Data_in;
    end
end

endmodule
