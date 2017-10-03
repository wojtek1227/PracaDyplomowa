
-- vhdl test bench template for design  :  tmds_encoder
-- 
-- simulation tool : modelsim-altera (vhdl)
-- 

library ieee;                                               
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all; 

use ieee.std_logic_textio.all;
use std.textio.all;

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
			data_out : out std_logic_vector(9 downto 0);
			load_to_serial : out std_logic
			
		);
	end component;
	
	-- constants
	constant clk_period : time := 10 ns;
	
	--stimulus signals
	signal rst : std_logic;
	signal clk : std_logic;
	signal ce : std_logic;
	signal de : std_logic;
	signal c1c0 : std_logic_vector(1 downto 0);
	signal data_in : std_logic_vector(7 downto 0);
	
	--observed signals
	signal data_out : std_logic_vector(9 downto 0);
	signal load_to_serial : std_logic;
	
	--
	signal end_sim: boolean := false;
	
	--procedures
	
	
	
	
begin
		uut : tmds_encoder
		port map (
			rst => rst,
			clk => clk,
			ce => ce,
			de => de,
			c1c0 => c1c0,
			data_in => data_in,
			data_out => data_out,
			load_to_serial => load_to_serial
		);
		

	stimulus : process 

	begin                                                         
		-- code executes for every event on sensitivity list
		rst <= '1';
		wait for 10 ns;
		rst <= '0';
		wait for 10 ns;
		ce <= '1';
		wait for 10 ns;
		data_in <= B"1011_0101";
		wait for 10 ns;
		data_in <= B"0101_0101";
		wait for 500 ns;
		end_sim <= true;
	wait;                                                        
	end process stimulus; 

	clk_process : process
	begin
		if end_sim = false then
			clk <= '0';
			wait for clk_period;
		else
			wait;
		end if;
		if end_sim = false then
			clk <= '1';
			wait for clk_period;
		else
			wait;
		end if;
	end process;
	
end tb_architecture;
