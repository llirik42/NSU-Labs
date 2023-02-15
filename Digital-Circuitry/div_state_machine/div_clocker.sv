`timescale 1ns / 1ps

module DIVCLOCKER(
    input clk,
    input [2:0] div,
    output q
    );
    
    enum logic [2:0] {st0, st1, st2, st3, st4, st5, st6} state = st0;

    always @(posedge clk) begin
        case(state)
            st0: state <= st1;
            
            st1: if(div == 2) state <= st0;
                 else              state <= st2;
            
            st2: if(div == 3) state <= st0;
                 else              state <= st3;
            
            st3: if(div == 4) state <= st0;
                 else              state <= st4;
            
            st4: if(div == 5) state <= st0;
                 else              state <= st5;
            
            st5: if(div == 6) state <= st0;
                 else              state <= st6;
            
            st6: state <= st0;
    
            endcase
    end
  
    assign q = ((div == 2 | div == 3) &  state == st0) | 
               ((div == 4 | div == 5) & (state == st0 | state == st1)) |
               ((div == 6 | div == 7) & (state == st0 | state == st1 | state == st2));
endmodule
