
module store_extend (
    input   [31:0] y,
    input   [1:0] sel,
    output reg [31:0] data
);

always @(*) begin
    case(sel)
        2'b00: data = y;//sw
        2'b01: data = {24'b0, y[7:0]};//sh
        2'b10: data = {16'b0, y[15:0]};//sb
        default: data = y;
    endcase
end

endmodule