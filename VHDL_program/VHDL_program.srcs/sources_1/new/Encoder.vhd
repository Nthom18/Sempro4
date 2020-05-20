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
        leds: out STD_LOGIC_VECTOR (14 downto 0));
end Encoder;

architecture Behavioral of Encoder is
    signal false_boi : std_logic := '0';
begin
process(Clk, A, B)--, RESET_COUNT)
    variable ABab: std_logic_vector(3 downto 0) := "0000"; 
    variable counter: integer range -128 to 127 := 0;  --Should be chosen to fit the application, 
    variable reset: boolean := true; --Reset false means that this has already been registered as a switch
begin
    if rising_edge(Clk) then

        ABab := A & B & ABab(3) & ABab(2);  --A B a b = NewA NewB OldA OldB

        if ABab = "1110" and reset = true then  
            counter := counter - 1;
            reset := false;
        elsif ABab = "1101" and reset = true then
            counter := counter + 1;
            reset := false;
        elsif ABab = "0000" then
            reset := true; 
        end if;

        angle <= conv_std_logic_vector(counter, 8);
        
        --just for display purposes
        if counter = 0 then
            leds(3 downto 0) <= "0001";
        elsif counter = 1 then
            leds(3 downto 0) <= "0010";
        elsif counter = 2 then
            leds(3 downto 0) <= "0100";
        elsif counter = -1 then
            leds(14 downto 11) <= "1000";
        elsif counter = -2 then
            leds(14 downto 11) <= "0100";
        else
            leds(3 downto 0) <= "1111";
        end if;
    end if;
    
    -- Reset counter if transfer is ongoing
--    if RESET_COUNT = '0' then
--        counter := 0;
--    end if;
end process;
end Behavioral;