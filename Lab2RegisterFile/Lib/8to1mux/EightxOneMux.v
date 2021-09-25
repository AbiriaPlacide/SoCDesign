module EightxOneMux #(localparam WIDTH=2, DEPTH = 4)(
    input      [2:0] RA, 
    input reg  [DEPTH-1:0] Din0,Din1,Din2,Din3,Din4,Din5,Din6,Din7, //4 registers of size 8 bits
    output reg [DEPTH-1:0] RDout
);

always @(RA)
begin
    RDout = 4'd0; //reset on every read select
    case (RA)
        3'd0: begin RDout = Din0; end
        3'd1: begin RDout = Din1; end
        3'd2: begin RDout = Din2; end
        3'd3: begin RDout = Din3; end
        3'd4: begin RDout = Din4; end
        3'd5: begin RDout = Din5; end
        3'd6: begin RDout = Din6; end
        3'd7: begin RDout = Din7; end

        default: RDout = 4'd0;
    endcase

end
endmodule