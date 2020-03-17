LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY M_SPI_MISO IS
    GENERIC(data_length : INTEGER := 8;
            CLK_SCL     : INTEGER := 24);
    PORT(clk   : IN STD_LOGIC;
         miso  : IN STD_LOGIC;
         get   : IN STD_LOGIC;
         data  : OUT STD_LOGIC_VECTOR (data_length-1 downto 0);
         sck   : OUT STD_LOGIC := '0';
         ss    : OUT STD_LOGIC := '1';
         busy  : OUT STD_LOGIC := '0';
         ready : OUT STD_LOGIC := '0');
END M_SPI_MISO;

ARCHITECTURE Behavioral OF M_SPI_MISO IS

TYPE state_type IS (RDY, RECEIVE, STOP);

SIGNAL state : state_type := RDY;
SIGNAL data_temp : STD_LOGIC_VECTOR (data_length-1 downto 0);
SIGNAL clkdiv : INTEGER := 0;
SIGNAL index : INTEGEr := 0;

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
                    IF get = '1' THEN
                        ss <= '0';
                        state <= RECEIVE;
                        busy <= '1';
                        ready <= '0';
                        data_temp <= (others => '0');
                        index <= data_length - 1;
                    END IF;
                WHEN RECEIVE =>
                    IF index = 0 THEN
                        state <= STOP;
                    END IF;
                    data_temp(index) <= miso;
                    index <= index - 1;
                WHEN STOP =>
                    busy <= '0';
                    ready <= '1';
                    ss <= '1';
                    data <= data_temp;
                    state <= RDY;
            END CASE;
        END IF;
    END PROCESS;

END Behavioral;
