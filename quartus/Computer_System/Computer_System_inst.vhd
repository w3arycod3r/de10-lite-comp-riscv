	component Computer_System is
		port (
			accel_I2C_SDAT             : inout std_logic                     := 'X';             -- I2C_SDAT
			accel_I2C_SCLK             : out   std_logic;                                        -- I2C_SCLK
			accel_G_SENSOR_CS_N        : out   std_logic;                                        -- G_SENSOR_CS_N
			accel_G_SENSOR_INT         : in    std_logic                     := 'X';             -- G_SENSOR_INT
			arduino_gpio_in_port       : in    std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			arduino_gpio_out_port      : out   std_logic_vector(31 downto 0);                    -- out_port
			arduino_reset_n_export     : out   std_logic;                                        -- export
			gpio_a_in_port             : in    std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			gpio_a_out_port            : out   std_logic_vector(31 downto 0);                    -- out_port
			gpio_b_in_port             : in    std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			gpio_b_out_port            : out   std_logic_vector(31 downto 0);                    -- out_port
			hex3_hex0_export           : out   std_logic_vector(31 downto 0);                    -- export
			hex5_hex4_export           : out   std_logic_vector(15 downto 0);                    -- export
			leds_export                : out   std_logic_vector(9 downto 0);                     -- export
			pushbuttons_export         : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
			master_reset_reset         : in    std_logic                     := 'X';             -- reset
			uart0_RXD                  : in    std_logic                     := 'X';             -- RXD
			uart0_TXD                  : out   std_logic;                                        -- TXD
			sdram_ba                   : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_addr                 : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_cas_n                : out   std_logic;                                        -- cas_n
			sdram_cke                  : out   std_logic;                                        -- cke
			sdram_cs_n                 : out   std_logic;                                        -- cs_n
			sdram_dq                   : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_dqm                  : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_ras_n                : out   std_logic;                                        -- ras_n
			sdram_we_n                 : out   std_logic;                                        -- we_n
			sdram_clk_clk              : out   std_logic;                                        -- clk
			slider_switches_export     : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			system_pll_ref_clk_clk     : in    std_logic                     := 'X';             -- clk
			system_pll_ref_reset_reset : in    std_logic                     := 'X';             -- reset
			vexriscv_core0_jtag_tms    : in    std_logic                     := 'X';             -- tms
			vexriscv_core0_jtag_tdi    : in    std_logic                     := 'X';             -- tdi
			vexriscv_core0_jtag_tdo    : out   std_logic;                                        -- tdo
			vexriscv_core0_jtag_tck    : in    std_logic                     := 'X';             -- tck
			vga_CLK                    : out   std_logic;                                        -- CLK
			vga_HS                     : out   std_logic;                                        -- HS
			vga_VS                     : out   std_logic;                                        -- VS
			vga_BLANK                  : out   std_logic;                                        -- BLANK
			vga_SYNC                   : out   std_logic;                                        -- SYNC
			vga_R                      : out   std_logic_vector(3 downto 0);                     -- R
			vga_G                      : out   std_logic_vector(3 downto 0);                     -- G
			vga_B                      : out   std_logic_vector(3 downto 0);                     -- B
			video_pll_ref_clk_clk      : in    std_logic                     := 'X';             -- clk
			video_pll_ref_reset_reset  : in    std_logic                     := 'X';             -- reset
			gpio_a_dir_export          : out   std_logic_vector(31 downto 0);                    -- export
			gpio_b_dir_export          : out   std_logic_vector(31 downto 0);                    -- export
			gpio_af_mux_a_export       : out   std_logic_vector(31 downto 0);                    -- export
			arduino_gpio_dir_export    : out   std_logic_vector(31 downto 0)                     -- export
		);
	end component Computer_System;

	u0 : component Computer_System
		port map (
			accel_I2C_SDAT             => CONNECTED_TO_accel_I2C_SDAT,             --                accel.I2C_SDAT
			accel_I2C_SCLK             => CONNECTED_TO_accel_I2C_SCLK,             --                     .I2C_SCLK
			accel_G_SENSOR_CS_N        => CONNECTED_TO_accel_G_SENSOR_CS_N,        --                     .G_SENSOR_CS_N
			accel_G_SENSOR_INT         => CONNECTED_TO_accel_G_SENSOR_INT,         --                     .G_SENSOR_INT
			arduino_gpio_in_port       => CONNECTED_TO_arduino_gpio_in_port,       --         arduino_gpio.in_port
			arduino_gpio_out_port      => CONNECTED_TO_arduino_gpio_out_port,      --                     .out_port
			arduino_reset_n_export     => CONNECTED_TO_arduino_reset_n_export,     --      arduino_reset_n.export
			gpio_a_in_port             => CONNECTED_TO_gpio_a_in_port,             --               gpio_a.in_port
			gpio_a_out_port            => CONNECTED_TO_gpio_a_out_port,            --                     .out_port
			gpio_b_in_port             => CONNECTED_TO_gpio_b_in_port,             --               gpio_b.in_port
			gpio_b_out_port            => CONNECTED_TO_gpio_b_out_port,            --                     .out_port
			hex3_hex0_export           => CONNECTED_TO_hex3_hex0_export,           --            hex3_hex0.export
			hex5_hex4_export           => CONNECTED_TO_hex5_hex4_export,           --            hex5_hex4.export
			leds_export                => CONNECTED_TO_leds_export,                --                 leds.export
			pushbuttons_export         => CONNECTED_TO_pushbuttons_export,         --          pushbuttons.export
			master_reset_reset         => CONNECTED_TO_master_reset_reset,         --         master_reset.reset
			uart0_RXD                  => CONNECTED_TO_uart0_RXD,                  --                uart0.RXD
			uart0_TXD                  => CONNECTED_TO_uart0_TXD,                  --                     .TXD
			sdram_ba                   => CONNECTED_TO_sdram_ba,                   --                sdram.ba
			sdram_addr                 => CONNECTED_TO_sdram_addr,                 --                     .addr
			sdram_cas_n                => CONNECTED_TO_sdram_cas_n,                --                     .cas_n
			sdram_cke                  => CONNECTED_TO_sdram_cke,                  --                     .cke
			sdram_cs_n                 => CONNECTED_TO_sdram_cs_n,                 --                     .cs_n
			sdram_dq                   => CONNECTED_TO_sdram_dq,                   --                     .dq
			sdram_dqm                  => CONNECTED_TO_sdram_dqm,                  --                     .dqm
			sdram_ras_n                => CONNECTED_TO_sdram_ras_n,                --                     .ras_n
			sdram_we_n                 => CONNECTED_TO_sdram_we_n,                 --                     .we_n
			sdram_clk_clk              => CONNECTED_TO_sdram_clk_clk,              --            sdram_clk.clk
			slider_switches_export     => CONNECTED_TO_slider_switches_export,     --      slider_switches.export
			system_pll_ref_clk_clk     => CONNECTED_TO_system_pll_ref_clk_clk,     --   system_pll_ref_clk.clk
			system_pll_ref_reset_reset => CONNECTED_TO_system_pll_ref_reset_reset, -- system_pll_ref_reset.reset
			vexriscv_core0_jtag_tms    => CONNECTED_TO_vexriscv_core0_jtag_tms,    --  vexriscv_core0_jtag.tms
			vexriscv_core0_jtag_tdi    => CONNECTED_TO_vexriscv_core0_jtag_tdi,    --                     .tdi
			vexriscv_core0_jtag_tdo    => CONNECTED_TO_vexriscv_core0_jtag_tdo,    --                     .tdo
			vexriscv_core0_jtag_tck    => CONNECTED_TO_vexriscv_core0_jtag_tck,    --                     .tck
			vga_CLK                    => CONNECTED_TO_vga_CLK,                    --                  vga.CLK
			vga_HS                     => CONNECTED_TO_vga_HS,                     --                     .HS
			vga_VS                     => CONNECTED_TO_vga_VS,                     --                     .VS
			vga_BLANK                  => CONNECTED_TO_vga_BLANK,                  --                     .BLANK
			vga_SYNC                   => CONNECTED_TO_vga_SYNC,                   --                     .SYNC
			vga_R                      => CONNECTED_TO_vga_R,                      --                     .R
			vga_G                      => CONNECTED_TO_vga_G,                      --                     .G
			vga_B                      => CONNECTED_TO_vga_B,                      --                     .B
			video_pll_ref_clk_clk      => CONNECTED_TO_video_pll_ref_clk_clk,      --    video_pll_ref_clk.clk
			video_pll_ref_reset_reset  => CONNECTED_TO_video_pll_ref_reset_reset,  --  video_pll_ref_reset.reset
			gpio_a_dir_export          => CONNECTED_TO_gpio_a_dir_export,          --           gpio_a_dir.export
			gpio_b_dir_export          => CONNECTED_TO_gpio_b_dir_export,          --           gpio_b_dir.export
			gpio_af_mux_a_export       => CONNECTED_TO_gpio_af_mux_a_export,       --        gpio_af_mux_a.export
			arduino_gpio_dir_export    => CONNECTED_TO_arduino_gpio_dir_export     --     arduino_gpio_dir.export
		);

