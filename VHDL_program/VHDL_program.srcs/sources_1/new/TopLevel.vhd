----------------------------------------------------------------------------------
-- University of Southern Denmark
-- RB-PRO4 - Group 4 
-- Semesterproject in control and regulation of robotic systems
-- Module: TopLevel
-- Create Date: 03/28/2020 03:08:09 PM
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity TopLevel is
    GENERIC(data_length : INTEGER := 16);
        Port (clk :   in STD_LOGIC;
              JA: in STD_LOGIC_VECTOR (2 downto 0);
              JB: in STD_LOGIC_VECTOR (7 downto 2);
              JA3: out STD_LOGIC;
              led: out STD_LOGIC_VECTOR (15 downto 0);
              JC: out STD_LOGIC_VECTOR (7 downto 0)); 
end TopLevel;

architecture Behavioral of TopLevel is

        COMPONENT PWM_Module
            generic(N: natural := 8; 
                    MAX: natural := 128);
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
                  angle: out STD_LOGIC_VECTOR (7 downto 0));                  
        end COMPONENT;
        
        component Encoder_filter
        port (Clk : in std_logic;
              A   : in std_logic;
              B   : in std_logic;
              Ax  : out std_logic;
              Bx  : out std_logic);
        end component;

        signal Ax1  : std_logic;
        signal Bx1  : std_logic;
        signal Ax2  : std_logic;
        signal Bx2  : std_logic;
        
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

    signal data_send : STD_LOGIC_VECTOR (15 downto 0) := (others => '0');

    signal busy_MISO : std_logic;
    signal data_receive : STD_LOGIC_VECTOR (15 downto 0);
    signal ready : STD_LOGIC;
    signal busy : STD_LOGIC;    
        
    signal EN_A : STD_LOGIC := '1';
    signal EN_B : STD_LOGIC := '1';
    
        
    --Variable registers 
    signal AngleMotor1: std_logic_vector(7 downto 0) := (others => '0');    
    signal AngleMotor2: std_logic_vector(7 downto 0) := (others => '0');
    signal VelocityMotor1: std_logic_vector(15 downto 0) := (others => '0');
    signal VelocityMotor2: std_logic_vector(15 downto 0) := (others => '0');
    signal PWMMotor1: std_logic_vector(7 downto 0) := (others => '0');
    signal PWMMotor2: std_logic_vector(7 downto 0) := (others => '0');
    
begin 
    -- Set EN_A and EN_B pins high on the H-bridge circuit to allow for PWM control.
    JC(0) <= EN_A;
    JC(4) <= EN_B;
    
    Hardware_Control_1: PWM_Module PORT MAP(Clk => clk, PCM => PWMMotor1, PWM_pos => JC(6), PWM_neg => JC(5));
    
    Hardware_Control_2: PWM_Module PORT MAP(Clk => clk, PCM => PWMMotor2, PWM_pos => JC(2), PWM_neg => JC(1));
    
    --Use signals from motor 2 in this
    Hardware_Feedback_1: Encoder PORT MAP(Clk => clk, A => Ax1, B => Bx1, RESET_COUNT => busy_MISO, angle => AngleMotor1); 
    
    --Use signals from motor 2 in this
    Hardware_Feedback_2: Encoder PORT MAP(Clk => clk, A => Ax2, B => Bx2, RESET_COUNT => busy_MISO, angle => AngleMotor2);

    encoder_filter_1 : Encoder_filter
    port map (Clk => Clk,
              A   => JB(6),
              B   => JB(2),
              Ax  => Ax1,
              Bx  => Bx1);

    encoder_filter_2 : Encoder_filter
    port map (Clk => Clk,
              A   => JB(7),
              B   => JB(3),
              Ax  => Ax2,
              Bx  => Bx2);

    Interface_Analysis : SPI_Analysis
    port map (Clk => clk,
              data_received => data_receive,
              enable_MISO => busy_MOSI,
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
               
end Behavioral;
