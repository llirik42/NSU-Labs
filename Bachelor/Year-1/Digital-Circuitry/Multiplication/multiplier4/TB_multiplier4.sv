`timescale 1ns / 1ps

module TB_multiplier4();
    logic clk;
    logic start;
    logic [3:0] a;
    logic [3:0] b;
    logic [7:0] p;
    logic ready;
    
    multiplier4 DUT(.clk(clk), .start(start), .a(a), .b(b), .p(p), .ready(ready));
    
    always begin
        clk = 0; #5;
        clk = 1; #5;
    end
    
    initial begin
        #5;
    
        {a, b} = 4'b0000; start = 1; #10;
        start = 0; #40;
         
        repeat (255) begin
            {a, b} = {a, b} + 1; start = 1; #10;
            start = 0; #40;
        end
        $finish;
    end
endmodule
