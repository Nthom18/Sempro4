----------------------------------------------------------------------------------
-- University of Southern Denmark
-- RB-PRO4 - Group 4 
-- Semesterproject in control and regulation of robotic systems
-- Module: Encoder_filter
-- Create Date: 03/28/2020 03:08:09 PM
-- NOTE: This module is based on material given during the course "Indlejret programmering og digital programmebar elektronik"
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

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
