`timescale 1ns / 1ps


module TB_multiplexor();

logic [7:0] in;
logic [2:0] sel;
logic out;

multiplexor3 DUT(.in(in), .sel(sel), .out(out));    

initial begin
    in = 8'b00000001; sel = 3'b000; #1;
    repeat (7) begin
       sel = sel + 1; in[7:0] = {in[6:0], 1'b0}; #1;
    end

    in = 8'b11111110; sel = 3'b000; #1;
    repeat (7) begin
       sel = sel + 1; in[7:0] = {in[6:0], 1'b1}; #1;
    end
$finish;
end

endmodule
