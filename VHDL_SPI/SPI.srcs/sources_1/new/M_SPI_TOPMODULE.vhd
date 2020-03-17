LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY M_SPI_TOPMODULE IS
    GENERIC(data_length : INTEGER := 8);
    PORT(clk  : IN STD_LOGIC;
         send : IN STD_LOGIC;
         get  : IN STD_LOGIC;
         data_send : IN STD_LOGIC_VECTOR (data_length-1 downto 0);
         data_receive : OUT STD_LOGIC_VECTOR (data_length-1 downto 0);
         miso : IN STD_LOGIC;
         mosi : OUT STD_LOGIC;
         ss   : OUT STD_LOGIC;
         sck  : OUT STD_LOGIC;
         busy : OUT STD_LOGIC;
         ready : OUT STD_LOGIC);
END M_SPI_TOPMODULE;

ARCHITECTURE Behavioral of M_SPI_TOPMODULE IS

    component M_SPI_MOSI
        generic (data_length : INTEGER := data_length;
                 CLK_SCL     : INTEGER := 24);
        port (clk  : in std_logic;
              data : in std_logic_vector (data_length-1 downto 0);
              send : in std_logic;
              sck  : out std_logic;
              ss   : out std_logic;
              mosi : out std_logic;
              busy : out std_logic);
    end component;
    
    signal sck_MOSI  : STD_LOGIC;
    signal ss_MOSI   : STD_LOGIC;
    signal busy_MOSI : std_logic;
    
    component M_SPI_MISO
        generic (data_length : INTEGER := data_length;
                 CLK_SCL     : INTEGER := 24);
        port (clk   : in std_logic;
              miso  : in std_logic;
              get   : in std_logic;
              data  : out std_logic_vector (data_length-1 downto 0);
              sck   : out std_logic;
              ss    : out std_logic;
              busy  : out std_logic;
              ready : out std_logic);
    end component;

    signal sck_MISO : STD_LOGIC;
    signal ss_MISO  : STD_LOGIC;
    signal busy_MISO : std_logic;

BEGIN

    U1 : M_SPI_MOSI
    port map (clk  => clk,
              data => data_send,
              send => send,
              sck  => sck_MOSI,
              ss   => ss_MOSI,
              mosi => mosi,
              busy => busy_MOSI);

    U2 : M_SPI_MISO
    port map (clk   => clk,
              miso  => miso,
              get   => get,
              data  => data_receive,
              sck   => sck_MISO,
              ss    => ss_MISO,
              busy  => busy_MISO,
              ready => ready);

    sck <= sck_MOSI OR sck_MISO;    --sck is high if either module is high
    ss <= ss_MOSI AND SS_MISO;      --ss is low if either module is low
    busy <= busy_MOSI OR busy_MISO; --busy is high if either module is high
END Behavioral;
