----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 13.03.2020 14:50:14
-- Design Name: 
-- Module Name: test - Behavioral
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

entity test is
    Port (sck : IN STD_LOGIC; --sck
          ss : IN STD_LOGIC; --ss
          mosi : IN STD_LOGIC; --mosi
          miso: OUT STD_LOGIC; --miso
          led : OUT STD_LOGIC_VECTOR (15 DOWNTO 0);
          sw  : IN STD_LOGIC_VECTOR(15 DOWNTO 0) );
end test;

architecture Behavioral of test is

begin
    led(7 downto 0) <= sw(7 downto 0);
    led(15) <= sck;
    led(14) <= ss;
    led(13) <= mosi;
    miso <= '1';
end Behavioral;
