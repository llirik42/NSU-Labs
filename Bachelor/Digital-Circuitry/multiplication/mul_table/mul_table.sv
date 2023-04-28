`timescale 1ns / 1ps

module ADD_TABLE(
    input [3:0] a,
    input [3:0] b,
    output logic [7:0] m,
    input clk
    );
    
    logic [8:0] address;
    logic [8:0] ram[255:0]; //random access memory
  
    initial $readmemb("table.mem", ram, 0, 255);
  
    assign address = {b, a};
  
    always_ff @(posedge clk) begin
        m <= ram[address];
    end
    
endmodule
