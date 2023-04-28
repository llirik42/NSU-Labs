`timescale 1ns / 1ps

module TB_main();

logic minuend, subtrahend, carry_in;

main DUT(.minuend(minuend), .subtrahend(subtrahend), .carry_in(carry_in));

initial begin
    minuend = 1'b0; subtrahend = 1'b0; carry_in = 1'b0; #10;
    minuend = 1'b0; subtrahend = 1'b0; carry_in = 1'b1; #10;
    minuend = 1'b0; subtrahend = 1'b1; carry_in = 1'b0; #10;
    minuend = 1'b0; subtrahend = 1'b1; carry_in = 1'b1; #10;
    minuend = 1'b1; subtrahend = 1'b0; carry_in = 1'b0; #10;
    minuend = 1'b1; subtrahend = 1'b0; carry_in = 1'b1; #10;
    minuend = 1'b1; subtrahend = 1'b1; carry_in = 1'b0; #10;
    minuend = 1'b1; subtrahend = 1'b1; carry_in = 1'b1; #10;

$finish;
end

endmodule
