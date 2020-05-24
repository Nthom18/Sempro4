----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 24.05.2020 14:43:40
-- Design Name: 
-- Module Name: Encoder_filter - Behavioral
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Encoder_filter is
  Port ( Clk : in STD_LOGIC;
         A,B : in STD_LOGIC;
         Ax,Bx : out STD_LOGIC);
end Encoder_filter;

architecture Behavioral of Encoder_filter is
begin
    process(Clk)
        variable AB: STD_LOGIC_VECTOR(1 downto 0);
    begin
        if clk'event and clk='1' then
            AB := A&B;
            case AB is
                when "00" => Ax <= '0';
                when "10" => Bx <= '0';
                when "01" => Bx <= '1';
                when "11" => Ax <= '1';
            end case;
        end if;
    end process;
end Behavioral;
