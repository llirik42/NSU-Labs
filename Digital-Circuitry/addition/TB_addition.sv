`timescale 1ns / 1ps

module TB_addition();
    logic [63:0] a;
    logic [63:0] b;
    logic carry_in;
    logic [63:0] sum;
    logic carry_out;

    addition64 DUT(.a(a), .b(b), .carry_in(carry_in), .sum(sum), .carry_out(carry_out));

    initial begin
        a = 64'h0000000000000000; b = 64'h0000000000000000; carry_in = 0; #10;
        a = 64'h0000000000000000; b = 64'h0000000000000000; carry_in = 1; #10;
        
        a = 64'h0000000000000000; b = 64'hffffffffffffffff; carry_in = 0; #10;
        a = 64'h0000000000000000; b = 64'hffffffffffffffff; carry_in = 1; #10;
    
        a = 64'hffffffffffffffff; b = 64'h0000000000000000; carry_in = 0; #10;
        a = 64'hffffffffffffffff; b = 64'h0000000000000000; carry_in = 1; #10;
                
        a = 64'hffffffffffffffff; b = 64'hffffffffffffffff; carry_in = 0; #10;
        a = 64'hffffffffffffffff; b = 64'hffffffffffffffff; carry_in = 1; #10;
        
        a = 64'h5555555555555555; b = 64'haaaaaaaaaaaaaaaa; carry_in = 0; #10;
        a = 64'h5555555555555555; b = 64'haaaaaaaaaaaaaaaa; carry_in = 1; #10;
    $finish;
    end


endmodule
