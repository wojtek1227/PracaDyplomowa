library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


entity addres_filter is
	generic
	(
		constant int_vram_addr_width	: integer := 19;
		constant addres_to_filter	: integer := 294400
	);
	port
	(
		data_in	: in std_logic;
		addres	: in std_logic_vector(int_vram_addr_width - 1 downto 0);
		data_out	: out std_logic
	);
	
end addres_filter;

architecture behavioral of addres_filter is
	
begin

	data_out <= data_in when (to_integer(unsigned(addres)) < addres_to_filter) else '0';

end behavioral;