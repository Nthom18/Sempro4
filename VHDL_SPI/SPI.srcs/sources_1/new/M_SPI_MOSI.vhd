LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY M_SPI_MOSI IS
    GENERIC (data_length : INTEGER := 8;
             CLK_SCL     : INTEGER := 24); 
    PORT ( clk  : IN STD_LOGIC;
           data : IN STD_LOGIC_VECTOR (data_length-1 downto 0);
           send : IN STD_LOGIC;
           sck  : OUT STD_LOGIC := '0';
           ss   : OUT STD_LOGIC := '1';
           mosi : OUT STD_LOGIC;
           busy : OUT STD_LOGIC := '0');
END M_SPI_MOSI;

ARCHITECTURE Behavioral OF M_SPI_MOSI IS

TYPE state_type is (RDY, START, TRANSMIT, STOP);

SIGNAL state  : state_type := RDY;
SIGNAL clkdiv : INTEGER := 0;
SIGNAL index  : INTEGER := 0;

SIGNAL sck_temp : STD_LOGIC := '0';

BEGIN

    sck <= sck_temp;
    PROCESS(clk)
    BEGIN
        IF Rising_edge(clk) THEN
            IF clkdiv = CLK_SCL THEN
                clkdiv <= 0;
                sck_temp <= not sck_temp;
            ELSE
                clkdiv <= clkdiv + 1;
            END IF;
        END IF;
    END PROCESS;

    PROCESS(sck_temp)
    BEGIN
        IF Rising_edge(sck_temp) THEN
            CASE state IS
                WHEN RDY =>
                    IF send = '1' THEN
                        state <= START;
                        busy <= '1';
                        index <= data_length - 1;
                    END IF;
                WHEN START =>
                    ss <= '0';
                    mosi <= data(index);
                    index <= index - 1;
                    state <= TRANSMIT;
                WHEN TRANSMIT =>
                    IF index = 0 THEN
                        state <= STOP;
                    END IF;
                    mosi <= data(index);
                    index <= index - 1;
                WHEN STOP =>
                    busy <= '0';
                    ss <= '1';
                    state <= RDY;
            END CASE;
        END IF;
    END PROCESS;
END Behavioral;
