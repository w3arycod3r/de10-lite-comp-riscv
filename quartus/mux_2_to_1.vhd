library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
library work;

entity mux_2_to_1 is
    port (
        in0 : in std_logic;
        in1 : in std_logic;
        sel : in std_logic;
        output : out std_logic
    );
end entity mux_2_to_1;

architecture rtl of mux_2_to_1 is
begin
    
    -- MUX
    process(sel, in0, in1)
    begin
        case sel is
            when '0' => output <= in0;
            when '1' => output <= in1;
            when others => output <= in0;
        end case;
    end process;
    
end architecture rtl;