`timescale 1ns / 1ps

module UpdownCounter(
    input ud,
    input clk,
    output logic [3:0] q
    );
    
    always @(posedge clk) begin
        q[0] <= !q[0];
    
        if ((ud & !q[0]) | (!ud & q[0])) begin 
            q[1] <= !q[1];
        end
        
        if ((ud & !q[0] & !q[1]) | (!ud & q[0] & q[1])) begin 
            q[2] <= !q[2];
        end
        
        if ((ud & !q[0] & !q[1] & !q[2]) | (!ud & q[0] & q[1] & q[2])) begin 
            q[3] <= !q[3];
        end  
    end 
endmodule
