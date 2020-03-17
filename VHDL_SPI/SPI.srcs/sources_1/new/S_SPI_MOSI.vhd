LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;


ENTITY S_SPI_MOSI IS
    GENERIC(data_length : INTEGER := 8);
    PORT(sck   : IN STD_LOGIC;
         ss    : IN STD_LOGIC;
         mosi  : IN STD_LOGIC;
         data  : OUT STD_LOGIC_VECTOR (data_length-1 downto 0) := (OTHERS => '0');
         busy  : OUT STD_LOGIC := '0';
         ready : OUT STD_LOGIC:= '0');
END S_SPI_MOSI;

ARCHITECTURE Behavioral of S_SPI_MOSI IS

SIGNAL data_temp : STD_LOGIC_VECTOR (data_length-1 downto 0) := (others => '0');
SIGNAL index : INTEGER := data_length - 1;

BEGIN
  
    
    PROCESS(sck, ss)
    BEGIN
        IF Rising_edge(sck) THEN                --If rising edge of clock, read bit from mosi into data_temp at index, and update index
            IF ss = '0' THEN
                data_temp(index) <= mosi;
                index <= index - 1;
                ready <= '0';                   --Indicate most recent data is about to be updated
                busy <= '1';                    --Indicate ongoing data transfer
            END IF; 
        END IF;
        
        IF index < 0 THEN                       --If full transfer complete, output data and reset index
            data <= data_temp;
            index <= data_length - 1;
            busy <= '0';                        --Indicate data transfer complete
            ready <= '1';                       --Indicate new data available
        END IF;
            
        IF ss = '1' THEN                        --Off-sync reset
            index <= data_length - 1;
        END IF;
    END PROCESS;

END Behavioral;
