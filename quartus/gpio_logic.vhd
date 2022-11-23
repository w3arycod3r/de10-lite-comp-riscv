-- File:    gpio_logic.vhd
-- Purpose: Contain logic for GPIO ports, including pin muxes and tristate buffers

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
library work;

entity gpio_logic is
    port (

        -- GPIO_A
        gpio_a_in_reg       : out std_logic_vector(31 downto 0);    -- To input register in PIO periph
        gpio_a_out_reg      : in std_logic_vector(31 downto 0);     -- From output register in PIO periph
        gpio_a_dir_reg      : in std_logic_vector(31 downto 0);     -- "Direction" register from PIO periph: '0' for input, '1' for output    
        
        -- GPIO_B
        gpio_b_in_reg       : out std_logic_vector(31 downto 0);
        gpio_b_out_reg      : in std_logic_vector(31 downto 0);
        gpio_b_dir_reg      : in std_logic_vector(31 downto 0);

        -- ARD_GPIO
        ard_gpio_in_reg     : out std_logic_vector(31 downto 0);
        ard_gpio_out_reg    : in std_logic_vector(31 downto 0);
        ard_gpio_dir_reg    : in std_logic_vector(31 downto 0);    

        -- Alt Func selection
        gpio_af_mux_a       : in std_logic_vector(31 downto 0);   -- These AF selection bits can apply to any of GPIO_A, GPIO_B, or Arduino GPIO

        -- Alternate Functions
        uart0_rx            : out std_logic;
        uart0_tx            : in std_logic;    

        -- IO Ports
        GPIO                : inout std_logic_vector(35 downto 0);
        ARDUINO_IO          : inout std_logic_vector(15 downto 0)
    
    );
end entity gpio_logic;

architecture rtl of gpio_logic is

    -- Signals
    signal gpio_a_out_af : std_logic_vector(31 downto 0);   -- Output from alt func muxes
    signal gpio_a_out : std_logic_vector(31 downto 0);      -- To actual GPIO output pins, output from tri-state buffers, drive "inout" ports
    
    signal gpio_b_out_af : std_logic_vector(31 downto 0);   
    signal gpio_b_out : std_logic_vector(31 downto 0);      

    signal ard_gpio_out_af : std_logic_vector(31 downto 0);
    signal ard_gpio_out : std_logic_vector(31 downto 0);

begin

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

    -- ARD_GPIO tristate output buffers
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
    -- ARD_GPIO[31..16] unused
    
    -- Map GPIOs to input registers, these are always connected, no switching required
    gpio_a_in_reg(31 downto 0) <= GPIO(31 downto 0);

    gpio_b_in_reg(31 downto 4) <= (others => '0');      -- Unused
    gpio_b_in_reg(3 downto 0) <= GPIO(35 downto 32);

    ard_gpio_in_reg(31 downto 16) <= (others => '0');   -- Unused
    ard_gpio_in_reg(15 downto 0) <= ARDUINO_IO;
    
    
end architecture rtl;