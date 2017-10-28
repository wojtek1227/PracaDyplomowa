

library ieee;
use ieee.std_logic_1164.all;

use ieee.std_logic_textio.all;
use std.textio.all;

package fun_pkg is 
	function count_ones(vector : std_logic_vector(7 downto 0)) return integer;
	function check_encoding(
		data : std_Logic_vector(7 downto 0);
		encoded_data: std_Logic_vector(9 downto 0))
		return boolean;
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
	
	function check_encoding(
		data : std_Logic_vector(7 downto 0);
		encoded_data: std_Logic_vector(9 downto 0))
		return boolean is
		
		variable encoded_data_buf : std_Logic_vector(7 downto 0);
		variable decoded_data : std_Logic_vector(7 downto 0);
		variable even : boolean;
	begin

		encoded_data_buf(7 downto 0) := encoded_data(7 downto 0);
		
		if encoded_data(9) = '1' then
			encoded_data_buf(7 downto 0) := not (encoded_data_buf(7 downto 0));
		end if;
		
		if encoded_data(8) = '1' then
			for i in data'range loop
				if i = 0 then
					decoded_data(i) := encoded_data_buf(i);
				else
					decoded_data(i) := encoded_data_buf(i) xor encoded_data_buf(i - 1);
				end if;
			end loop;
		else
			for i in data'range loop
				if i = 0 then
					decoded_data(i) := encoded_data_buf(i);
				else
					decoded_data(i) := encoded_data_buf(i) xnor encoded_data_buf(i - 1);
				end if;
			end loop;
		end if;
		
		if decoded_data = data then
			even := true;
		else
			even := false;
		end if;
		return even;
	end function;
	
end package body;