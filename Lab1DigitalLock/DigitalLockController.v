/*
 Author: Abiria Placide
 X = Input binary code
 Enter = read current input value
 Reset/Lock = restart or lock after its opened
 Open = 1 if X = 1-1-1-0-1-1-1
 
 key assignments
 X = SW0              ( PIN_AB12 )
 Enter = KEY1         ( PIN_AA15 )
 RESET/LOCK = KEY0 ( PIN_AA14 )
 OPEN = LEDR0          ( PIN_V16  )
 
 *remember to handle edge cases for reset on each state
*/
module DigitalLockController(X, Enter,Reset_Lock, Open, State, HEXout);

output [0:6] HEXout;

//------------------
// Constants
//------------------

parameter WIDTH = 4;

//------------------
// INPUTS
//------------------

input X, Enter, Reset_Lock;

//------------------
// OUTPUTS
//------------------

output reg Open = 0;

//------------------

//------------------
//  STATE ENCODING
//------------------


output reg [WIDTH-1:0] State; //keeps track of current state
reg        [WIDTH-1:0] nextState;

localparam 	  STATE0 = 4'd0,
              STATE1 = 4'd1,
              STATE2 = 4'd2,
              STATE3 = 4'd3,
              STATE4 = 4'd4,
              STATE5 = 4'd5,
              STATE6 = 4'd6,
              STATE7 = 4'd7,
              STATE8 = 4'd8;


always@(negedge Enter) //neg edge since the buttons are active low
begin
    if(Reset_Lock == 0) State <= STATE0; //on reset, change state to reset state0. zero because  active low
    else State <= nextState;
end //end of always block

always@( * )
begin
    nextState = State; //make next state to current state if it has not been assigned
    
    case(State)
        STATE0: begin 
						if(X) nextState = STATE1;
						else nextState = STATE8; //reapeat for every state
					 end //1
		  
        STATE1: begin 
						if(X) nextState = STATE2;
				   	else nextState = STATE8; //reapeat for every state
					 end //1
						
        STATE2: begin 
						if(X)  begin nextState = STATE3; end
						else nextState = STATE8; //reapeat for every state
					 end //1
        STATE3: begin 
					   if(~X) nextState = STATE4; 
				      else nextState = STATE8; //reapeat for every state
					 end //0
        STATE4: begin 
						if(X)  nextState = STATE5; 
						else nextState = STATE8; //reapeat for every state
					 end //1
        STATE5: begin 
						if(X)  nextState = STATE6; 
						else nextState = STATE8; //reapeat for every state
					 end //1
        STATE6: begin 
						 if(X)  nextState = STATE7; 
						 else nextState = STATE8; //reapeat for every state
					 end //1
        STATE7: begin
                    nextState = STATE7;
                  end  
		  STATE8: begin 
					   nextState = STATE8;
					 end
    endcase
end //end of always block

always @( * )
begin
	case (State)
		STATE0, STATE1, STATE2, STATE3, STATE4, STATE5, STATE6 : Open = 0;
		STATE7: Open = 1;
	endcase
end

//hex0 display instantiation
binary2seven(.BIN(State), .SEV(HEXout));

endmodule
