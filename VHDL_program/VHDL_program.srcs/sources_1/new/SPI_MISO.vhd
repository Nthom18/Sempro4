----------------------------------------------------------------------------------
-- University of Southern Denmark
-- RB-PRO4 - Group 4 
-- Semesterproject in control and regulation of robotic systems
-- Module: SPI_MISO
-- Create Date: 03/28/2020 03:08:09 PM
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY SPI_MISO IS
    GENERIC (data_length : INTEGER := 16);
    PORT (sck  : IN STD_LOGIC;
          ss   : IN STD_LOGIC;
          data : IN STD_LOGIC_VECTOR (data_length-1 downto 0);
          miso : OUT STD_LOGIC := '0';
          busy : OUT STD_LOGIC := '0');
END SPI_MISO;

ARCHITECTURE Behavioral of SPI_MISO IS
    SIGNAL data_out : STD_LOGIC_VECTOR (data_length - 1 downto 0);
BEGIN
    miso <= data_out(data_length-1) when ss = '0';  --If slave select is low, put first bit onto bus
    PROCESS(sck, data, ss)
    BEGIN
        IF falling_edge(sck) THEN                   --At every falling edge of SCK while SS is low, shift lower bits up
            IF ss = '0' THEN
                busy <= '1';                        -- Indicate that transfer ongoing
                data_out <= data_out(data_length - 2 downto 0) & '0';
            END IF;
        END IF;
        IF ss = '1' THEN                            -- If SS goes high, or data changes, load most recent data into register.
            busy <= '0';                            -- Indicate transfer done
            data_out <= data;
        END IF;
    END PROCESS; 
    
END Behavioral;
