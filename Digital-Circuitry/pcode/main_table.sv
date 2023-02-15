`timescale 1ns / 1ps


module main(
    input [7:0] in,
    output [2:0] code
    );
    assign code[0] = in[7] | (!in[6] & in[5]) | (!in[6] & !in[5] & !in[4] & in[3]) | 
                     (!in[6] & !in[5] & !in[4] & !in[3] & !in[2] & in[1]);
    assign code[1] = in[7] | in[6] | (!in[5] & !in[4] & (in[3] | in[2]));
    assign code[2] = in[7] | in[6] | in[5] | in[4];
    
endmodule
