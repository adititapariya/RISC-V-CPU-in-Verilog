`timescale 1 ps/1 ps
module ripple_carry_adder (
    input [1:0] a, b,
    input cin, 
    output [1:0] sum,
    output c_out 
);
wire c1;

full_adder FA0 (a[0], b[0], cin, sum[0], c1); // instantiate full_adder (FA0)
full_adder FA1 (a[1], b[1], c1, sum[1], c_out); // instantiate full_adder (FA1)

endmodule