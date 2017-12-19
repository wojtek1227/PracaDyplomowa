library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity data_mux is
	port
	(
		data_in	: in std_logic_vector(31 downto 0);
		sel		: in std_logic_vector(4 downto 0);
		result	: out std_logic 
	);
end data_mux;

architecture arch of data_mux is
begin
	result <= data_in(to_integer(unsigned(sel)));
end arch;