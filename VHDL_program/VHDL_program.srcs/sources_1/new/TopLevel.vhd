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
    GENERIC(data_length : INTEGER := 16);
        Port (clk :   in STD_LOGIC;
              JA: in STD_LOGIC_VECTOR (2 downto 0); --perhaps fix inout thing
              JB: in STD_LOGIC_VECTOR (7 downto 2);
              --JB: in STD_LOGIC_VECTOR (6 downto 7);
              JA3: out STD_LOGIC;
              led: out STD_LOGIC_VECTOR (15 downto 0); --Should only be the one
              JC: out STD_LOGIC_VECTOR (7 downto 0)); 
              
             -- sw : in STD_LOGIC_VECTOR (15 downto 0)); -- Maybee not used
end TopLevel;



architecture Behavioral of TopLevel is

        COMPONENT PWM_Module
            generic(N: natural := 8; --Should be chosen to fit the applicaiton
                    MAX: natural := 128); --Should be chosen to fit the application
            Port ( Clk :   in STD_LOGIC;
                   PCM :   in STD_LOGIC_VECTOR (N-1 downto 0);
                   PWM_pos :   out STD_LOGIC;
                   PWM_neg : out STD_LOGIC);
        end COMPONENT;
        
        
        COMPONENT Encoder
            Port (Clk: in STD_LOGIC;
                  A: in STD_LOGIC;
                  B: in STD_LOGIC;
                  RESET_COUNT : in STD_LOGIC;
                  angle: out STD_LOGIC_VECTOR (7 downto 0);
                  leds: out STD_LOGIC_VECTOR (14 downto 0));
        end COMPONENT;
        
        COMPONENT SPI_Analysis
        Port(Clk: in STD_LOGIC;
             data_received : in STD_LOGIC_VECTOR (15 downto 0);
             enable_MISO : out STD_LOGIC;
             AngleMotor1: in std_logic_vector(7 downto 0);    
             AngleMotor2: in std_logic_vector(7 downto 0);
             data_send: out std_logic_vector(15 downto 0);
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

    signal data_send : STD_LOGIC_VECTOR (15 downto 0) := (others => '0');--; -- := "10110100"; --should be set to be output from encoder (perhaps)
   
   --test
    signal data_send2 : STD_LOGIC_VECTOR (15 downto 0) := "1011010010110100"; --should be set to be output from encoder (perhaps)
   --test end
   
    signal busy_MISO : std_logic;
    signal data_receive : STD_LOGIC_VECTOR (15 downto 0);
    signal ready : STD_LOGIC;
    signal busy : STD_LOGIC;    
        
    
    --????
    signal EN_A : STD_LOGIC;
    signal EN_B : STD_LOGIC;
    ---???
    
        
    --Variable registers 
    --shared variable 
    signal AngleMotor1: std_logic_vector(7 downto 0) := "01100101"; --(others => '0');    
    signal AngleMotor2: std_logic_vector(7 downto 0) := "01100101"; --(others => '0');
    signal RESET_ANGLE1 : std_logic := EN_A;-- '0';
    signal RESET_ANGLE2 : std_logic := EN_B; ---'0';
    signal VelocityMotor1: std_logic_vector(15 downto 0) := (others => '0');
    signal VelocityMotor2: std_logic_vector(15 downto 0) := (others => '0');
    signal PWMMotor1: std_logic_vector(7 downto 0) := (others => '0');
    signal PWMMotor2: std_logic_vector(7 downto 0) := (others => '0');
    
    
    --temporary signals
    signal Leds_temp: std_logic_vector(14 downto 0) := (others => '0');
    signal led15_temp: std_logic := '0';
    signal Leds_temp2: std_logic_vector(14 downto 0) := (others => '0');
    
begin 
    -- EN_A and EN_B signals. These allow the PWM signals to control the motors. Perhaps not always on.
    JC(0) <= '1';--EN_A; -- EN_A  -- '1';
    JC(4) <= '1'; --EN_B; -- EN_B  -- '1'; 
    
    Hardware_Control_1: PWM_Module PORT MAP(Clk => clk, PCM => PWMMotor1, PWM_pos => JC(6), PWM_neg => JC(5));
    
    Hardware_Control_2: PWM_Module PORT MAP(Clk => clk, PCM => PWMMotor2, PWM_pos => JC(2), PWM_neg => JC(1));
    
    --Use signals from motor 2 in this
    Hardware_Feedback_1: Encoder PORT MAP(Clk => clk, A => JB(4), B => JB(5), RESET_COUNT => ready, angle => AngleMotor1, leds => Leds_temp2); --(Clk => clk, A => JB(6), B => JB(2), RESET_COUNT => ready, angle => AngleMotor1, leds => Leds_temp2);
    
    --Use signals from motor 2 in this
    Hardware_Feedback_2: Encoder PORT MAP(Clk => clk, A => JB(4), B => JB(5), RESET_COUNT => ready, angle => AngleMotor2, leds => Leds_temp); --(Clk => clk, A => JB(7), B => JB(3), RESET_COUNT => ready, angle => AngleMotor2, leds => Leds_temp);

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
              data => data_send,---2,  --              data => data_send,  --
              miso => JA3,
              busy => busy_MISO);
              
    busy <= busy_MOSI OR busy_MISO; 
end Behavioral;
