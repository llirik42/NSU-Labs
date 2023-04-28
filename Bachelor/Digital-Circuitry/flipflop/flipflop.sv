`timescale 1ns / 1ps

module flipflop_sync(
    input clk,
    input ena,
    input d,
    input clr,
    output logic q
    );
    
    always_ff @(posedge clk)
        if (clr)
            q <= 0;
        else if(ena)
            q <= d;
endmodule

module flipflop_async(
    input clk,
    input ena,
    input d,
    input clr,
    output logic q
    );
 
    always_ff @(posedge clk or posedge clr)
        if (clr)
            q <= 0;
        else if(ena)
            q <= d; 
endmodule