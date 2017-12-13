-------------------------------------------------------------------------------
--
-- Title       : tmds_encoder
-- Design      : HDMI_Module
-- Author      : Wojciech Caputa
--
-------------------------------------------------------------------------------
--
-- Description : T.M.D.S encoder based on alghorithm specified by Figure 3-5 
-- 				  from DVI spec - Revision 1.0
--
-- Signal names are  the same as in DVI spec, with few exceptions
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

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
		data_out	:	out std_logic_vector(9 downto 0)
	);
end tmds_encoder;

architecture behavioral of tmds_encoder is

	signal cnt : integer range -16 to 15 := 0;		--used to keep track of the data stream disparity
	signal q_m : std_logic_vector(8 downto 0);		--transition minimized data 9 bit code
	signal q_out : std_logic_vector(9 downto 0);  	--finished tmds 10-bit code word
	
begin	
	
	
	process(data_in, q_m)
	begin
		if (count_ones(data_in) > 4) or (count_ones(data_in) = 4 and (data_in(0) = '0')) then
			for i in 0 to 8 loop
				if i = 0 then
					q_m(i) <= data_in(i);
				elsif i = 8 then
					q_m(i) <= '0';
				else
					q_m(i) <= q_m(i-1) xnor data_in(i);
				end if;
			end loop;
		else
			for i in 0 to 8 loop
				if i = 0 then
					q_m(i) <= data_in(i);
				elsif i = 8 then
					q_m(i) <= '1';
				else
					q_m(i) <= q_m(i-1) xor data_in(i);
				end if;
			end loop;
		end if;
	end process;

	process(rst, clk)
	
	begin
		if rst = '1' then
			q_out <= (others => '0');
		elsif rising_edge(clk) then
			if ce = '1' then
				if de = '1' then
					--video data period
					if cnt = 0 or count_ones(q_m(7 downto 0)) = 4 then
						q_out(9) <= not q_m(8);
						q_out(8) <= q_m(8);
						if q_m(8) = '1' then
							q_out(7 downto 0) <= q_m(7 downto 0);
							cnt <= cnt + 2 * count_ones(q_m(7 downto 0)) - 8 ;
						else
							q_out(7 downto 0) <= not q_m(7 downto 0);
							cnt <= cnt + 8 - 2 * count_ones(q_m(7 downto 0));
						end if;
					else 
						if (cnt > 0 and (count_ones(q_m(7 downto 0)) > 4)) or (cnt < 0 and (count_ones(q_m(7 downto 0)) < 4)) then
							q_out <= '1' & q_m(8) & not q_m(7 downto 0);
							cnt <= cnt + 2 * to_integer(unsigned'("" & q_m(8))) + 8 - 2*count_ones(q_m(7 downto 0));
						else
							q_out <= '0' & q_m(8) & q_m(7 downto 0);
							cnt <= cnt - 2 * to_integer(unsigned'("" & not q_m(8))) + 2* count_ones(q_m(7 downto 0)) - 8;
						end if;
					end if;
				else
					--control period
					cnt <= 0;
					case c1c0 is
						when "00" => q_out <= B"11_0101_0100";
						when "01" => q_out <= B"00_1010_1011";
						when "10" => q_out <= B"01_0101_0100";
						when "11" => q_out <= B"10_1010_1011";
						when others => null;
					end case;
				end if;
			end if;
		end if;
	
	end process;
	
	data_out <=  q_out;
	
end behavioral;
