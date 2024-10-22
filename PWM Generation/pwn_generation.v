//PWM Generator
//Inputs : clk_3125KHz, duty_cycle
//Output : clk_195KHz, pwm_signal

module pwm_generator(
    input clk_3125KHz,
    input [3:0] duty_cycle,
    output reg clk_195KHz, pwm_signal
);

initial begin
    clk_195KHz = 0; pwm_signal = 1; 
end

//////////////////DO NOT MAKE ANY CHANGES ABOVE THIS LINE//////////////////

reg [2:0] counter = 0; // counts 0 to 7
reg [3:0] counter_duty = 0 ; // count 0 to 15


always @ (posedge clk_3125KHz) begin
    if (!counter) clk_195KHz = ~clk_195KHz; // toggles clock signal
	 
	 if(counter_duty<duty_cycle) pwm_signal = 1;
	 else pwm_signal = 0;
	 counter = counter + 1'b1;
	 counter_duty = counter_duty +1'b1;
end

//////////////////DO NOT MAKE ANY CHANGES BELOW THIS LINE//////////////////

endmodule