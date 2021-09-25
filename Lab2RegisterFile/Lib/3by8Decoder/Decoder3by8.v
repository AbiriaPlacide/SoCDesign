module Decoder3by8(
    input   [2:0] usr_input,
    input ENABLE, 
    output  reg [7:0] Dout
    );

always @(usr_input)
begin
    Dout = 8'd0; //will reset the Dout to 0 for a new selection

    if(ENABLE)
    begin
        case(usr_input)
            3'd0: begin Dout[0] = 1'd1; end
            3'd1: begin Dout[1] = 1'd1; end
            3'd2: begin Dout[2] = 1'd1; end
            3'd3: begin Dout[3] = 1'd1; end
            3'd4: begin Dout[4] = 1'd1; end
            3'd5: begin Dout[5] = 1'd1; end
            3'd6: begin Dout[6] = 1'd1; end
            3'd7: begin Dout[7] = 1'd1; end

            default : Dout = 8'd0;
        endcase
    end
end

endmodule
