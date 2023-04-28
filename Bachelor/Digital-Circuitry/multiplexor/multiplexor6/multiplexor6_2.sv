`timescale 1ns / 1ps

module multiplexor3(
    input [7:0] in,
    input [2:0] sel,
    output out
    );
        
     assign out = (!sel[2] & !sel[1] & !sel[0] & in[0]) |
                  (!sel[2] & !sel[1] &  sel[0] & in[1]) |
                  (!sel[2] &  sel[1] & !sel[0] & in[2]) |
                  (!sel[2] &  sel[1] &  sel[0] & in[3]) |
                  ( sel[2] & !sel[1] & !sel[0] & in[4]) |
                  ( sel[2] & !sel[1] &  sel[0] & in[5]) |
                  ( sel[2] &  sel[1] & !sel[0] & in[6]) |
                  ( sel[2] &  sel[1] &  sel[0] & in[7]);        
endmodule


module multiplexor6(
    input [63:0] in,
    input [5:0] sel,
    output out
    );    
    
    logic tmp[7:0];
    
    genvar i;
        generate
        for (i = 0; i < 8; i++)
        begin: label
            multiplexor3 multiplexor(.in(in[i*8+7:i*8]), .sel(sel[2:0]), .out(tmp[i])); 
        end
        endgenerate       
    
    multiplexor3 m8(.in({tmp[7], tmp[6], tmp[5], tmp[4], tmp[3], tmp[2], tmp[1], tmp[0]}), .sel(sel[5:3]), .out(out));
endmodule
