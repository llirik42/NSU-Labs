`timescale 1ns / 1ps

module main(
    input minuend,
    input subtrahend,
    input carry_in,
    output difference,
    output carry_out
);

assign difference = (!minuend & !subtrahend & carry_in) |
                    (!minuend & subtrahend & !carry_in) |
                    (minuend & !subtrahend & !carry_in) |
                    (minuend & subtrahend & carry_in);

assign carry_out = (!minuend & !subtrahend & carry_in) |
                   (!minuend & subtrahend & !carry_in) |
                   (!minuend & subtrahend & carry_in) |
                   (minuend & subtrahend & carry_in);
                   
endmodule