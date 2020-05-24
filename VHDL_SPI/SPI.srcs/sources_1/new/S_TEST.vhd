
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;


ENTITY S_TEST IS
    GENERIC(data_length : INTEGER := 16);
    Port (clk : IN STD_LOGIC;
          sck : IN STD_LOGIC; --sck
          ss : IN STD_LOGIC; --ss
          mosi : IN STD_LOGIC; --mosi
          miso: OUT STD_LOGIC; --miso
          led : OUT STD_LOGIC_VECTOR (15 DOWNTO 0);
          sw  : IN STD_LOGIC_VECTOR(15 DOWNTO 0)); 
END S_TEST;

ARCHITECTURE Behavioral OF S_TEST IS
  component S_SPI_TOPMODULE
        port (sck          : in std_logic;
              ss           : in std_logic;
              data_receive : out std_logic_vector (data_length-1 downto 0);
              data_send    : in std_logic_vector (data_length-1 downto 0);
              miso         : out std_logic;
              mosi         : in std_logic;
              busy         : out std_logic;
              ready        : out std_logic);
    end component;

    --signal ss           : std_logic;
    signal data_receive : std_logic_vector (data_length-1 downto 0);
    signal data_send    : std_logic_vector (data_length-1 downto 0);
    --signal miso         : std_logic;
    --signal mosi         : std_logic;
    signal busy         : std_logic;
    signal ready        : std_logic;


BEGIN
   data_send <= sw(15 downto 0);
   
   SLAVE_SPI : S_SPI_TOPMODULE
    port map (sck          => sck,
              ss           => ss,
              data_receive => data_receive,
              data_send    => data_send,
              miso         => miso,
              mosi         => mosi,
              busy         => busy,
              ready        => ready);


    led(15 downto 0) <= data_receive;
    --led(15 downto 8) <= data_send;
    --led(14) <= ready;
    --led(15) <= busy;
END Behavioral;
