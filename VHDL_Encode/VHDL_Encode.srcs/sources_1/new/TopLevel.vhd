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
--use IEEE.STD_LOGIC_ARITH.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity TopLevel is
  Port (JA: in STD_LOGIC_VECTOR (1 downto 0);
        clk: in STD_LOGIC;
        led: out STD_LOGIC_VECTOR (15 downto 0));
end TopLevel;

architecture Behavioral of TopLevel is

begin
process(clk, JA(1 downto 0))
    variable ABab: std_logic_vector(3 downto 0) := "0000";
    variable int_ABab: integer range 0 to 15;
    variable counter: integer range -1000 to 1000 := 0; 
    variable reset: boolean := true; --Reset false means that this has already been registered as a switch
    variable temp: std_logic_vector(3 downto 0) := "0000";
begin
    if rising_edge(clk) then
--        if counter < 3 then --JA(0) = '1' then
--        led(0) <= '1';
--        led(1) <= '0';
--    else
--        led(1) <= '1';
--        led(0) <= '0';
--    end if;
        ABab := JA(0) & JA(1) & ABab(3) & ABab(2);
        int_ABab := conv_integer(ABab);

        if ABab = "1110" and reset = true then
            counter := counter - 1;
            reset := false;
        elsif ABab = "1101" and reset = true then
            counter := counter + 1;
            reset := false;
        elsif ABab = "0000" then
            reset := true; 
        end if;
        
--        if counter = 0 then
--            led(3 downto 0) <= "0001";
--        elsif counter <= 10 then
--            led(3 downto 0) <= "0010";
--        elsif counter <= 20 then
--            led(3 downto 0) <= "0100";
--        elsif counter <= -10 then
--            led(15 downto 12) <= "1000";
--        elsif counter <= -20 then
--            led(15 downto 12) <= "0100";
----        else
----            led(3 downto 0) <= "1111";
--        end if;
        
        --just for display purposes
        if counter = 0 then
            led(3 downto 0) <= "0001";
        elsif counter = 1 then
            led(3 downto 0) <= "0010";
        elsif counter = 2 then
            led(3 downto 0) <= "0100";
        elsif counter = -1 then
            led(15 downto 12) <= "1000";
        elsif counter = -2 then
            led(15 downto 12) <= "0100";
        else
            led(3 downto 0) <= "1111";
        end if;
        --led(3 downto 0) <= temp;
--        case int_ABab is
--            when 0|5|10|15 =>
--                             led(0) <= '0'; --Count_enable <= '0'
                             
--                             led(1) <= '0'; --Illegal <= '0'
--          -------------------------------------------------
--            when 14 =>
--                             led(2) <= '1'; --Direction
--                             led(3) <= '1';--Count_enable <= 
--             when 1|7|8 =>      if true then
--                                led(2) <= '1'; -- direction;
--                                led(3) <= '1'; --count_enable;
--                             end if;                   
--          --------------------------------------------------
--              when 11 =>
--                             led(2) <= '0'; --direction
--                             led(3) <= '1'; ---count_enable
--              when 2|4|13 =>
--                             if true then
--                                led(2) <= '0'; --direction
--                                led(3) <= '1'; --count_enable
--                             end if;
--              when others =>
--                             led(1) <= '1'; --illegals
--          end case; 

    end if;
end process;
end Behavioral;
