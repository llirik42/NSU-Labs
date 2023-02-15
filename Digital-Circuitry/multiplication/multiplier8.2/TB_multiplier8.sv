`timescale 1ns / 1ps

module TB_multiplier8();
    logic clk;
    logic start;
    logic [7:0] a;
    logic [7:0] b;
    logic [15:0] p;
    logic ready;
    
    Multiplier8 DUT(.clk(clk), .start(start), .a(a), .b(b), .p(p), .ready(ready));
    
    always begin
        clk = 0; #5;
        clk = 1; #5;
    end
    
    initial begin
        #5;
    
        {a, b} = 0; start = 1; #10;
        start = 0; #50;
         
        repeat (65535) begin
            {a, b} = {a, b} + 1; start = 1; #10;
            start = 0; #50;
        end
        $finish;
    end
endmodule
