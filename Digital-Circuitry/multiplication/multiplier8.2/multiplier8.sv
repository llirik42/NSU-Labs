`timescale 1ns / 1ps

module ShiftRegister(
    input clk,
    input load,
    input [10:0] di,
    output logic [10:0] q
    );
                              
   always @(posedge clk) begin
       if (load) q <= di;
       else q <= {q[9:0], 1'b0};   
   end          
endmodule

module Multiplier8(
    input clk,
    input start,
    input [7:0] a,
    input [7:0] b,
    output [15:0] p,
    output ready
    );
         
    logic [15:0] p_d;
    logic [10:0] b_d;
    
    enum logic [2:0] {st0, st1, st2, st3, st4, st5} state;
        
    ShiftRegister sr(.load(state == st0), .clk(clk), .di({3'b000, b}), .q(b_d));
    
    always_ff @(posedge clk) begin
        if (start) begin
            p_d <= 0;
            state <= st0;
        end
        else begin
           if ((state == st1 & a[0]) | (state == st2 & a[1]) | (state == st3 & a[2]) | (state == st4 & a[3])) begin
               p_d += {5'b00000, b_d};
           end
        
            if ((state == st1 & a[4]) | (state == st2 & a[5]) | (state == st3 & a[6]) | (state == st4 & a[7])) begin
               p_d += {1'b0, b_d, 4'b0000};
           end
        end
    end    
      
    always_ff @(posedge clk) begin
        case(state)
            st0: state <= st1;
            st1: state <= st2;
            st2: state <= st3;
            st3: state <= st4;
            st4: state <= st5;
        endcase    
    end
     
    assign p = p_d;
    assign ready = (state == st5);
endmodule            