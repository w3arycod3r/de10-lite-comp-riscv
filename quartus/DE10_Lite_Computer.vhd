-- Porting de10-lite nios ii computer to riscv core
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library work;


entity DE10_Lite_Computer is
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
end DE10_Lite_Computer;

architecture arch of DE10_Lite_Computer is

    -- Components
    component Computer_System is
    port (
        accelerometer_I2C_SDAT      : inout std_logic                     := 'X';             -- I2C_SDAT
        accelerometer_I2C_SCLK      : out   std_logic;                                        -- I2C_SCLK
        accelerometer_G_SENSOR_CS_N : out   std_logic;                                        -- G_SENSOR_CS_N
        accelerometer_G_SENSOR_INT  : in    std_logic                     := 'X';             -- G_SENSOR_INT
        arduino_gpio_export         : inout std_logic_vector(15 downto 0) := (others => 'X'); -- export
        arduino_reset_n_export      : out   std_logic;                                        -- export
        expansion_jp1_0_export      : inout std_logic_vector(17 downto 0) := (others => 'X'); -- export
        expansion_jp1_1_export      : inout std_logic_vector(17 downto 0) := (others => 'X'); -- export
        hex3_hex0_export            : out   std_logic_vector(31 downto 0);                    -- export
        hex5_hex4_export            : out   std_logic_vector(15 downto 0);                    -- export
        leds_export                 : out   std_logic_vector(9 downto 0);                     -- export
        pushbuttons_export          : in    std_logic_vector(1 downto 0)  := (others => 'X'); -- export
        sdram_ba                    : out   std_logic_vector(1 downto 0);                     -- ba
        sdram_addr                  : out   std_logic_vector(12 downto 0);                    -- addr
        sdram_cas_n                 : out   std_logic;                                        -- cas_n
        sdram_cke                   : out   std_logic;                                        -- cke
        sdram_cs_n                  : out   std_logic;                                        -- cs_n
        sdram_dq                    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
        sdram_dqm                   : out   std_logic_vector(1 downto 0);                     -- dqm
        sdram_ras_n                 : out   std_logic;                                        -- ras_n
        sdram_we_n                  : out   std_logic;                                        -- we_n
        sdram_clk_clk               : out   std_logic;                                        -- clk
        slider_switches_export      : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
        system_pll_ref_clk_clk      : in    std_logic                     := 'X';             -- clk
        system_pll_ref_reset_reset  : in    std_logic                     := 'X';             -- reset
        vexriscvavalon_0_jtag_tms   : in    std_logic                     := 'X';             -- tms
        vexriscvavalon_0_jtag_tdi   : in    std_logic                     := 'X';             -- tdi
        vexriscvavalon_0_jtag_tdo   : out   std_logic;                                        -- tdo
        vexriscvavalon_0_jtag_tck   : in    std_logic                     := 'X';             -- tck
        vga_CLK                     : out   std_logic;                                        -- CLK
        vga_HS                      : out   std_logic;                                        -- HS
        vga_VS                      : out   std_logic;                                        -- VS
        vga_BLANK                   : out   std_logic;                                        -- BLANK
        vga_SYNC                    : out   std_logic;                                        -- SYNC
        vga_R                       : out   std_logic_vector(3 downto 0);                     -- R
        vga_G                       : out   std_logic_vector(3 downto 0);                     -- G
        vga_B                       : out   std_logic_vector(3 downto 0);                     -- B
        video_pll_ref_clk_clk       : in    std_logic                     := 'X';             -- clk
        video_pll_ref_reset_reset   : in    std_logic                     := 'X';             -- reset
        reset_controller_0_reset_in0_reset : in    std_logic                     := 'X'             -- reset

	);
	end component Computer_System;

    -- Signals
    signal asyncReset : std_logic;

    signal hex3_hex0 : std_logic_vector(31 downto 0);
    signal hex5_hex4 : std_logic_vector(15 downto 0);

    signal sdram_dqm : std_logic_vector(1 downto 0);
    
begin
    -- Assignments
    HEX0 <= NOT hex3_hex0( 7 downto  0);
    HEX1 <= NOT hex3_hex0(15 downto  8);
    HEX2 <= NOT hex3_hex0(23 downto 16);
    HEX3 <= NOT hex3_hex0(31 downto 24);
    HEX4 <= NOT hex5_hex4( 7 downto  0);
    HEX5 <= NOT hex5_hex4(15 downto  8);

    DRAM_UDQM <= sdram_dqm(1);
    DRAM_LDQM <= sdram_dqm(0);
    
    asyncReset <= not KEY(0);
    
    -- Instantiations 
    comp_inst : component Computer_System
    port map (

        -- Global Signals
        system_pll_ref_clk_clk               => MAX10_CLK1_50,
        video_pll_ref_clk_clk                => MAX10_CLK2_50,
        system_pll_ref_reset_reset           => '0',    -- Logically dead...
        video_pll_ref_reset_reset            => '0',    -- Logically dead...
        reset_controller_0_reset_in0_reset   => asyncReset, -- Real reset

        -- CPU Core JTAG
        vexriscvavalon_0_jtag_tms   => open,
        vexriscvavalon_0_jtag_tdi   => open,
        vexriscvavalon_0_jtag_tdo   => open,
        vexriscvavalon_0_jtag_tck   => open,
        
        -- Accelerometer
        accelerometer_I2C_SDAT      => GSENSOR_SDI,
        accelerometer_I2C_SCLK      => GSENSOR_SCLK,
        accelerometer_G_SENSOR_CS_N => GSENSOR_CS_N,
        accelerometer_G_SENSOR_INT  => GSENSOR_INT(1),

        -- Arduino GPIO
        arduino_gpio_export         => ARDUINO_IO,
        arduino_reset_n_export      => ARDUINO_RESET_N,

        -- GPIO
        expansion_jp1_0_export      => GPIO(35 downto 18),
        expansion_jp1_1_export      => GPIO(17 downto 0),

        -- Hex Displays
        hex3_hex0_export            => hex3_hex0,
        hex5_hex4_export            => hex5_hex4,

        -- LEDs
        leds_export                 => LEDR,

        -- KEY
        pushbuttons_export          => NOT KEY,
        
        -- SDRAM
        sdram_ba                    => DRAM_BA,
        sdram_addr                  => DRAM_ADDR,
        sdram_cas_n                 => DRAM_CAS_N,
        sdram_cke                   => DRAM_CKE,
        sdram_cs_n                  => DRAM_CS_N,
        sdram_dq                    => DRAM_DQ,
        sdram_dqm                   => sdram_dqm,
        sdram_ras_n                 => DRAM_RAS_N,
        sdram_we_n                  => DRAM_WE_N,
        sdram_clk_clk               => DRAM_CLK,

        -- SW
        slider_switches_export      => SW,

        -- VGA
        vga_HS                      => VGA_HS,
        vga_VS                      => VGA_VS,
        vga_R                       => VGA_R,
        vga_G                       => VGA_G,
        vga_B                       => VGA_B,
        vga_CLK                     => open,
        vga_BLANK                   => open,
        vga_SYNC                    => open
    );
        
    
end arch;



