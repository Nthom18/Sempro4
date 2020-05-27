----------------------------------------------------------------------------------
-- University of Southern Denmark
-- RB-PRO4 - Group 4 
-- Semesterproject in control and regulation of robotic systems
-- Module: Encoder
-- Create Date: 03/28/2020 03:08:09 PM
-- NOTE: This module is based on material given during the course "Indlejret programmering og digital programmebar elektronik"
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;

entity Encoder is
  Port (Clk: in STD_LOGIC;
        A: in STD_LOGIC;
        B: in STD_LOGIC;
        RESET_COUNT : in STD_LOGIC;
        angle: out STD_LOGIC_VECTOR(7 downto 0));
end Encoder;

architecture Behavioral of Encoder is
begin
    process(clk)
        variable ABab: STD_LOGIC_VECTOR(3 downto 0) := "0000";
        variable int_ABab: integer range 0 to 15;
        variable counter: integer range -128 to 127 := 0;
    begin
        if rising_edge(clk) then
        
            ABab := A & B & ABab(3) & ABab(2);
            int_ABab := conv_integer(ABab);
            
            case int_ABab is
                when 0|5|10|15 => Null;
                when 14 => counter := counter -1;
                when 1|7|8 => Null;
                when 11 => counter := counter +1;
                when 2|4|13 => Null;
                when others => Null;
            end case;
            
            angle <= conv_std_logic_vector(counter, 8);
        
            if RESET_COUNT = '1' then
                counter := 0;
            end if;
        end if;
    end process;
end Behavioral;