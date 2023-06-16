`timescale 1ns / 1ps


module main(
    input [7:0] in,
    output logic [2:0] code
    );
    
    always_comb
        if (in[7])      code = 3'b111;
        else if (in[6]) code = 3'b110;
        else if (in[5]) code = 3'b101;
        else if (in[4]) code = 3'b100;
        else if (in[3]) code = 3'b011;
        else if (in[2]) code = 3'b010;
        else if (in[1]) code = 3'b001;
        else            code = 3'b000;
endmodule
