----------------------------------------------------------------------------------
-- University of Southern Denmark
-- RB-PRO4 - Group 4 
-- Semesterproject in control and regulation of robotic systems
-- Module: PWM
-- Create Date: 03/28/2020 03:08:09 PM
-- NOTE: This module is based on material given during the course "Indlejret programmering og digital programmebar elektronik"
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity PWM_Module is
    generic (N: natural := 8; --Should be chosen to fit the applicaiton (have chosen 8 bits because this is the number of bits pr data transfer, fix if nessesary)
             MAX: natural := 128); --Should be chosen to fit the application
    Port ( Clk :   in STD_LOGIC;
           PCM :   in STD_LOGIC_VECTOR (N-1 downto 0);
           PWM_pos :   out STD_LOGIC;
           PWM_neg :   out STD_LOGIC);
end PWM_Module;

architecture Behavioral of PWM_Module is
    signal PWM_Count:  STD_LOGIC_VECTOR (N-1 downto 0) := (others=> '0');
    signal Scaled_clk: STD_LOGIC;
    signal PWM : STD_LOGIC;
begin

    process(Clk)
        variable Scalex:       integer := 5000; --should be chosen to fit the application
        variable Count: integer range 0 to 50000001 := 0; 
    begin  
    --Implements clockscaling
    if rising_edge(Clk)then
        if Count>=Scalex then
            Scaled_clk <= '1'; --Sets Scaled_clk to one once every scaled_clock cycle
            Count      := 0;
        else
            Scaled_clk <= '0';
            Count      := Count+1;
        end if;   
    end if;
   end process;
    
   PWM_Generator: process( Clk, PCM)
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
    if PWM_Count < (PCM and "01111111") then
            PWM <= '1';
        else
            PWM <= '0';
        end if; 
    
    end process;
    
    process (Clk)
    begin
        if Rising_edge(clk) then
            if PCM(7) = '0' then
                PWM_pos <= PWM;
                PWM_neg <= '0';
            else
                PWM_neg <= PWM;
                PWM_pos <= '0';
            end if;
        end if;
    end process;
    
end Behavioral;