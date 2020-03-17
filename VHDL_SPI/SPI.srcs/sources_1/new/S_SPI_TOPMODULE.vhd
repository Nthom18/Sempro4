--This module combines the S_SPI_MOSI and S_SPI_MISO modules into a complete Slave SPI module.
-- The inputs for this module should be the slave clock (SCK), slave select (SS), Master-out-Slave-in (MOSI) and vector of data to be sent (data_send).
-- The outputs from this module is Master-in-slave out (MISO), the received data (data_receive), flag indicating transfer is ongoing (busy), and flag indicating new data is available (ready).


LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY S_SPI_TOPMODULE IS
    GENERIC(data_length : INTEGER := 8);
    PORT(sck : IN STD_LOGIC;
         ss : IN STD_LOGIC;
         data_receive : OUT STD_LOGIC_VECTOR (data_length-1 downto 0);
         data_send : IN STD_LOGIC_VECTOR (data_length-1 downto 0);
         miso : OUT STD_LOGIC;
         mosi : IN STD_LOGIC;
         busy : OUT STD_LOGIC;
         ready : OUT STD_LOGIC);
END S_SPI_TOPMODULE;

ARCHITECTURE Behavioral OF S_SPI_TOPMODULE IS

component S_SPI_MOSI
        GENERIC(data_length : INTEGER := data_length);
        port (sck   : in std_logic;
              ss    : in std_logic;
              mosi  : in std_logic;
              data  : out std_logic_vector (data_length-1 downto 0);
              busy  : out std_logic;
              ready : out std_logic);
    end component;

    signal busy_MOSI  : std_logic;
    
    component S_SPI_MISO
        port (sck  : in std_logic;
              ss   : in std_logic;
              data : in std_logic_vector (data_length-1 downto 0);
              miso : out std_logic;
              busy : out std_logic);
    end component;

    signal busy_MISO : std_logic;
  

BEGIN

    U1 : S_SPI_MOSI
    port map (sck   => sck,
              ss    => ss,
              mosi  => mosi,
              data  => data_receive,
              busy  => busy_MOSI,
              ready => ready);
 
    U2 : S_SPI_MISO
    port map (sck  => sck,
              ss   => ss,
              data => data_send,
              miso => miso,
              busy => busy_MISO);

    busy <= busy_MOSI OR busy_MISO;     ----busy is high if either module is transmitting. Should always be the same though
END Behavioral;
