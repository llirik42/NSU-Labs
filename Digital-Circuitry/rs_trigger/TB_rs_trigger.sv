`timescale 1ns / 1ps

module TB_rs_trigger();

logic r, s, q, qn;

rs DUT(.r(r), .s(s), .q(q), .qn(qn));

initial begin
    r = 0; s = 0; #35;

    r = 1; # 15;

    r = 0; #5;

    s = 1; #15;

    r = 1; s = 0; #15;

    r = 0; s = 0; #10;
    
    r = 1; s = 1; #5;
    
    
$finish;
end

endmodule
