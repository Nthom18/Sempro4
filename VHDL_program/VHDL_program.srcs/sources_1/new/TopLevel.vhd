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
              JB: in STD_LOGIC_VECTOR (7 downto 2);
              --JB: in STD_LOGIC_VECTOR (6 downto 7);
              JA3: out STD_LOGIC;
              led: out STD_LOGIC_VECTOR (15 downto 0)); --Should only be the one
             -- sw : in STD_LOGIC_VECTOR (15 downto 0)); -- Maybee not used
end TopLevel;



architecture Behavioral of TopLevel is

        COMPONENT PWM_Module
            generic(N: natural := 8; --Should be chosen to fit the applicaiton
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
                  angle: out STD_LOGIC_VECTOR(15 downto 0);
                  leds: out STD_LOGIC_VECTOR (14 downto 0));
        end COMPONENT;
        
        COMPONENT SPI_Analysis
        Port(Clk: in STD_LOGIC;
             data_received : in STD_LOGIC_VECTOR (7 downto 0);
             enable_MISO : out STD_LOGIC;
             AngleMotor1: in std_logic_vector(15 downto 0);    
             AngleMotor2: in std_logic_vector(15 downto 0);
             data_send: out std_logic_vector(7 downto 0);
             PWMMotor1: out std_logic_vector(7 downto 0);
             PWMMotor2: out std_logic_vector(7 downto 0));
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
        
        
    --Variable registers 
    --shared variable 
    signal AngleMotor1: std_logic_vector(15 downto 0) := (others => '0');    
    signal AngleMotor2: std_logic_vector(15 downto 0) := (others => '0');
    signal VelocityMotor1: std_logic_vector(15 downto 0) := (others => '0');
    signal VelocityMotor2: std_logic_vector(15 downto 0) := (others => '0');
    signal PWMMotor1: std_logic_vector(7 downto 0) := (others => '0');
    signal PWMMotor2: std_logic_vector(7 downto 0) := (others => '0');
    
    
    --temporary signals
    signal Leds_temp: std_logic_vector(14 downto 0) := (others => '0');
    signal led15_temp: std_logic := '0';
    
begin
    Hardware_Control_1: PWM_Module PORT MAP(Clk => clk, PCM => PWMMotor1, PWM => led(15));
    
    Hardware_Control_2: PWM_Module PORT MAP(Clk => clk, PCM => PWMMotor2, PWM => led15_temp);
    
    --Use signals from motor 2 in this
    Hardware_Feedback_1: Encoder PORT MAP(Clk => clk, A => JB(6), B => JB(2), angle => AngleMotor1, leds => led(14 downto 0));
    
    --Use signals from motor 2 in this
    Hardware_Feedback_2: Encoder PORT MAP(Clk => clk, A => JB(7), B => JB(3), angle => AngleMotor2, leds => Leds_temp);

    Interface_Analysis : SPI_Analysis
    port map (Clk => clk,
              data_received => data_receive,
              enable_MISO => busy_MISO,
              AngleMotor1 => AngleMotor1,   
              AngleMotor2 => AngleMotor2,
              data_send => data_send,
              PWMMotor1 => PWMMotor1,
              PWMMotor2 => PWMMotor2);

    InterfaceTiva1 : SPI_MOSI   --In 
    port map (sck   => JA(0),
              ss    => JA(1),
              mosi  => JA(2),
              data  => data_receive,
              busy  => busy_MOSI,
              ready => ready);

    InterfaceTiva2 : SPI_MISO   --Out
    port map (sck  => JA(0),
              ss   => JA(1),
              data => data_send,
              miso => JA3,
              busy => busy_MISO);
              
    busy <= busy_MOSI OR busy_MISO; 
end Behavioral;
