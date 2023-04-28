`timescale 1ns / 1ps


module TB_updown_counter();

    logic clk;
    logic ud;
    logic [3:0] q;
    UpdownCounter DUT(.clk(clk), .ud(ud), .q(q));
    
    always begin
        clk = 0; #10;
        clk = 1; #10;
    end
    
    initial begin
        q = 3'b000;
        ud = 0; #300;
        ud = 1; #300;
        ud = 0; #150;
        ud = 1; #150;
        ud = 0; #100;
        ud = 1; #50;
        ud = 0; #25;
        ud = 1; #25;
    end
endmodule
