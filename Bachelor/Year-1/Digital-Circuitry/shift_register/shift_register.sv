`timescale 1ns / 1ps


module ShiftRegister(
    input [7:0] di,
    input load,
    input clk,
    output logic [7:0] q
    );
                                  
   always @(posedge clk) begin
       if (load) q <= di;
       else q <= {q[6:0], q[7]};   
   end          
endmodule
