----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/25/2020 04:39:40 PM
-- Design Name: 
-- Module Name: TopLevel - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
--use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

use IEEE.STD_LOGIC_ARITH.ALL;

--use IEEE.STD_LOGIC_ARITH.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Encoder is
  Port (Clk: in STD_LOGIC;
        A: in STD_LOGIC;
        B: in STD_LOGIC;
        RESET_COUNT : in STD_LOGIC;
        angle: out STD_LOGIC_VECTOR(7 downto 0);
        leds: out STD_LOGIC_VECTOR(14 downto 0));

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


--process(Clk, A, B, RESET_COUNT)
--    variable ABab: std_logic_vector(3 downto 0) := "0000"; 
--    variable counter: integer range -128 to 127 := 0;  --Should be chosen to fit the application, 
--    variable reset: boolean := true; --Reset false means that this has already been registered as a switch
--begin
--    if rising_edge(Clk) then

--        ABab := A & B & ABab(3) & ABab(2);  --A B a b = NewA NewB OldA OldB

--        if ABab = "1110" and reset = true then  
--            counter := counter - 1;
--            reset := false;
--        elsif ABab = "1101" and reset = true then
--            counter := counter + 1;
--            reset := false;
--        elsif ABab = "0000" then
--            reset := true; 
--        end if;

--        angle <= conv_std_logic_vector(counter, 8);
        
--    end if;
    
--    --HERE!
--    --Reset counter if transfer is ongoing
--    if RESET_COUNT = '1' then
--        counter := 0;
--    end if;
--end process;
end Behavioral;