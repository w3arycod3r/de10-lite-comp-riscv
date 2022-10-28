
module Computer_System (
	accelerometer_I2C_SDAT,
	accelerometer_I2C_SCLK,
	accelerometer_G_SENSOR_CS_N,
	accelerometer_G_SENSOR_INT,
	arduino_gpio_export,
	arduino_reset_n_export,
	expansion_jp1_0_export,
	expansion_jp1_1_export,
	hex3_hex0_export,
	hex5_hex4_export,
	leds_export,
	pushbuttons_export,
	reset_controller_0_reset_in0_reset,
	sdram_ba,
	sdram_addr,
	sdram_cas_n,
	sdram_cke,
	sdram_cs_n,
	sdram_dq,
	sdram_dqm,
	sdram_ras_n,
	sdram_we_n,
	sdram_clk_clk,
	slider_switches_export,
	system_pll_ref_clk_clk,
	system_pll_ref_reset_reset,
	vexriscvavalon_0_jtag_tms,
	vexriscvavalon_0_jtag_tdi,
	vexriscvavalon_0_jtag_tdo,
	vexriscvavalon_0_jtag_tck,
	vga_CLK,
	vga_HS,
	vga_VS,
	vga_BLANK,
	vga_SYNC,
	vga_R,
	vga_G,
	vga_B,
	video_pll_ref_clk_clk,
	video_pll_ref_reset_reset);	

	inout		accelerometer_I2C_SDAT;
	output		accelerometer_I2C_SCLK;
	output		accelerometer_G_SENSOR_CS_N;
	input		accelerometer_G_SENSOR_INT;
	inout	[15:0]	arduino_gpio_export;
	output		arduino_reset_n_export;
	inout	[17:0]	expansion_jp1_0_export;
	inout	[17:0]	expansion_jp1_1_export;
	output	[31:0]	hex3_hex0_export;
	output	[15:0]	hex5_hex4_export;
	output	[9:0]	leds_export;
	input	[1:0]	pushbuttons_export;
	input		reset_controller_0_reset_in0_reset;
	output	[1:0]	sdram_ba;
	output	[12:0]	sdram_addr;
	output		sdram_cas_n;
	output		sdram_cke;
	output		sdram_cs_n;
	inout	[15:0]	sdram_dq;
	output	[1:0]	sdram_dqm;
	output		sdram_ras_n;
	output		sdram_we_n;
	output		sdram_clk_clk;
	input	[9:0]	slider_switches_export;
	input		system_pll_ref_clk_clk;
	input		system_pll_ref_reset_reset;
	input		vexriscvavalon_0_jtag_tms;
	input		vexriscvavalon_0_jtag_tdi;
	output		vexriscvavalon_0_jtag_tdo;
	input		vexriscvavalon_0_jtag_tck;
	output		vga_CLK;
	output		vga_HS;
	output		vga_VS;
	output		vga_BLANK;
	output		vga_SYNC;
	output	[3:0]	vga_R;
	output	[3:0]	vga_G;
	output	[3:0]	vga_B;
	input		video_pll_ref_clk_clk;
	input		video_pll_ref_reset_reset;
endmodule
