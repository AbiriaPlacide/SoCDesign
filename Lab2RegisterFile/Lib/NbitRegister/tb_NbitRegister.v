`timescale 1ns/1ns

module tb_NbitRegister;

localparam N = 4;
reg [N-1:0] Data_in;
wire [N-1:0] Data_out;
reg clock;
reg clear;


NbitRegister reg0(.Data_in(Data_in),
                  .Data_out(Data_out),
                  .Clock(clock),
                  .Clear(clear)
                  );
//

initial 
begin
    Data_in = 0;
    clear = 1;
    #20;

    Data_in = 16;

    #20;

    Data_in = 10;
    #20;
    Data_in = 9;
    #20;
    clear = 0;
    #20;

    Data_in = 5;
    #20;

end

always
begin
    clock = 1'b1;

    #10; //wait 20ns

    clock = 1'b0;

    #10; //wait 20ns
end

endmodule
