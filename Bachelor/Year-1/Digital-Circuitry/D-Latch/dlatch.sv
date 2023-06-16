`timescale 1ns / 1ps

module dlatch(
    input s,
    input c,
    input r,
    output qn,
    output q
    );
    
    assign qn = !(q | (s & c));
    assign q = !(qn | (r & c));
endmodule
