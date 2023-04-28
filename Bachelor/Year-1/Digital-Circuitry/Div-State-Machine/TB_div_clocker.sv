`timescale 1ns / 1ps

module TB_div_clocker();

logic clk;
logic [2:0] div;
logic q;

DIVCLOCKER DUT(.clk(clk), .div(div), .q(q));

always begin
    clk = 1; #10;
    clk = 0; #10;
end

initial begin
    div = 2; #300;
    div = 3; #300;
    div = 4; #300;
    div = 5; #300;
    div = 6; #300;
    div = 7; #300;
$finish;
end

endmodule
