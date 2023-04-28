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