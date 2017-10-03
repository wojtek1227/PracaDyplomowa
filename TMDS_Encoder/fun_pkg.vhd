

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;

package fun_pkg is 
	function count_ones(vector : std_logic_vector(7 downto 0)) return integer;
	function count_zeros(vector : std_Logic_vector) return integer;
end;

package body fun_pkg is 

	function count_ones (vector : std_Logic_vector(7 downto 0)) return integer is
		variable cnt : integer := 0;
	begin
		for i in vector'range loop
			if vector(i) = '1' then
				cnt := cnt + 1;
			end if;
		end loop;
		return cnt;
	end function;
	
	function count_zeros (vector : std_Logic_vector) return integer is
		variable cnt : integer;
	begin
		for i in vector'range loop
			if vector(i) = '0' then
				cnt := cnt + 1;
			end if;
		end loop;
		return cnt;
	end function;
	
end package body;