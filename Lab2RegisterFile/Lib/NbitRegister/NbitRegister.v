module NbitRegister #(localparam N = 4)
( input [N-1:0] Data_in,
  input Clock, Clear,
  output reg [N-1:0] Data_out
);

always @(negedge Clock, negedge Clear)
begin
    if(Clear == 1'b0) Data_out <= 0;
    else if(Clock== 1'b0) Data_out <= Data_in;
end

endmodule