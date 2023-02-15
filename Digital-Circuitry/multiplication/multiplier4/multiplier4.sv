`timescale 1ns / 1ps

module multiplier4(
    input clk,
    input start,
    input [3:0] a,
    input [3:0] b,
    output [7:0] p,
    output ready
    );
    
    logic [3:0] a_d;
    logic [7:0] b_d;
    logic [7:0] p_d;
    
    always_ff @(posedge clk) begin
        if (start) begin
            a_d <= a;
            b_d <= {4'b0000, b};
            p_d <= 0;   
        end
        else begin
            if (a_d[0]) begin
                p_d = p_d + b_d;
            end 
               
            a_d <= {1'b0, a_d[3:1]};
            b_d <= {b_d[6:0], 1'b0};
        end    
    end    
    
    assign p = p_d;
    assign ready = (!a_d);
    
endmodule          