// (C) 2001-2022 Intel Corporation. All rights reserved.
// Your use of Intel Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files from any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Intel Program License Subscription 
// Agreement, Intel FPGA IP License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Intel and sold by 
// Intel or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// THIS FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THIS FILE OR THE USE OR OTHER DEALINGS
// IN THIS FILE.

/******************************************************************************
 *                                                                            *
 * This module is a rom for auto initializing the accelerometer on the        *
 *   DE-Nano.                                                                 *
 *                                                                            *
 ******************************************************************************/

module altera_up_accelerometer_spi_auto_init (
	// Inputs
	rom_address,

	// Bidirectionals

	// Outputs
	rom_data
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/


/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/
// Inputs
input		[ 4: 0]	rom_address;

// Bidirectionals

// Outputs
output		[15: 0]	rom_data;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/

// States

/*****************************************************************************
 *                 Internal wires and registers Declarations                 *
 *****************************************************************************/
// Internal Wires
reg			[13: 0]	data;

// Internal Registers

// State Machine Registers

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/


/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/

// Output Registers

// Internal Registers

/*****************************************************************************
 *                            Combinational logic                            *
 *****************************************************************************/

// Output Assignments
assign rom_data = {2'h0, data[13: 0]};

// Internal Assignments
always @(*)
begin
	case (rom_address)
	0		:	data	<=	{6'h24, 8'h04};		// Activity value threshold.
	1		:	data	<=	{6'h25, 8'h02};		// Inactivity value threshold.
	2		:	data	<=	{6'h26, 8'h02};		// Activity time threshold.
	3		:	data	<=	{6'h27, 8'hFF};		// AC coupling for thresholds.
	4		:	data	<=	{6'h28, 8'h09};		// Free fall value threshold.
	5		:	data	<=	{6'h29, 8'h46};		// Free fall time threshold.
	6		:	data	<=	{6'h2C, 8'h0A};		// Output data rate: 100 Hz.
	7		:	data	<=	{6'h2E, 8'h18};		// Enabling ACTIVITY and INACTIVITY interrupts.
	8		:	data	<=	{6'h2F, 8'h10};		// Sends the ACTIVITY interrupt to the INT2 pin.
	9		:	data	<=	{6'h31, 8'h4B};		// 3-Wire SPI mode, 16G, full resolution.
	10		:	data	<=	{6'h2D, 8'h08};		// Start measuring.
	default	:	data	<=	14'h0000;
	endcase
end

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/


endmodule

