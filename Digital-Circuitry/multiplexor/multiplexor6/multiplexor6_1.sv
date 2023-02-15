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
    
    multiplexor3 m0(.in(in[7:0]),   .sel(sel[2:0]), .out(tmp[0]));
    multiplexor3 m1(.in(in[15:8]),  .sel(sel[2:0]), .out(tmp[1]));
    multiplexor3 m2(.in(in[23:16]), .sel(sel[2:0]), .out(tmp[2]));
    multiplexor3 m3(.in(in[31:24]), .sel(sel[2:0]), .out(tmp[3]));
    multiplexor3 m4(.in(in[39:32]), .sel(sel[2:0]), .out(tmp[4]));
    multiplexor3 m5(.in(in[47:40]), .sel(sel[2:0]), .out(tmp[5]));
    multiplexor3 m6(.in(in[55:48]), .sel(sel[2:0]), .out(tmp[6]));
    multiplexor3 m7(.in(in[63:56]), .sel(sel[2:0]), .out(tmp[7]));
    
    multiplexor3 m8(.in({tmp[7], tmp[6], tmp[5], tmp[4], tmp[3], tmp[2], tmp[1], tmp[0]}), .sel(sel[5:3]), .out(out));
endmodule
