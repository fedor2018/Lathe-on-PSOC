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
module q4(
    input wire clk,
    input wire A,
    input wire B,
    output reg count,
    output reg dir,
    output reg cw
    );
//`#start body` -- edit after this line, do not edit this line

reg [2:0] quadA_delayed, quadB_delayed;

always @(posedge clk) quadA_delayed <= {quadA_delayed[1:0], A};
always @(posedge clk) quadB_delayed <= {quadB_delayed[1:0], B};

wire c = quadA_delayed[1] ^ quadA_delayed[2] ^ quadB_delayed[1] ^ quadB_delayed[2];
wire d = quadA_delayed[1] ^ quadB_delayed[2];

always @(posedge clk) begin
    count <= c;
    dir <= d;
    if(count) cw <= dir;
end
//`#end` -- edit above this line, do not edit this line

//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
endmodule

//[] END OF FILE
