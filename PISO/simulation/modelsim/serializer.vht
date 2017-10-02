library ieee;                                               
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all; 

use ieee.std_logic_textio.all;
use std.textio.all;

entity TB_serializer is
end TB_serializer;

architecture TB_serializer_arch of TB_serializer is

	component serializer
		port 
		(
			rst : in std_logic;
			clk : in std_logic;
			ce	: in std_logic;
			load : in std_logic; 
			data_in : in std_logic_vector(9 downto 0);
			data_out_n : out std_logic;
			data_out_p : out std_logic			
		);
	end component;
	
	-- constants
	constant clk_period : time := 20 ns;
	
	--stimulus signals
	signal rst : std_logic := '0';	
	signal clk : std_logic := '1';
	signal ce : std_logic := '0';
	signal load : std_logic := '0';
	signal data_in : std_logic_vector(9 downto 0) := B"00_0000_0000";
	
	--observed signals
	signal data_out_n : std_logic;
	signal data_out_p : std_logic;
	
	--
	signal end_sim: boolean := false;
	
	--procedures
	
	
	
	
begin
		uut : serializer
		port map 
		(
			ce => ce,
			clk => clk,
			data_in => data_in,
			data_out_n => data_out_n,
			data_out_p => data_out_p,
			load => load,
			rst => rst
		);
		

	stimulus : process 

		procedure test is
			variable l : line;
		begin
			writeline(output, l);
			write(l, string'("*********<Test>*********"));
			writeline(output, l);
			write(l, string'("Test started at: "));
			write(l, now);
			writeline(output, l);
			
			wait for clk_period;
			write(l, string'("Testing if reset works asynchronously, time: "));
			write(l, now);
			writeline(output, l);
			wait until rising_edge(clk);
			ce <= '0';
			wait for clk_period/2;
			rst <= '1';
			wait for clk_period/4;
			rst <= '0';
			
			wait for 2*clk_period;
			write(l, string'("Testing if reset blocks load operation, time: "));
			write(l, now);
			writeline(output, l);
			wait until falling_edge(clk);
			ce <= '1';
			load <= '1';
			data_in <= B"11_1111_1111";
			wait for clk_period/4;
			rst <= '1';
			
			wait for clk_period;
			wait until falling_edge(clk);
			write(l, string'("Testing load operation and shifting bits, time: "));
			write(l, now);
			writeline(output, l);
			rst <= '0';
			ce <= '1';
			load <= '1';
			data_in <= B"11_0101_0101";
			wait until falling_edge(clk);
			load <= '0';
			wait for 10*clk_period;
			write(l, string'("End of shifting, time: "));
			write(l, now);
			writeline(output, l);
			
			wait for 2*clk_period;
			write(l, string'("Testing if reset works after load operation, time: "));
			write(l, now);
			writeline(output, l);
			wait for clk_period/4;
			rst <= '0';
				
			writeline(output, l);
			write(l, string'("*********<Test_reset>*********"));
			writeline(output, l);
			write(l, string'("Test ended at: "));
			write(l, now);
			writeline(output, l);
			end test;
		
		variable l :line;
	begin                                                         
		-- code executes for every event on sensitivity list
		test;
		wait for 500 ns;
		end_sim <= true;
	wait;                                                        
	end process stimulus; 

	clk_process : process
	begin
		if end_sim = false then
			clk <= not clk;
			wait for clk_period/2;
		else
			wait;
		end if;
	end process;
	
end TB_serializer_arch;
