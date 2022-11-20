
module Computer_System (
	accel_I2C_SDAT,
	accel_I2C_SCLK,
	accel_G_SENSOR_CS_N,
	accel_G_SENSOR_INT,
	arduino_gpio_in_port,
	arduino_gpio_out_port,
	arduino_reset_n_export,
	gpio_a_in_port,
	gpio_a_out_port,
	gpio_b_in_port,
	gpio_b_out_port,
	hex3_hex0_export,
	hex5_hex4_export,
	leds_export,
	pushbuttons_export,
	master_reset_reset,
	uart0_RXD,
	uart0_TXD,
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
	vexriscv_core0_jtag_tms,
	vexriscv_core0_jtag_tdi,
	vexriscv_core0_jtag_tdo,
	vexriscv_core0_jtag_tck,
	vga_CLK,
	vga_HS,
	vga_VS,
	vga_BLANK,
	vga_SYNC,
	vga_R,
	vga_G,
	vga_B,
	video_pll_ref_clk_clk,
	video_pll_ref_reset_reset,
	gpio_a_dir_export,
	gpio_b_dir_export,
	gpio_af_mux_a_export,
	arduino_gpio_dir_export);	

	inout		accel_I2C_SDAT;
	output		accel_I2C_SCLK;
	output		accel_G_SENSOR_CS_N;
	input		accel_G_SENSOR_INT;
	input	[31:0]	arduino_gpio_in_port;
	output	[31:0]	arduino_gpio_out_port;
	output		arduino_reset_n_export;
	input	[31:0]	gpio_a_in_port;
	output	[31:0]	gpio_a_out_port;
	input	[31:0]	gpio_b_in_port;
	output	[31:0]	gpio_b_out_port;
	output	[31:0]	hex3_hex0_export;
	output	[15:0]	hex5_hex4_export;
	output	[9:0]	leds_export;
	input	[1:0]	pushbuttons_export;
	input		master_reset_reset;
	input		uart0_RXD;
	output		uart0_TXD;
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
	input		vexriscv_core0_jtag_tms;
	input		vexriscv_core0_jtag_tdi;
	output		vexriscv_core0_jtag_tdo;
	input		vexriscv_core0_jtag_tck;
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
	output	[31:0]	gpio_a_dir_export;
	output	[31:0]	gpio_b_dir_export;
	output	[31:0]	gpio_af_mux_a_export;
	output	[31:0]	arduino_gpio_dir_export;
endmodule
