`timescale 1 ps/1 ps
module full_adder (
    input a, b, c_in, 
    output sum , c_out 
);

assign sum = a^b^c_in;
assign c_out = ((c_in & (a^b)) | (a & b));

endmodule