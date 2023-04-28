`timescale 1ns / 1ps

module multiplexor3(
    input [7:0] in,
    input [2:0] sel,
    output logic out
    );
    
    always_comb
        if      (sel == 3'b000) out = in[0];
        else if (sel == 3'b001) out = in[1];
        else if (sel == 3'b010) out = in[2];
        else if (sel == 3'b011) out = in[3];
        else if (sel == 3'b100) out = in[4];
        else if (sel == 3'b101) out = in[5];
        else if (sel == 3'b110) out = in[6];
        else                    out = in[7];
endmodule