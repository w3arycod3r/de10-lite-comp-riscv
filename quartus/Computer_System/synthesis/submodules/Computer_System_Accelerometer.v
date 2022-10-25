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
 * This module sends and receives data to/from the DE0-Nano's accelerometer.  *
 *                                                                            *
 ******************************************************************************/

module Computer_System_Accelerometer (
	// Inputs
	clk,
	reset,

	address,
	byteenable,
	read,
	write,
	writedata,

	G_SENSOR_INT,
	
	// Bidirectionals
	I2C_SDAT,

	// Outputs
	readdata,
	waitrequest,
	irq,

	G_SENSOR_CS_N,
	I2C_SCLK
);

/*****************************************************************************
 *                           Parameter Declarations                          *
 *****************************************************************************/

parameter DW					= 15;	// Serial protocol's datawidth

parameter CFG_TYPE			= 8'h00;

parameter READ_MASK			= {1'b0, 1'b0, 6'h00, 8'hFF};
parameter WRITE_MASK			= {1'b0, 1'b0, 6'h00, 8'h00};

parameter RESTART_COUNTER	= 'h9;

// Auto init parameters
parameter AIRS					= 11;
parameter AIAW					= 3;

// Serial Bus Controller parameters
//parameter SBDW					= 26;	// Serial bus's datawidth
parameter SBCW					= 4;	// Serial bus counter's width
parameter SCCW					= 4;	// Slow clock's counter's width

/*****************************************************************************
 *                             Port Declarations                             *
 *****************************************************************************/
// Inputs
input						clk;
input						reset;

input						address;
input						byteenable;
input						read;
input						write;
input			[ 7: 0]	writedata;

input						G_SENSOR_INT;

// Bidirectionals
inout						I2C_SDAT;

// Outputs
output reg	[ 7: 0]	readdata;
output					waitrequest;
output					irq;

output					G_SENSOR_CS_N;
output					I2C_SCLK;

/*****************************************************************************
 *                           Constant Declarations                           *
 *****************************************************************************/

// States for finite state machine
localparam	STATE_0_IDLE				= 3'h0,
				STATE_1_PRE_WRITE			= 3'h1,
				STATE_2_WRITE_TRANSFER	= 3'h2,
				STATE_3_POST_WRITE		= 3'h3,
				STATE_4_PRE_READ			= 3'h4,
				STATE_5_READ_TRANSFER	= 3'h5,
				STATE_6_POST_READ			= 3'h6;

/*****************************************************************************
 *                 Internal wires and registers Declarations                 *
 *****************************************************************************/
// Internal Wires
//wire						internal_reset;

//  Auto init signals
wire			[AIAW:0]	rom_address;
wire			[DW: 0]	rom_data;
wire						ack;

wire			[DW: 0]	auto_init_data;
wire						auto_init_transfer_en;
wire						auto_init_complete;
wire						auto_init_error;

//  Serial controller signals
wire			[DW: 0]	transfer_mask;
wire			[DW: 0]	data_to_controller;
wire			[DW: 0]	data_from_controller;

wire						start_transfer;

wire						transfer_complete;

// Internal Registers
//reg			[31: 0]	control_reg;
reg			[ 5: 0]	address_reg;
reg			[ 7: 0]	data_reg;

reg						start_external_transfer;
reg						external_read_transfer;
reg			[ 7: 0]	address_for_transfer;

// State Machine Registers
reg			[ 2: 0]	ns_serial_transfer;
reg			[ 2: 0]	s_serial_transfer;

/*****************************************************************************
 *                         Finite State Machine(s)                           *
 *****************************************************************************/

always @(posedge clk)
begin
	if (reset)
		s_serial_transfer <= STATE_0_IDLE;
	else
		s_serial_transfer <= ns_serial_transfer;
end

always @(*)
begin
	// Defaults
	ns_serial_transfer = STATE_0_IDLE;

    case (s_serial_transfer)
	STATE_0_IDLE:
		begin
			if (transfer_complete | ~auto_init_complete)
				ns_serial_transfer = STATE_0_IDLE;
			else if (write & (address == 1'b1))
				ns_serial_transfer = STATE_1_PRE_WRITE;
			else if (read & (address == 1'b1))
				ns_serial_transfer = STATE_4_PRE_READ;
			else
				ns_serial_transfer = STATE_0_IDLE;
		end
	STATE_1_PRE_WRITE:
		begin
			ns_serial_transfer = STATE_2_WRITE_TRANSFER;
		end
	STATE_2_WRITE_TRANSFER:
		begin
			if (transfer_complete)
				ns_serial_transfer = STATE_3_POST_WRITE;
			else
				ns_serial_transfer = STATE_2_WRITE_TRANSFER;
		end
	STATE_3_POST_WRITE:
		begin
			ns_serial_transfer = STATE_0_IDLE;
		end
	STATE_4_PRE_READ:
		begin
			ns_serial_transfer = STATE_5_READ_TRANSFER;
		end
	STATE_5_READ_TRANSFER:
		begin
			if (transfer_complete)
				ns_serial_transfer = STATE_6_POST_READ;
			else
				ns_serial_transfer = STATE_5_READ_TRANSFER;
		end
	STATE_6_POST_READ:
		begin
			ns_serial_transfer = STATE_0_IDLE;
		end
	default:
		begin
			ns_serial_transfer = STATE_0_IDLE;
		end
	endcase
end

/*****************************************************************************
 *                             Sequential logic                              *
 *****************************************************************************/

// Output regsiters
always @(posedge clk)
begin
	if (reset)
		readdata		<= 32'h00000000;
	else if (read)
	begin
		if (address == 1'b0)
			readdata	<= {2'h0, address_reg};
		else
			readdata	<= data_from_controller[ 7: 0];
	end
end

// Internal regsiters
always @(posedge clk)
begin
	if (reset)
	begin
		address_reg			<= 8'h00;
		data_reg				<= 8'h00;
	end
	
	else if (write & ~waitrequest)
	begin
		// Write to address register
		if ((address == 1'b0) & byteenable)
			address_reg		<= writedata[5:0];

		// Write to data register
		if ((address == 1'b1) & byteenable)
			data_reg			<= writedata;
	end
end

always @(posedge clk)
begin
	if (reset)
	begin
		start_external_transfer <= 1'b0;
		external_read_transfer	<= 1'b0;
		address_for_transfer	<= 8'h00;
	end
	else if (transfer_complete)
	begin
		start_external_transfer <= 1'b0;
		external_read_transfer	<= 1'b0;
		address_for_transfer	<= 8'h00;
	end
	else if (s_serial_transfer == STATE_1_PRE_WRITE)
	begin
		start_external_transfer <= 1'b1;
		external_read_transfer	<= 1'b0;
		address_for_transfer	<= address_reg;
	end
	else if (s_serial_transfer == STATE_4_PRE_READ)
	begin
		start_external_transfer <= 1'b1;
		external_read_transfer	<= 1'b1;
		address_for_transfer	<= address_reg;
	end
end

/*****************************************************************************
 *                            Combinational logic                            *
 *****************************************************************************/

// Output Assignments
assign waitrequest	=
	((address == 1'b1) & write & (s_serial_transfer != STATE_1_PRE_WRITE)) |
	((address == 1'b1) & read  & (s_serial_transfer != STATE_6_POST_READ));
assign irq			= G_SENSOR_INT;

// Internal Assignments
//  Signals to the serial controller
assign transfer_mask = (~auto_init_complete | ~external_read_transfer) ?
						WRITE_MASK : READ_MASK;

assign data_to_controller = 
		(~auto_init_complete) ?
			auto_init_data :
			{external_read_transfer, 1'b0,
			 address_for_transfer[5:0],
			 data_reg};

assign start_transfer = (auto_init_complete) ? 
							start_external_transfer : 
							auto_init_transfer_en;

//  Signals from the serial controller
assign ack = 1'b0;

/*****************************************************************************
 *                              Internal Modules                             *
 *****************************************************************************/

altera_up_accelerometer_spi_auto_init_ctrl Auto_Init_Controller (
	// Inputs
	.clk						(clk),
	.reset					(reset),

	.clear_error			(1'b0),

	.ack						(ack),
	.transfer_complete	(transfer_complete),

	.rom_data				(rom_data),

	// Bidirectionals

	// Outputs
	.data_out				(auto_init_data),
	.transfer_data			(auto_init_transfer_en),

	.rom_address			(rom_address),
	
	.auto_init_complete	(auto_init_complete),
	.auto_init_error		(auto_init_error)
);
defparam	
	Auto_Init_Controller.ROM_SIZE	= AIRS,
	Auto_Init_Controller.AW			= AIAW,
	Auto_Init_Controller.DW			= DW;

altera_up_accelerometer_spi_auto_init Auto_Init_Accelerometer (
	// Inputs
	.rom_address			(rom_address),

	// Bidirectionals

	// Outputs
	.rom_data				(rom_data)
);

altera_up_accelerometer_spi_serial_bus_controller Serial_Bus_Controller (
	// Inputs
	.clk						(clk),
	.reset					(reset),

	.start_transfer		(start_transfer),

	.data_in					(data_to_controller),
	.transfer_mask			(transfer_mask),

	// Bidirectionals
	.serial_data			(I2C_SDAT),

	// Outputs
	.serial_clk				(I2C_SCLK),
	.serial_en				(G_SENSOR_CS_N),

	.data_out				(data_from_controller),
	.transfer_complete	(transfer_complete)
);
defparam
	Serial_Bus_Controller.DW	= DW,
	Serial_Bus_Controller.CW	= SBCW,
	Serial_Bus_Controller.SCCW	= SCCW;

endmodule

