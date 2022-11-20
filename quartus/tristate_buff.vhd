library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;
library work;

entity tristate_buff is
    port (
        data : in std_logic;
        out_en : in std_logic;
        output : inout std_logic
    );
end entity tristate_buff;

architecture rtl of tristate_buff is
begin

    -- Altera devices contain tri-state buffers in the I/O.  Thus, a tri-state
    -- buffer must feed a top-level I/O in the final design.  Otherwise, the
    -- Quartus Prime software will convert the tri-state buffer into logic.
    output <= data when (out_en = '1') else 'Z';
    
end architecture rtl;