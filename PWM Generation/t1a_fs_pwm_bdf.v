module t1a_fs_pwm_bdf(
	clk_50M,
	duty_cycle,
	pwm_signal,
	clk_195KHz,
	clk_3125KHz
);


input wire	clk_50M;
input wire	[3:0] duty_cycle;
output wire	pwm_signal;
output wire	clk_195KHz;
output wire	clk_3125KHz;

wire	SYNTHESIZED_WIRE_0;

assign	clk_3125KHz = SYNTHESIZED_WIRE_0;




frequency_scaling	b2v_inst(
	.clk_50M(clk_50M),
	.clk_3125KHz(SYNTHESIZED_WIRE_0));


pwm_generator	b2v_inst1(
	.clk_3125KHz(SYNTHESIZED_WIRE_0),
	.duty_cycle(duty_cycle),
	.clk_195KHz(clk_195KHz),
	.pwm_signal(pwm_signal));


endmodule