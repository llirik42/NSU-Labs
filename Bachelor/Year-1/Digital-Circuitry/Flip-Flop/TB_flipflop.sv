`timescale 1ns / 1ps

module TB_flipflop();

logic clk, d, clr, ena, q;

flipflop_sync DUT(.clk(clk), .d(d), .ena(ena), .clr(clr), .q(q));

always begin
    clk = 1; #10;
    clk = 0; #10;
end

initial begin
    //clr = 0;
    clr = 1;
    d = 1; ena = 0; #15;
    d = 0; ena = 1; #20;
    d = 1; ena = 1; #20;
    d = 1; ena = 0; clr = 0; #20;
    d = 0; ena = 1; #20;
    d = 1; #20;
    
$finish;
end


endmodule
