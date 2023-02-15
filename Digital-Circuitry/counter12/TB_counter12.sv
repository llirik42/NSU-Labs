`timescale 1ns / 1ps


module TB_counter12();
    logic clk;
    logic [3:0] q = 0;

    Counter12 DUT(.clk(clk), .q(q));

    always begin
        clk = 1; #10;
        clk = 0; #10;
     end
endmodule
