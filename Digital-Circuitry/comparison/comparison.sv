`timescale 1ns / 1ps

module merge(
    input [1:0] l, 
    input [1:0] g,
    output less,
    output greater
    );

    assign less =    l[1] | (!l[1] & !g[1] & l[0]);
    assign greater = g[1] | (!l[1] & !g[1] & g[0]);
endmodule
    
    
module cmp2(
    input [1:0] a,
    input [1:0] b,
    output less,
    output greater
    );
        
    assign less = (!a[1] & b[1]) |
                  (!a[1] & !b[1] & !a[0] & b[0]) |
                  (a[1] & b[1] & !a[0] & b[0]); 
    
    assign greater = (a[1] & !b[1]) |
                     (!a[1] & !b[1] & a[0] & !b[0]) |
                     (a[1] & b[1] & a[0] & !b[0]); 
                     
endmodule
    
    
module cmp4(
    input [3:0] a,
    input [3:0] b,
    output less,
    output greater
    );
    
    logic [1:0] l;
    logic [1:0] g;
    
    cmp2 c20(.a(a[1:0]), .b(b[1:0]), .less(l[0]), .greater(g[0]));
    cmp2 c21(.a(a[3:2]), .b(b[3:2]), .less(l[1]), .greater(g[1]));
    
    merge m0(.l(l), .g(g), .less(less), .greater(greater));
endmodule
    
    
module cmp8(
    input [7:0] a,
    input [7:0] b,
    output less,
    output greater
    );
        
    logic [1:0] l;
    logic [1:0] g;
    
    cmp4 c40(.a(a[3:0]), .b(b[3:0]), .less(l[0]), .greater(g[0]));
    cmp4 c41(.a(a[7:4]), .b(b[7:4]), .less(l[1]), .greater(g[1]));
    
    merge m1(.l(l), .g(g), .less(less), .greater(greater));
endmodule

    
module cmp3(
    input [2:0] a,
    input [2:0] b,
    output less,
    output greater
    );

    logic [1:0] l;
    logic [1:0] g;

    cmp2 c22(.a(a[2:1]), .b(b[2:1]), .less(l[1]), .greater(g[1]));

    assign l[0] = !a[0] & b[0];
    assign g[0] = a[0] & !b[0];

    merge m3(.l(l), .g(g), .less(less), .greater(greater));
endmodule
    

module cmp6(
    input [5:0] a,
    input [5:0] b,
    output less,
    output greater
    );
    
    logic [1:0] l;
    logic [1:0] g;
    
    cmp3 c30(.a(a[2:0]), .b(b[2:0]), .less(l[0]), .greater(g[0]));
    cmp3 c31(.a(a[5:3]), .b(b[5:3]), .less(l[1]), .greater(g[1]));
    
    merge m4(.l(l), .g(g), .less(less), .greater(greater));
endmodule


module cmp9(
    input [8:0] a,
    input [8:0] b,
    output less,
    output greater
    );
    
    logic [1:0] l;
    logic [1:0] g;
    
    cmp3 c31(.a(a[2:0]), .b(b[2:0]), .less(l[0]), .greater(g[0]));
    cmp6 c60(.a(a[8:3]), .b(b[8:3]), .less(l[1]), .greater(g[1]));
    
    merge m5(.l(l), .g(g), .less(less), .greater(greater));
endmodule
