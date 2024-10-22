//Frequency Scaling
//Inputs : clk_50M
//Output : clk_3125KHz


module frequency_scaling (
    input clk_50M,
    output reg clk_3125KHz
);

reg [2:0] counter = 0; // counts 0 to 7
initial begin
    clk_3125KHz = 0;
end

always @ (posedge clk_50M) begin
    if (!counter) clk_3125KHz = ~clk_3125KHz; // toggles clock signal
    counter = counter + 1'b1; // increment counter // after 7 it resets to 0
end

endmodule
