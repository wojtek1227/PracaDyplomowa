-------------------------------------------------------------------------------
--
-- Title       : tmds_encoder
-- Design      : T.M.D.S encoder
-- Author      : Wojciech Caputa
--
-------------------------------------------------------------------------------
--
-- Description : 
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

use work.fun_pkg.all;

entity tmds_encoder is
	port
	(
		rst	:	in std_logic;
		clk	: 	in std_logic;
		ce		:	in std_logic;
		de		:	in std_logic;
		c1c0	:	in std_logic_vector(1 downto 0);
		data_in	:	in std_logic_vector(7 downto 0);
		data_out	:	out std_logic_vector(9 downto 0);
		load_to_serial	:	out std_logic
	);
end tmds_encoder;

architecture behavioral of tmds_encoder is

	signal parity : integer range -16 to 15 := 0;
	signal d_int : std_logic_vector(8 downto 0);
	signal d_out_buff : std_logic_vector(9 downto 0);
	
begin	
	
	
	process(data_in, d_int)
	begin
		if (count_ones(data_in) > 4) or (count_ones(data_in) = 4 and (data_in(0) = '0')) then
			for i in 0 to 8 loop
				if i = 0 then
					d_int(i) <= data_in(i);
				elsif i = 8 then
					d_int(i) <= '0';
				else
					d_int(i) <= d_int(i-1) xnor data_in(i);
				end if;
			end loop;
		else
			for i in 0 to 8 loop
				if i = 0 then
					d_int(i) <= data_in(i);
				elsif i = 8 then
					d_int(i) <= '1';
				else
					d_int(i) <= d_int(i-1) xor data_in(i);
				end if;
			end loop;
		end if;
	end process;

	process(rst, clk)
	
	begin
		if rst = '1' then
		--rst
		elsif rising_edge(clk) then
			if ce = '1' then
				if de = '1' then
					--video out
					if parity = 0 or count_ones(d_int(7 downto 0)) = 4 then
						d_out_buff(9) <= not d_int(8);
						d_out_buff(8) <= d_int(8);
						if d_int(8) = '1' then
							d_out_buff(7 downto 0) <= d_int(7 downto 0);
							parity <= parity + 2 * count_ones(d_int(7 downto 0) - 8 );
						else
							d_out_buff(7 downto 0) <= not d_int(7 downto 0);
							parity <= parity + 8 - 2 * count_ones(d_int(7 downto 0));
						end if;
					else 
						if (parity > 0 and (count_ones(d_int(7 downto 0)) > 4)) or (parity < 0 and (count_ones(d_int(7 downto 0)) < 4)) then
							d_out_buff <= '1' & d_int(8) & not d_int(7 downto 0);
							parity <= parity + 2 * d_int(8) + 8 - 2*count_ones(d_int(7 downto 0));
						else
							d_out_buff <= '0' & d_int(8) & d_int(7 downto 0);
							parity <= parity - 2 * (not d_int(8)) + 2* count_ones(d_int(7 downto 0)) - 8;
						end if;
					end if;
				else
					parity <= 0;
					case c1c0 is
						when "00" => d_out_buff <= B"00_1010_1011";
						when "01" => d_out_buff <= B"11_0101_0100";
						when "10" => d_out_buff <= B"00_1010_1010";
						when "11" => d_out_buff <= B"11_0101_0101";
						when others => null;
					end case;
				end if;
			end if;
		end if;
	
	end process;
	
	data_out <=  '0' & d_int;
	
end behavioral;
