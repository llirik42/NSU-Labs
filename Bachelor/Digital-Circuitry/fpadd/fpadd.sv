`timescale 1ns / 1ps

module pcode(
    input [23:0] in,
    output logic [4:0] code
    );
    always_comb
        casex(in) 
            24'b1xxxxxxxxxxxxxxxxxxxxxxx : code = 0;
            24'b01xxxxxxxxxxxxxxxxxxxxxx : code = 1;
            24'b001xxxxxxxxxxxxxxxxxxxxx : code = 2;
            24'b0001xxxxxxxxxxxxxxxxxxxx : code = 3;
            24'b00001xxxxxxxxxxxxxxxxxxx : code = 4;
            24'b000001xxxxxxxxxxxxxxxxxx : code = 5;
            24'b0000001xxxxxxxxxxxxxxxxx : code = 6;
            24'b00000001xxxxxxxxxxxxxxxx : code = 7;
            24'b000000001xxxxxxxxxxxxxxx : code = 8;
            24'b0000000001xxxxxxxxxxxxxx : code = 9;
            24'b00000000001xxxxxxxxxxxxx : code = 10;
            24'b000000000001xxxxxxxxxxxx : code = 11;
            24'b0000000000001xxxxxxxxxxx : code = 12;
            24'b00000000000001xxxxxxxxxx : code = 13;
            24'b000000000000001xxxxxxxxx : code = 14;
            24'b0000000000000001xxxxxxxx : code = 15;
            24'b00000000000000001xxxxxxx : code = 16;
            24'b000000000000000001xxxxxx : code = 17;
            24'b0000000000000000001xxxxx : code = 18;
            24'b00000000000000000001xxxx : code = 19;
            24'b000000000000000000001xxx : code = 20;
            24'b0000000000000000000001xx : code = 21;
            24'b00000000000000000000001x : code = 22;
            24'b000000000000000000000001 : code = 23;
        endcase;
endmodule 

module exp_comparison(
    input [7:0] exp1,
    input [7:0] exp2,
    output [7:0] shift1,
    output [7:0] shift2
    );

    assign shift1 = exp1 < exp2 ? exp2 - exp1 : 8'b00000000;
    assign shift2 = exp2 < exp1 ? exp1 - exp2 : 8'b00000000;
endmodule

module fpadd(
    input [31:0] a,
    input [31:0] b,
    output [31:0] s
    );
    
    logic [31:0] MINUS_INF;
    logic [31:0] PLUS_INF;
    logic [31:0] NAN;
    logic [31:0] MAX;
      
    logic sgn1;
    logic sgn2;
    logic [7:0] exp1;
    logic [7:0] exp2;
    logic [22:0] fraction1;
    logic [22:0] fraction2;
    
    assign sgn1 = a[31];
    assign sgn2 = b[31];
    assign exp1 = a[30:23];
    assign exp2 = b[30:23];
    assign fraction1 = a[22:0];
    assign fraction2 = b[22:0];
    
    logic is_a_zero;
    logic is_b_zero;
    logic is_a_special;
    logic is_b_special;
    logic is_a_nan;
    logic is_b_nan;
    logic is_a_plus_inf;
    logic is_a_minus_inf;
    logic is_b_plus_inf;
    logic is_b_minus_inf;

    assign MINUS_INF = 32'b11111111100000000000000000000000;
    assign PLUS_INF = 32'b01111111100000000000000000000000;
    assign NAN = 32'b01111111100000000000000000000001;
    assign MAX = 32'b01111111111111111111111111111111;

    assign is_a_zero = a[30:0] == 31'b0000000000000000000000000000000;
    assign is_b_zero = b[30:0] == 31'b0000000000000000000000000000000;
    assign is_a_special = exp1 == 8'b11111111;
    assign is_b_special = exp2 == 8'b11111111;
    assign is_a_nan = is_a_special & fraction1 != 23'b00000000000000000000000;
    assign is_b_nan = is_b_special & fraction2 != 23'b00000000000000000000000;
    assign is_a_plus_inf = is_a_special & !sgn1 & fraction1 == 23'b00000000000000000000000;
    assign is_b_plus_inf = is_a_special & !sgn2 & fraction2 == 23'b00000000000000000000000;
    assign is_a_minus_inf = is_a_special & sgn1 & fraction1 == 23'b00000000000000000000000;
    assign is_b_minus_inf = is_a_special & sgn2 & fraction2 == 23'b00000000000000000000000;
    
    /////////////////////////////////////////////////////
    
    logic zero_result; // будет ли результат в точности = 0
    logic bitFromRounding; // 0 или 1, которые нужно прибавить к мантиссе результата из-за округления
    
    logic [7:0] shift1; // насколько нужно сдвинуть первую мантиссу, чтобы экспоненты стали одинаковыми 
    logic [7:0] shift2; // насколько нужно сдвинуть вторую мантиссу, чтобы экспоненты стали одинаковыми 
    logic [4:0] result_shift; // насколько нужно сдвинуть мантиссу результата для нормализации
    
    logic [23:0] mantissa1; // мантисса первого числа с учётом сдвига
    logic [23:0] mantissa2; // мантисса второго числа с учётом сдвига
    logic [23:0] min_mantissa; // наименьшая мантисса
    logic [23:0] max_mantissa; // наибольшая мантисса
    logic [24:0] mantissas_addition_result; // результат сложения мантисс
    
    logic result_sgn; // знак результата
    logic [7:0] result_exp; // экспонента результата
    logic [22:0] result_fraction; // дробь результата
    
    logic is_result_minus_inf;
    logic is_result_plus_inf;
    logic is_result_nan;
    logic is_result_number;
    logic is_result_max;
    
    exp_comparison c0(.exp1(exp1), .exp2(exp2), .shift1(shift1), .shift2(shift2));
    
    assign bitFromRounding = mantissas_addition_result[24] ? mantissas_addition_result[0] : 0;
    
    assign mantissa1 = {1, fraction1} >> shift1;
    assign mantissa2 = {1, fraction2} >> shift2;
    assign min_mantissa = mantissa1 < mantissa2 ? mantissa1 : mantissa2;
    assign max_mantissa = mantissa1 > mantissa2 ? mantissa1 : mantissa2;
     
    assign mantissas_addition_result = (!(sgn1 ^ sgn2)) * (mantissa1 + mantissa2) +
                           (sgn1 ^ sgn2) * (max_mantissa - min_mantissa);
        
    assign result_fraction = bitFromRounding + ( mantissas_addition_result[24]) * mantissas_addition_result[23:1] +
                                               (!mantissas_addition_result[24]) * (mantissas_addition_result[22:0] << result_shift);
                             
    assign zero_result = ((sgn1 ^ sgn2) & mantissa1 == mantissa2) | (is_a_zero & is_b_zero);
    
    pcode pcode(.in(mantissas_addition_result[23:0]), .code(result_shift));
    
    assign result_exp = ( zero_result) * 0 +
                        (!zero_result & mantissas_addition_result[24]) * (exp1 + shift1 + 1) +
                        (!zero_result & !mantissas_addition_result[24]) * (exp1 + shift1 - result_shift);
    
    assign result_sgn = ((sgn1 == sgn2) & sgn1) | 
                        (!sgn1 &  sgn2 & mantissa1 < mantissa2) | 
                        ( sgn1 & !sgn2 & mantissa2 < mantissa1);
    
    assign is_result_minus_inf = (is_a_minus_inf & !is_b_plus_inf & !is_b_nan) | 
                                 (is_b_minus_inf & !is_a_plus_inf & !is_a_nan);
    
    assign is_result_plus_inf = (is_a_plus_inf & !is_b_minus_inf & !is_b_nan) | 
                                (is_b_plus_inf & !is_a_minus_inf & !is_a_nan);
    
    assign is_result_nan = (is_a_nan | is_b_nan) | 
                           (is_a_plus_inf & is_b_minus_inf) | 
                           (is_a_minus_inf & is_b_plus_inf);
    
    assign is_result_number = !is_a_special & !is_b_special;

    assign is_result_max = (255 - exp1 - shift1 < result_shift) & is_result_number;
        
    assign s = is_result_minus_inf * MINUS_INF + 
               is_result_plus_inf * PLUS_INF + 
               is_result_nan * NAN + 
               is_result_max * MAX + 
               (is_result_number & !is_result_max) * {result_sgn, result_exp, result_fraction};
    
endmodule
