`timescale 1ns / 1ps


module TB_main();

logic [7:0] in;
logic [2:0] code;

main DUT(.in(in), .code(code));

initial begin
    in = 8'b00000000; #1;

    repeat (255) begin
        in = in + 1; #1;
    end

$finish;
end

endmodule
