`timescale 1ns / 1ps


module TB_shift_register();

logic [7:0] di;
logic load;
logic clk;
logic [7:0] q;

ShiftRegister DUT(.di(di), .clk(clk), .load(load), .q(q));

always begin
    clk = 1; #10;
    clk = 0; #10;
end

initial begin
    q = 8'b01010101; load = 0; di = 8'b11110000; #100;
    load = 1; #40;
    load = 0; #100;
    
    
$finish;
end



endmodule
