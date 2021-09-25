`timescale 1ns/1ns

module tb_EightxOneMux;

reg  [2:0] Read;
reg  [3:0] Din0,Din1,Din2,Din3,Din4,Din5,Din6,Din7; //4 registers of size 8 bits
wire [3:0] Output;

EightxOneMux show(
    .Din0(Din0),
    .Din1(Din1),
    .Din2(Din2),
    .Din3(Din3),
    .Din4(Din4),
    .Din5(Din5),
    .Din6(Din6),
    .Din7(Din7),
    .RA(Read),
    .RDout(Output)
);

initial
begin
    Din0 = 10;
    Din1 = 9;
    Din2 = 8;
    Din3 = 7;
    Din4 = 6;
    Din5 = 5;
    Din6 = 4;
    Din7 = 3;
    
    #20;
    Read = 0;
    #20;
    Read = 1;
    #20;
    Read = 2;
    #20;
    Read = 3;
    #20;
    Read = 4;

end
endmodule

