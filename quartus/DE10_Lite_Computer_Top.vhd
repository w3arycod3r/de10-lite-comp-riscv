-- Porting de10-lite nios ii computer to riscv core
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library work;


entity DE10_Lite_Computer_Top is
port (
    --///////// CLOCKS /////////
    MAX10_CLK1_50  : in std_logic;
    MAX10_CLK2_50  : in std_logic;
    ADC_CLK_10     : in std_logic;

    --///////// DRAM /////////
    DRAM_ADDR  : out std_logic_vector(12 downto 0);
    DRAM_BA    : out std_logic_vector(1 downto 0);
    DRAM_CAS_N : out std_logic;
    DRAM_CKE   : out std_logic;
    DRAM_CLK   : out std_logic;
    DRAM_CS_N  : out std_logic;
    DRAM_DQ    : inout std_logic_vector(15 downto 0);
    DRAM_LDQM  : out std_logic;
    DRAM_RAS_N : out std_logic;
    DRAM_UDQM  : out std_logic;
    DRAM_WE_N  : out std_logic;


    --///////// GPIO /////////
    GPIO        : inout std_logic_vector(35 downto 0);


    --///////// HEX /////////
    HEX0          : out std_logic_vector(7 downto 0);
    HEX1          : out std_logic_vector(7 downto 0);
    HEX2          : out std_logic_vector(7 downto 0);
    HEX3          : out std_logic_vector(7 downto 0);
    HEX4          : out std_logic_vector(7 downto 0);
    HEX5          : out std_logic_vector(7 downto 0);

    --///////// KEY /////////
    KEY          : in std_logic_vector(1 downto 0);

    --///////// LEDR /////////
    LEDR         : out std_logic_vector(9 downto 0);

    --///////// SW /////////
    SW           : in std_logic_vector(9 downto 0);

    --///////// VGA /////////
    VGA_R        : out std_logic_vector(3 downto 0);
    VGA_G        : out std_logic_vector(3 downto 0);
    VGA_B        : out std_logic_vector(3 downto 0); 
    VGA_HS       : out std_logic;             
    VGA_VS       : out std_logic;

    --///////// GSENSOR /////////
    GSENSOR_CS_N    : out std_logic;
    GSENSOR_INT     : in std_logic_vector(2 downto 1);
    GSENSOR_SCLK    : out std_logic;
    GSENSOR_SDI     : inout std_logic;
    GSENSOR_SDO     : inout std_logic;
    
    --///////// Arduino IO /////////
    ARDUINO_IO      : inout std_logic_vector(15 downto 0);
    ARDUINO_RESET_N : inout std_logic

);
end DE10_Lite_Computer_Top;

architecture arch of DE10_Lite_Computer_Top is

    -- Components
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

    -- Signals
    signal NOT_KEY : std_logic_vector(1 downto 0);
    signal asyncReset : std_logic;

    signal hex3_hex0 : std_logic_vector(31 downto 0);
    signal hex5_hex4 : std_logic_vector(15 downto 0);

    signal sdram_dqm : std_logic_vector(1 downto 0);

    signal gpio_a_in_reg : std_logic_vector(31 downto 0);   -- To input register in PIO periph
    signal gpio_a_out_reg : std_logic_vector(31 downto 0);  -- From output register in PIO periph
    signal gpio_a_dir_reg : std_logic_vector(31 downto 0);  -- "Direction" register from PIO periph: '0' for input, '1' for output
    signal gpio_a_out_af : std_logic_vector(31 downto 0);   -- Output from alt func muxes
    signal gpio_a_out : std_logic_vector(31 downto 0);      -- To actual GPIO output pins, output from tri-state buffers, drive "inout" ports
    
    signal gpio_b_in_reg : std_logic_vector(31 downto 0);
    signal gpio_b_out_reg : std_logic_vector(31 downto 0);
    signal gpio_b_dir_reg : std_logic_vector(31 downto 0);
    signal gpio_b_out_af : std_logic_vector(31 downto 0);
    signal gpio_b_out : std_logic_vector(31 downto 0);

    signal ard_gpio_in_reg : std_logic_vector(31 downto 0);
    signal ard_gpio_out_reg : std_logic_vector(31 downto 0);
    signal ard_gpio_dir_reg : std_logic_vector(31 downto 0);
    signal ard_gpio_out_af : std_logic_vector(31 downto 0);
    signal ard_gpio_out : std_logic_vector(31 downto 0);

    signal gpio_af_mux_a : std_logic_vector(31 downto 0);   -- These AF selection bits can apply to any of GPIO_A, GPIO_B, or Arduino GPIO

    signal uart0_rx : std_logic;
    signal uart0_tx : std_logic;
    
begin
    -- Assignments
    NOT_KEY <= NOT KEY;

    HEX0 <= NOT hex3_hex0( 7 downto  0);
    HEX1 <= NOT hex3_hex0(15 downto  8);
    HEX2 <= NOT hex3_hex0(23 downto 16);
    HEX3 <= NOT hex3_hex0(31 downto 24);
    HEX4 <= NOT hex5_hex4( 7 downto  0);
    HEX5 <= NOT hex5_hex4(15 downto  8);

    DRAM_UDQM <= sdram_dqm(1);
    DRAM_LDQM <= sdram_dqm(0);

    -- Instantiations 
    comp_inst : component Computer_System
    port map (
        
        -- Global Signals
        system_pll_ref_clk_clk          => MAX10_CLK1_50,
        video_pll_ref_clk_clk           => MAX10_CLK2_50,
        system_pll_ref_reset_reset      => '0',        -- Logically dead...
        video_pll_ref_reset_reset       => '0',        -- Logically dead...
        master_reset_reset              => asyncReset, -- Real reset
        
        -- CPU Core JTAG
        vexriscv_core0_jtag_tms         => open,
        vexriscv_core0_jtag_tdi         => open,
        vexriscv_core0_jtag_tdo         => open,
        vexriscv_core0_jtag_tck         => open,
        
        -- uart0
        uart0_RXD                       => uart0_rx,
        uart0_TXD                       => uart0_tx,
        
        -- Accelerometer
        accel_I2C_SDAT                  => GSENSOR_SDI,
        accel_I2C_SCLK                  => GSENSOR_SCLK,
        accel_G_SENSOR_CS_N             => GSENSOR_CS_N,
        accel_G_SENSOR_INT              => GSENSOR_INT(1),
        
        -- Arduino GPIO
        arduino_gpio_in_port            => ard_gpio_in_reg,
        arduino_gpio_out_port           => ard_gpio_out_reg,
        arduino_gpio_dir_export         => ard_gpio_dir_reg,
        arduino_reset_n_export          => ARDUINO_RESET_N,
        
        -- GPIO A
        gpio_a_in_port                  => gpio_a_in_reg,
        gpio_a_out_port                 => gpio_a_out_reg,
        gpio_a_dir_export               => gpio_a_dir_reg,

        -- GPIO B
        gpio_b_in_port                  => gpio_b_in_reg,
        gpio_b_out_port                 => gpio_b_out_reg,
        gpio_b_dir_export               => gpio_b_dir_reg,
        
        -- GPIO Alternate Function Mux A
        gpio_af_mux_a_export            => gpio_af_mux_a,

        -- Hex Displays
        hex3_hex0_export                => hex3_hex0,
        hex5_hex4_export                => hex5_hex4,

        -- LEDs
        leds_export                     => LEDR,

        -- KEY
        pushbuttons_export              => NOT_KEY,
        
        -- SW
        slider_switches_export          => SW,

        -- SDRAM
        sdram_ba                        => DRAM_BA,
        sdram_addr                      => DRAM_ADDR,
        sdram_cas_n                     => DRAM_CAS_N,
        sdram_cke                       => DRAM_CKE,
        sdram_cs_n                      => DRAM_CS_N,
        sdram_dq                        => DRAM_DQ,
        sdram_dqm                       => sdram_dqm,
        sdram_ras_n                     => DRAM_RAS_N,
        sdram_we_n                      => DRAM_WE_N,
        sdram_clk_clk                   => DRAM_CLK,

        -- VGA
        vga_HS                          => VGA_HS,
        vga_VS                          => VGA_VS,
        vga_R                           => VGA_R,
        vga_G                           => VGA_G,
        vga_B                           => VGA_B,
        vga_CLK                         => open,
        vga_BLANK                       => open,
        vga_SYNC                        => open
    );

    -- uart0
    uart0_rx <= GPIO(7);
        
    -- Alternate function muxes, switch between standard GPIO port outputs and special (af) outputs
    -- afm.a.0 : Switch GPIO[9] between GPIO_A[9] and UART0_TX
    af_mux_a_0 : entity work.mux_2_to_1 port map (
        in0 => gpio_a_out_reg(9),   -- GPIO output bit
        in1 => uart0_tx,            -- Special function
        sel => gpio_af_mux_a(0),    -- GPIO direction bit
        output => gpio_a_out_af(9)  -- AF out, goes to tristate buffer
    );

    -- afm.a.1 : Switch master_reset between ~KEY[0] and '0' (inactive) 
    af_mux_a_1 : entity work.mux_2_to_1 port map (
        in0 => NOT_KEY(0),
        in1 => '0',
        sel => gpio_af_mux_a(1),
        output => asyncReset
    );

    -- GPIOs without alt func, bypass alt func muxes
    gpio_a_out_af(31 downto 10) <= gpio_a_out_reg(31 downto 10);
    gpio_a_out_af(8  downto  0) <= gpio_a_out_reg(8  downto  0);

    gpio_b_out_af(31 downto  0) <= gpio_b_out_reg(31 downto  0);

    ard_gpio_out_af(31 downto  0) <= ard_gpio_out_reg(31 downto  0);
    
    -- GPIO_A tristate output buffers
    gpio_a_tri: for i in 31 downto 0 generate
        gpio_a_tri_x : entity work.tristate_buff
        port map (
            data => gpio_a_out_af(i),
            out_en => gpio_a_dir_reg(i),
            output => gpio_a_out(i)
        );
    end generate gpio_a_tri;

    -- GPIO_B tristate output buffers
    gpio_b_tri: for i in 31 downto 0 generate
        gpio_b_tri_x : entity work.tristate_buff
        port map (
            data => gpio_b_out_af(i),
            out_en => gpio_b_dir_reg(i),
            output => gpio_b_out(i)
        );
    end generate gpio_b_tri;

    -- Arduino GPIO tristate output buffers
    ard_gpio_tri: for i in 31 downto 0 generate
        ard_gpio_tri_x : entity work.tristate_buff
        port map (
            data => ard_gpio_out_af(i),
            out_en => ard_gpio_dir_reg(i),
            output => ard_gpio_out(i)
        );
    end generate ard_gpio_tri;

    -- Map tristate buffer outputs to actual "inout" ports for GPIOs
    GPIO(31 downto 0) <= gpio_a_out(31 downto 0);
    GPIO(35 downto 32) <= gpio_b_out(3 downto 0);
    -- GPIO_B[31..4] unused
    ARDUINO_IO(15 downto 0) <= ard_gpio_out(15 downto 0);
    -- Arduino GPIO[31..16] unused
    
    -- Map GPIOs to input registers, these are always connected, no switching required
    gpio_a_in_reg(31 downto 0) <= GPIO(31 downto 0);

    gpio_b_in_reg(31 downto 4) <= (others => '0');      -- Unused
    gpio_b_in_reg(3 downto 0) <= GPIO(35 downto 32);

    ard_gpio_in_reg(31 downto 16) <= (others => '0');   -- Unused
    ard_gpio_in_reg(15 downto 0) <= ARDUINO_IO;

end arch;



