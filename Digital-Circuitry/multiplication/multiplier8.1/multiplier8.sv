`timescale 1ns / 1ps

module multiplier8(
    input clk,
    input start,
    input [7:0] a,
    input [7:0] b,
    output [15:0] p,
    output ready
    );
    
    logic [7:0] a_d;
    logic [15:0] b_d;
    logic [15:0] p_d;
    
    always_ff @(posedge clk) begin
        if (start) begin
            a_d <= a;
            b_d <= {8'b00000000, b};
            p_d <= 0;   
        end
        else begin
            if (a_d[0]) begin
                p_d = p_d + b_d;
            end 
               
            a_d <= {1'b0, a_d[7:1]};
            b_d <= {b_d[14:0], 1'b0};
        end    
    end    
    
    assign p = p_d;
    assign ready = (!a_d);
    
endmodule            