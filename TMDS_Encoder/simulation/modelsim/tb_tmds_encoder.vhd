
-- vhdl test bench template for design  :  tmds_encoder
-- 
-- simulation tool : modelsim-altera (vhdl)
-- 

library ieee;
library work;                                               
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

use ieee.std_logic_textio.all;
use std.textio.all;


use work.fun_pkg.all;

entity tb_tmds_encoder is
end tb_tmds_encoder;

architecture tb_architecture of tb_tmds_encoder is

	component tmds_encoder
		port 
		(
			rst : in std_logic;
			clk : in std_logic;
			ce :in std_logic;
			de : in std_logic;
			c1c0 : in std_logic_vector(1 downto 0);
			data_in : in std_logic_vector(7 downto 0);
			data_out : out std_logic_vector(9 downto 0)
		);
	end component;
	
	-- constants and simulation signals
	constant clk_period : time := 20 ns;
	constant number_of_test : integer := 5000000;
	signal end_sim: boolean := false;
	
	--stimulus signals
	signal rst : std_logic;
	signal clk : std_logic;
	signal ce : std_logic;
	signal de : std_logic;
	signal c1c0 : std_logic_vector(1 downto 0);
	signal data_in : std_logic_vector(7 downto 0);
	
	--observed signals
	signal data_out : std_logic_vector(9 downto 0);
	
begin
		uut : tmds_encoder
		port map (
			rst => rst,
			clk => clk,
			ce => ce,
			de => de,
			c1c0 => c1c0,
			data_in => data_in,
			data_out => data_out
		);
		

	stimulus : process
		variable my_line : line;
		variable tested_data : std_logic_vector(7 downto 0);
		variable passed_tests : integer := 0;
	begin                                                         
		-- code executes for every event on sensitivity list
		tested_data := B"0000_0000";
		c1c0 <= "00";
		de <= '0';
		rst <= '1';
		wait for 10 ns;
		rst <= '0';
		wait for 10 ns;
		ce <= '1';
		wait for 15 ns;
		data_in <= B"0111_1101";
		wait for 15 ns;
		de <= '1';
		
		wait for 15 ns;
		for i in 0 to number_of_test - 1 loop
			data_in <= tested_data;
			wait for clk_period / 2;
			if check_encoding(data_in,data_out) then
				passed_tests := passed_tests + 1;
				write(my_line, string'("Passed: "));
				write(my_line, tested_data);
				write(my_line, string'("@ "));
				write(my_line, now);
				writeline(output, my_line);
			else
				write(my_line, string'("Failed: "));
				write(my_line, tested_data);
				write(my_line, string'("@ "));
				write(my_line,now);
				writeline(output, my_line);
			end if;
			
			wait for clk_period / 2;
			tested_data := tested_data + "1";
		end loop;

		writeline(output, my_line);
		write(my_line, string'("Passed "));
		write(my_line, passed_tests);
		write(my_line, string'(" tests out of "));
		write(my_line, number_of_test);
		writeline(output, my_line);
		
		wait for 500 ns;
		end_sim <= true;
	wait;                                                        
	end process stimulus; 

	clk_process : process
	begin
		if end_sim = false then
			clk <= '0';
			wait for clk_period/2;
		else
			wait;
		end if;
		if end_sim = false then
			clk <= '1';
			wait for clk_period/2;
		else
			wait;
		end if;
	end process;
	
end tb_architecture;
