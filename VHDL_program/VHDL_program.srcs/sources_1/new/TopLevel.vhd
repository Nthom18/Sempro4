----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/28/2020 03:08:09 PM
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity TopLevel is
    GENERIC(data_length : INTEGER := 8);
        Port (clk :   in STD_LOGIC;
              JA: in STD_LOGIC_VECTOR (2 downto 0); --perhaps fix inout thing
              JB: in STD_LOGIC_VECTOR (1 downto 0);
              JA3: out STD_LOGIC;
              led: out STD_LOGIC_VECTOR (15 downto 0)); --Should only be the one
             -- sw : in STD_LOGIC_VECTOR (15 downto 0)); -- Maybee not used
end TopLevel;



architecture Behavioral of TopLevel is

        COMPONENT PWM_Module
            generic(N: natural := 7; --Should be chosen to fit the applicaiton
                    MAX: natural := 128); --Should be chosen to fit the application
            Port ( Clk :   in STD_LOGIC;
                   PCM :   in STD_LOGIC_VECTOR (N-1 downto 0);
                   PWM :   out STD_LOGIC);
        end COMPONENT;
        constant PWM_temp: STD_LOGIC_VECTOR (6 downto 0) := "1000000";
        
        COMPONENT Encoder
            Port (Clk: in STD_LOGIC;
                  A: in STD_LOGIC;
                  B: in STD_LOGIC;
                  leds: out STD_LOGIC_VECTOR (14 downto 0));
        end COMPONENT;
        
   component SPI_MOSI
        GENERIC(data_length : INTEGER := data_length);
        port (sck   : in std_logic;
              ss    : in std_logic;
              mosi  : in std_logic;
              data  : out std_logic_vector (data_length-1 downto 0);
              busy  : out std_logic;
              ready : out std_logic);
    end component;
    
    signal busy_MOSI  : std_logic;
    
    component SPI_MISO
        port (sck  : in std_logic;
              ss   : in std_logic;
              data : in std_logic_vector (data_length-1 downto 0);
              miso : out std_logic;
              busy : out std_logic);
    end component;

    signal data_send : STD_LOGIC_VECTOR (7 downto 0) := "10110100"; --should be set to be output from encoder (perhaps)
   
    signal busy_MISO : std_logic;
    signal data_receive : STD_LOGIC_VECTOR (7 downto 0);
    signal ready : STD_LOGIC;
    signal busy : STD_LOGIC;    
        
    
begin
    Hardware_Control: PWM_Module PORT MAP(Clk => clk, PCM => PWM_temp, PWM => led(15));
    
    Hardware_Feedback: Encoder PORT MAP(Clk => clk, A => JB(1), B => JB(0), leds => led(14 downto 0));

   
    InterfaceTiva1 : SPI_MOSI
    port map (sck   => JA(0),
              ss    => JA(1),
              mosi  => JA(2),
              data  => data_receive,
              busy  => busy_MOSI,
              ready => ready);

    InterfaceTiva2 : SPI_MISO
    port map (sck  => JA(0),
              ss   => JA(1),
              data => data_send,
              miso => JA3,
              busy => busy_MISO);
              
    busy <= busy_MOSI OR busy_MISO; 
end Behavioral;
