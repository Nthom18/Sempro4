----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/13/2020 01:13:32 PM
-- Design Name: 
-- Module Name: PWM - Behavioral
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

entity PWM_v1 is
    generic (N: natural := 7; --Should be choosen to fit the applicaiton
             MAX: natural := 128); --Should be choosen to fit the application
    Port ( Clk :   in STD_LOGIC;
           Scale : in STD_LOGIC;
           PCM :   in STD_LOGIC_VECTOR (N-1 downto 0);
           PWM :   out STD_LOGIC);
end PWM_v1;

architecture Behavioral of PWM_v1 is
    signal PWM_Count:  STD_LOGIC_VECTOR (N-1 downto 0) := (others=> '0');
    signal Scaled_clk: STD_LOGIC;
begin

    process(Clk, Scale)
        variable Scalex:       integer;
        constant Scale100msek: integer  :=  5000; --Should be choosen to fit apppication
        constant Scale500msek: integer  :=  25000; --Should be choosen to fit application
        variable Count: integer range 0 to 50000001 := 0;
    
    --should possibly just be a certain scaling determined from the applikation 
    begin --Determines the scaling factor from input
        if Scale='1' then
            Scalex := Scale100msek;
        else
            Scalex := Scale500msek;
    end if;    
    
    --Implements clockscaling
    if rising_edge(Clk)then
        if Count>=Scalex then
            Scaled_clk <= '1';--Sets Scaled_clk to one once every scaled_clock cycle
            Count      := 0;
        else
            Scaled_clk <= '0';
            Count      := Count+1;
        end if;   
    end if;
   end process;

   PWM_Generator: process( Clk, PWM_Count, PCM)
    begin
      if rising_edge(Clk) then
          if Scaled_Clk='1' then
             if PWM_Count<MAX-1 then
                 PWM_Count <= PWM_Count+1;
                else
                    PWM_Count <= (others => '0');
                end if; 
            end if;
     end if;
    if PWM_Count < PCM then
            PWM <= '1';
        else
            PWM <= '0';
        end if; 
    
    end process;
    
end Behavioral;