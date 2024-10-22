/*
Module UART Transmitter

Input:  clk_50M - 50 MHz clock
        data    - 8-bit data line to transmit
Output: tx      - UART Transmission Line
*/

// module declaration
module uart_tx(
    input  clk_50M,
    input  [7:0] data,
    output reg tx
	 
);

reg [1:0] state;
initial begin
	 tx = 0;state =2'b11 ;
end
reg [10:0] count = 0;
reg [3:0] i = 0;
parameter state_start = 2'b00 , state_data=2'b01, state_stop=2'b10, state_end = 2'b11;

always @(posedge clk_50M) begin
	  case(state)
	  
	  state_end: begin
	  if(data!=0)begin
		  tx=0;
		  //count=1;
		  state=state_start;
	  end
	  else begin
	     tx=1;
	  end
	  end
	   state_start:begin
		 if(count < 433) begin
		   //tx=0;
			count=count+1;
			state=state_start;
		 end
		 
		 else begin
		   count= 1;
			state=state_data;
			//tx=data[i];
		 end
		 end
		 
		state_data: begin
		  if(i<8) begin
		    if(count<433)begin
			   count=count+1;
				tx=data[i];
				state=state_data;
			 end
			 
			 else begin
			   i=i+1;
				//tx = data[i];
				count=0;
				state=state_data;
			 end
		  end
		  
		 else begin
		   i=0;
			count=0;
			//tx=1;
			state=state_stop;
		 end
		end
	
	state_stop: begin
	  if(count<433) begin
	    //tx=1;
		 count=count+1;
		 tx=1;
	  end
	  
	  else begin
	    //tx=1 ;
		 count=1;
		 tx=0;
		 state=state_end;
		 //tx=0;
	  end
	end
	  endcase

end

endmodule