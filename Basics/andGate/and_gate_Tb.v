`timescale 1 ps/1 ps
module and_gate_Tb;

reg a, b;
wire out;

and_gate uut (.a(a), .b(b), .out(out));

initial begin
    a = 0; b = 0; #100;
    a = 0; b = 1; #100;
    a = 1; b = 0; #100;
    a = 1; b = 1; #100;
end

endmodule