
module load_extend (
    input [31:0] y,
    input [ 2:0] sel,
    output reg [31:0] data
);

always @(*) begin
    case (sel)
    3'b000: data = {{24{y[7]}}, y[7:0]};//lb
    3'b001: data = {{16{y[15]}}, y[15:0]};//lh
    3'b010: data = y;//lw
    3'b011: data = {24'b0, y[7:0]};//lbu
    3'b100: data = {16'b0, y[15:0]};//lhu
    default: data = y;
    endcase
end

endmodule
