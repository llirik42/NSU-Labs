`timescale 1ns / 1ps


module TB_dlatch();

logic s, c, r, qn, q;

dlatch DUT(.s(s), .c(c), .r(r), .qn(qn), .q(q));

initial begin
    c = 0; s = 0; r = 0; #30;

    s = 1; r = 1; #20;
    s = 0; #10;
    
    s = 1; #10;
    r = 0; #10;
    
    c = 1; #10;
    r = 0; s = 0; #30;
    r = 1; #10;
    s = 0; r = 0; #10;
    
    s = 1; r = 1; #10;
    
$finish;
end

endmodule
