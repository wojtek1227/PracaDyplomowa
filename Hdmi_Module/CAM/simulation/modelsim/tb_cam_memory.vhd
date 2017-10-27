
-- vhdl test bench template for design  :  cam_memory
-- 
-- simulation tool : modelsim-altera (vhdl)
-- 

library ieee;                                               
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;


use ieee.std_logic_textio.all;
use std.textio.all;

entity tb_cam_memory is
end tb_cam_memory;

architecture tb_architecture of tb_cam_memory is

	component cam_memory
		generic 
		( 
			address_width : integer := 5;
			data_width : integer := 32;
			object_size : integer := 20 
		);
		port
		(
			rst		:	 in std_logic;
			clk		:	 in std_logic;
			ce		:	 in std_logic;
			write_enable		:	 in std_logic;
			write_data		:	 in std_logic_vector(data_width-1 downto 0);
			write_addr		:	 in std_logic_vector(address_width-1 downto 0);
			match_data		:	 in std_logic_vector(data_width-1 downto 0);
			match_hit		:	 out std_logic;
			match_addr		:	 out std_logic_vector(address_width-1 downto 0)
		);
	end component;
	
	-- constants
	constant clk_period : time := 10 ns;
	constant data_width : integer := 32;
	constant address_width : integer := 5;
	
	--stimulus signals
	signal rst : std_logic;
	signal clk : std_logic;
	signal ce : std_logic;
	
	signal write_enable : std_logic;
	signal write_data : std_logic_vector(data_width - 1 downto 0);
	signal write_addr : std_logic_vector(address_width - 1 downto 0);
	
	signal match_data : std_logic_vector(data_width - 1 downto 0);
		
	--observed signals
	signal match_hit : std_logic;
	signal match_addr : std_logic_vector(address_width - 1 downto 0);
	
	--
	signal end_sim: boolean := false;
	
	--procedures
	
	
	
	
begin
		uut : cam_memory
		port map 
		(
			rst => rst,
			clk => clk,
			ce => ce,
			write_enable => write_enable,
			write_data => write_data,
			write_addr => write_addr,
			match_data => match_data,
			match_hit => match_hit,
			match_addr => match_addr
		);
		

	stimulus : process 
	variable l : line;
	begin                                                         
		-- code executes for every event on sensitivity list
		wait for 10 ns;
		rst <= '1';
		ce <= '0';
		write_enable <= '0';
		write_data <= (others => '0');
		write_addr <= (others => '0');
		match_data <= (others => '0');
		wait for 10 ns;
		rst <= '0';
--		match_data <= x"00FF00CC";
		match_data <= x"00100010";
		wait for 10 ns;
		ce <= '1';
		write_addr <= b"00100";
		write_data <= x"00FE00CB";
		write_enable <= '1';
--		wait for 10 ns;
--		write_addr <= b"00001";
--		write_data <= x"00010001";
--		wait for 10 ns;
--		write_addr <= b"00010";
--		write_data <= x"00100010";
--		wait for 10 ns;
--		write_addr <= b"00011";
--		write_data <= x"00110011";

		

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
