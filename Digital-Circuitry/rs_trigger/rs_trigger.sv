`timescale 1ns / 1ps

module rs(
    input r,
    input s,
    output q,
    output qn
    );
    
    assign qn = !(s | q);
    assign q = !(r | qn);
    endmodule
