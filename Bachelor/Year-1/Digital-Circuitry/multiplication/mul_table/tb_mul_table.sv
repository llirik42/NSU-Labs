`timescale 1ns / 1ps

module TB_add_table();

logic [3:0] a, b;
logic [7:0] m;
logic clk;

ADD_TABLE DUT(.a(a), .b(b), .clk(clk), .m(m));

always begin
    clk = 0; #5;
    clk = 1; #5;
end

initial begin
    {b, a} = 0; #10;
    
    repeat (255) begin
        {b, a} = {b, a} + 1; #10;
    end

$finish;
end

endmodule
