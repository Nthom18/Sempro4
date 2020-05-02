----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 04/28/2020 10:44:29 AM
-- Design Name: 
-- Module Name: SPI_Analysis - Behavioral
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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

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

--process(ready)
--    variable SPI_angle_send : std_logic := '0';
--    variable SPI_register : std_logic_vector(6 downto 0);
    
--    begin
--        if (Rising_edge(ready)) then
--            --Last received data was receive new angle
--            if (SPI_angle_send = '1') then
--                SPI_angle_send := '0';  --reset bool to allow for next transfer
--                case SPI_register(6 downto 0) is
--                    when "0000010" =>
--                        PWMMotor1 <= data_received(7 downto 0); 
                        
--                    when "0000011" =>
--                        PWMMotor2 <= data_received(7 downto 0); 
                    
--                    when others =>
--                end case;     
--            -- "read" - send angle placement in register data_received(6 downto 0)
--            elsif (data_received(7) = '0') then  
--                case  data_received(6 downto 0) is
--                    when "0000000" =>
--                        data_send <= AngleMotor1(7 downto 0);
--                    when "0000001" =>
--                        data_send <= AngleMotor2(7 downto 0);
--                    when others =>                  
--                 end case;
--                 enable_MISO <= '1';
                  
--            --"Write" - change register(6 downto 0) to the content of next SPI transmission (MOSI)
--            elsif (data_received(7) = '1' ) then
--                SPI_angle_send := '1';
--                SPI_register := data_received(6 downto 0);                    
--            end if; 
--        end if;
--    end process;
    
    data_send <= AngleMotor1(7 downto 0) & AngleMotor2(7 downto 0);
    PWMMotor1 <= data_received(15 downto 8);
    PWMMotor2 <= data_received(7 downto 0);
  
end Behavioral;
