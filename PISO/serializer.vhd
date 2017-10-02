-------------------------------------------------------------------------------
--
-- Title       : Serializer
-- Design      : Parallel in Serial out
-- Author      : Wojciech Caputa
--
-------------------------------------------------------------------------------
--
-- Description : 
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity serializer is
	generic
	(
		data_width : integer := 10
	);
	port
	(		
		rst	:	in std_logic;
		clk	:	in std_logic;
		ce		:	in std_logic;
		load	:	in std_logic;
		data_in	: 	in std_logic_vector(data_width - 1 downto 0);
		data_out_p	: 	out std_logic;
		data_out_n	: 	out std_logic
	);
end serializer;

architecture behavioral of serializer is

	signal data : std_logic_vector(data_width - 1 downto 0);

begin

	process(rst, clk)
	begin
		if rst = '1' then
			data <= (others => '0');
		elsif rising_edge(clk) then
			if ce = '1' then
				if load = '1' then
					data <= data_in;
				else
					data <= '0' & data(data_width - 1 downto 1);
				end if;
			end if;
		end if;
	end process;

	data_out_p <= data(0);
	data_out_n <= not data(0);


end behavioral;