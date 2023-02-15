`timescale 1ns / 1ps

module TB_comparison();

logic [8:0] a;
logic [8:0] b;

logic less;
logic greater;

cmp9 DUT(.a(a), .b(b), .less(less), .greater(greater));

initial begin
    {a, b} = 18'b0; #1;

    repeat(262144) begin
        {a, b} = {a, b} + 1; #1;
    end
$finish;
end

endmodule
