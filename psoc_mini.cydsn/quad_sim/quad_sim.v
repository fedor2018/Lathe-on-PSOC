//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
module quad_sim( 
    input wire clk,
    input wire dir,
    output reg A,
    output reg B
    );
//`#start body` -- edit after this line, do not edit this line
always @(posedge clk)begin
	A<=~A;
end

always @(negedge clk)begin
	B<=(dir)?A:~A;
end
//`#end` -- edit above this line, do not edit this line

//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
endmodule

//[] END OF FILE
