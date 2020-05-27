----------------------------------------------------------------------------------
-- University of Southern Denmark
-- RB-PRO4 - Group 4 
-- Semesterproject in control and regulation of robotic systems
-- Module: SPI_analysis
-- Create Date: 03/28/2020 03:08:09 PM
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity SPI_Analysis is
  Port (Clk: in STD_LOGIC;
        data_received : in STD_LOGIC_VECTOR (15 downto 0);
        enable_MISO : out STD_LOGIC;
        AngleMotor1: in std_logic_vector(7 downto 0);    
        AngleMotor2: in std_logic_vector(7 downto 0);
        data_send: out std_logic_vector(15 downto 0);
        PWMMotor1: out std_logic_vector(7 downto 0);
        PWMMotor2: out std_logic_vector(7 downto 0));
end SPI_Analysis;


architecture Behavioral of SPI_Analysis is
begin
    
    data_send <= AngleMotor1(7 downto 0) & AngleMotor2(7 downto 0);
    PWMMotor1 <= data_received(15 downto 8);
    PWMMotor2 <= data_received(7 downto 0);
  
end Behavioral;
