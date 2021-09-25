`timescale 1 ns/1ns;

module tb_Decoder3by8;
    
reg  [2:0] tb_usr_input;
reg tb_ENABLE;
wire [7:0] tb_Dout;
reg [2:0] count = 3'd0;

//instantiate module

Decoder3by8 uut( .usr_input(tb_usr_input), 
                 .Dout(tb_Dout),
                 .ENABLE(tb_ENABLE)
            );

initial 
begin
    $display($time, " << starting simulation >> ");

    tb_usr_input = 3'd0;
    tb_ENABLE  = 1'b1;

    #10;


 for(count = 0; count < 8; count =1'b1)
    begin
	$display("\ninput is : %b", tb_usr_input);
        tb_usr_input = tb_usr_input + 1'b1;

        #20;
    end

    tb_ENABLE = 1'b0;
end
endmodule

