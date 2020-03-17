
-- Testbench automatically generated online
-- at http://vhdl.lapinoo.net
-- Generation date : 16.3.2020 09:14:59 GMT

library ieee;
use ieee.std_logic_1164.all;

entity tb_S_SPI_MOSI is
GENERIC(data_length : INTEGER := 8);
end tb_S_SPI_MOSI;

architecture tb of tb_S_SPI_MOSI is
    

    component S_SPI_MOSI
        port (sck   : in std_logic;
              ss    : in std_logic;
              mosi  : in std_logic;
              data  : out std_logic_vector (data_length-1 downto 0) := (others => '0');
              busy  : out std_logic;
              ready : out std_logic);
    end component;

    signal sck   : std_logic;
    signal ss    : std_logic;
    signal mosi  : std_logic;
    signal data  : std_logic_vector (data_length-1 downto 0) := (others => '0');
    signal busy  : std_logic;
    signal ready : std_logic;

    constant TbPeriod : time := 1000 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin

    dut : S_SPI_MOSI
    port map (sck   => sck,
              ss    => ss,
              mosi  => mosi,
              data  => data,
              busy  => busy,
              ready => ready);

    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that sck is really your main clock signal
    sck <= TbClock;

    stimuli : process
    begin
        -- EDIT Adapt initialization as needed
        ss <= '1';
        mosi <= '1';
        
        mosi <= not mosi after 1000 ns;
        ss <= '0' after 1000 ns, '1' after 9000 ns, '0' after 11000 ns, '1' after 19000 ns; 

        -- EDIT Add stimuli here
        wait for 100 * TbPeriod;

        -- Stop the clock and hence terminate the simulation
        TbSimEnded <= '1';
        wait;
    end process;

end tb;