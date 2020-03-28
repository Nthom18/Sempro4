-- Testbench automatically generated online
-- at http://vhdl.lapinoo.net
-- Generation date : 16.3.2020 07:54:44 GMT

library ieee;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity tb_PWM_v1 is
       generic (N: natural := 7; --Should be choosen to fit the applicaiton
             MAX: natural := 128); --Should be choosen to fit the application
end tb_PWM_v1;


architecture tb of tb_PWM_v1 is

    component PWM_v1
        port (Clk   : in std_logic;
              Scale : in std_logic;
              PCM   : in std_logic_vector (n-1 downto 0);
              PWM   : out std_logic);
    end component;

    signal Clk   : std_logic;
    signal Scale : std_logic;
    signal PCM   : std_logic_vector (n-1 downto 0);
    signal PWM   : std_logic;

    constant TbPeriod : time := 1000 ns; -- EDIT Put right period here
    signal TbClock : std_logic := '0';
    signal TbSimEnded : std_logic := '0';

begin

    dut : PWM_v1
    port map (Clk   => Clk,
              Scale => Scale,
              PCM   => PCM,
              PWM   => PWM);

    -- Clock generation
    TbClock <= not TbClock after TbPeriod/2 when TbSimEnded /= '1' else '0';

    -- EDIT: Check that Clk is really your main clock signal
    Clk <= TbClock;

    stimuli : process
    begin
        -- EDIT Adapt initialization as needed
        Scale <= '0';
        PCM <= (others => '0');

        -- EDIT Add stimuli here
        wait for 100 * TbPeriod;

        -- Stop the clock and hence terminate the simulation
        TbSimEnded <= '1';
        wait;
    end process;

end tb;

-- Configuration block below is required by some simulators. Usually no need to edit.

configuration cfg_tb_PWM_v1 of tb_PWM_v1 is
    for tb
    end for;
end cfg_tb_PWM_v1;