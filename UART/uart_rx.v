/*
Module UART Receiver

Input:  clk_50M - 50 MHz clock
        rx      - UART Receiver

Output: rx_msg      - read incoming message
        rx_complete - message received flag
*/

// module declaration
module uart_rx (
  input clk_50M, rx,
  output reg [7:0] rx_msg,
  output reg rx_complete
);

initial begin

rx_msg = 0; rx_complete = 0;

end
reg [1:0] state=0;
reg [10:0] counter = 0;
reg [7:0] data_reg = 0;
reg [3:0] i=0;
parameter state_start = 2'b00 , state_data=2'b01, state_stop=2'b10, state_end = 2'b11;

always @(posedge clk_50M) begin
case (state)
 state_start : begin 
  if(counter<434) begin
   data_reg=0;
	//i=0;
	rx_complete = 0;
   counter = counter +1;
  state = state_start;
  end
  else begin 
   state = state_data;
	//data_reg[7-i] = rx;	
	counter = 0;
	//i=i+1;
   //state = state_data;
  end
 end
 
 state_data: begin
 	//i=i+1;
  if(i < 8 ) begin 
    if(counter< 433) begin
	  if(counter == 200) begin
	   data_reg[7-i] = rx;	
	  end	
	 // i=i+1;
		//data_reg[i] = 1;
	   counter=counter+1;
	 state = state_data;
    end
	 else begin
	   i = i+1;
	  // data_reg[7-i] = rx;
	   //i = i+1;
		counter = 0;
		state = state_data;
		//counter = 0;
	 end
	 end 
  else begin
    /*if(counter == 435 && i < 8) begin
	  counter = 0;
	  //rx_msg[i] = data_reg[i];
	  i=i+1;
	  state=state_data;
	 end*/
	 //else begin
	 counter = 1;
	 i=0;
	 //rx_msg= data_msg
	 //rx_msg=0;
	 state = state_stop;
	 //end
  end
 end
 
 state_stop: begin 
  if(counter<433) begin
    //rx_msg = data_reg;
   counter=counter+1;
	state=state_stop;
 end 
  else begin   
    rx_msg = data_reg;	 
	 counter=1;
	  if (rx_msg!=0)begin
	 rx_complete=1;
	 state = state_start;
	  end
	  else begin
	  rx_complete=0;
	 end 
	 
  end
 end
endcase
end

endmodule