// t1c_pulse_gen_detect
//Inputs : clk_50M, reset, echo_rx
//Output : trigger, distance, pulses, state

// module declaration
module t1c_pulse_gen_detect (
    input clk_50M, reset, echo_rx,
    output reg trigger, out,
    output reg [21:0] pulses,
    output reg [1:0] state
);

initial begin
    trigger = 0; out = 0; pulses = 0; state = 0;
end

parameter state_warmup=2'b00, state_trigger=2'b01, state_read=2'b10, state_output=2'b11;
reg [21:0] count_1ms = 1;
reg [21:0] count_10us = 0;
reg [21:0] count_1us = 1;
//state = 2'b00;
reg [1:0] flag = 0;
	always @(posedge clk_50M) begin

		if(reset == 1)begin
			if(flag == 0)begin
				count_10us = 1;
				flag = 1;
			end
			out = 0;
			pulses = 0;
			trigger = 0;  count_1us = 1 ; count_1ms = 1;
			state = state_warmup;
		end
		
		else begin
		case (state)
			state_warmup: begin
				if(count_1us<50) begin
				state = state_warmup;
				count_1us = count_1us +1'b1;
				end
				else begin
				state = state_trigger;
				count_1us = 1;
				end
			end
			state_trigger: begin
				
				if(count_10us < 501) begin
				trigger = 1 ; 
				state = state_trigger;
				count_10us = count_10us + 1'b1;	
				end
				else begin
				trigger = 0;
				state = state_read ;
				count_10us = 1;
				end
			end
			state_read: begin
				if(count_1ms< 50000-1)begin
					if(echo_rx == 1)begin
						count_1ms = count_1ms+1'b1;
						pulses = pulses +1;
					end
				
					else
					count_1ms= count_1ms + 1'b1;
				end		
		      else begin
					
				  state = state_output;
				  count_1ms = 1;
				  end
		   end	
			state_output: begin
				if(pulses == 29409)begin
					out = 1;
					state = state_warmup; trigger = 0 ; pulses = 0 ; count_1us=1;
					end
				else begin
					state = state_warmup; trigger = 0 ; pulses = 0 ; count_1us = 1;
					end
			end
		endcase
		end
    end

endmodule