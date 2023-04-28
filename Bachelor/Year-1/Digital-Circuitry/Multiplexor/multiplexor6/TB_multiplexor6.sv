`timescale 1ns / 1ps


module TB_multiplexor();

logic [63:0] in;
logic [5:0] sel;
logic out;

multiplexor6 DUT(.in(in), .sel(sel), .out(out));    

initial begin
    in = 64'b0000000000000000000000000000000000000000000000000000000000000001; 
    sel = 6'b000000; #1;
    repeat (63) begin
       sel = sel + 1; in[63:0] = {in[62:0], 1'b0}; #1;
    end

    in = 64'b1111111111111111111111111111111111111111111111111111111111111110; 
    sel = 6'b000000; #1;
    repeat (63) begin
       sel = sel + 1; in[63:0] = {in[62:0], 1'b1}; #1;
    end
$finish;
end

endmodule
