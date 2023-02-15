`timescale 1ns / 1ps

module carry_gen(
    input [3:0] p,
    input [3:0] g,
    input carry_in,
    output [4:0] carry,
    output p_group,
    output g_group
    );
    assign carry[0] = carry_in;
    assign carry[1] = g[0] | (p[0] & carry_in);
    assign carry[2] = g[1] | (p[1] & (g[0] | (p[0] & carry_in)));
    assign carry[3] = g[2] | (p[2] & (g[1] | (p[1] & (g[0] | (p[0] & carry_in)))));
    assign carry[4] = g[3] | (p[3] & g[2] | (p[2] & (g[1]| (p[1] & (g[0] | (p[0] & carry_in))))));
    
    assign p_group = p[3] & p[2] & p[1] & p[0];
    assign g_group = g[3] | p[3] & g[2] | p[3] & p[2] & g[1] | p[3] & p[2] &p[1] & g[0];
endmodule

module addition4(
    input [3:0] a,
    input [4:0] b,
    input carry_in,
    output [4:0] sum,
    output carry_out,
    output g_group,
    output p_group
    );
    
    logic [4:0] carry;
    logic [3:0] p;
    logic [3:0] g;
    
    genvar i;
    generate
    for (i = 0; i < 4; i++)
    begin: label
        assign sum[i] = a[i] ^ b[i] ^ carry[i];
        assign p[i] = a[i] ^ b[i];
        assign g[i] = a[i] & b[i];
    end
    endgenerate
    
    assign carry_out = carry[4];
    
    carry_gen cg0(.g(g),.p(p),.carry_in(carry_in),.carry(carry),.p_group(p_group),.g_group(g_group));
endmodule

module addition16(
    input [15:0] a,
    input [15:0] b,
    input carry_in,
    output [15:0] sum,
    output carry_out,
    output g_group,
    output p_group
    );
    
    logic [4:0] carry;
    logic [3:0] p;
    logic [3:0] g;
    assign carry_out = carry[4];
    genvar  i;
    generate
        for (i = 0; i < 4; i++)
        begin: label
            addition4 a0(
                         .a(a[3+4*i:4*i]), 
                         .b(b[3+4*i:4*i]),
                         .sum(sum[3+4*i:4*i]),
                         .carry_in(carry[i]), 
                         .carry_out(),
                         .p_group(p[i]),
                         .g_group(g[i])
                         );
        end
        endgenerate
    carry_gen cg1(.p(p), .g(g), .carry_in(carry_in), .carry(carry), .p_group(p_group), .g_group(g_group));
endmodule

module addition64(
    input [63:0] a,
    input [63:0] b,
    input carry_in,
    output [63:0] sum,
    output carry_out
    );
    
    logic [4:0] carry;
    logic [3:0] p;
    logic [3:0] g;
    
    assign carry_out = carry[4];
    
    genvar i;
    generate
        for(i = 0; i <4; i++)
        begin: label
            addition16 a1(
                          .a(a[15+16*i:i*16]),
                          .b(b[15+16*i:i*16]), 
                          .sum(sum[15+16*i:i*16]), 
                          .carry_in(carry[i]), 
                          .carry_out(), 
                          .p_group(p[i]),
                          .g_group(g[i])
                          );
        end
        endgenerate;
        
    carry_gen carryG(.p(p), .g(g), .carry_in(carry_in), .carry(carry), .p_group(), .g_group());
endmodule