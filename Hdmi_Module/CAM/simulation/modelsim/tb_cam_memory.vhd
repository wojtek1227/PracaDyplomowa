
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
			ADDR_WIDTH : integer := 5;
			data_width : integer := 32;
			object_size : integer := 20 
		);
		port
		(
			rst		:	 in std_logic;
			clk		:	 in std_logic;
			ce		:	 in std_logic;
			
			we		:	 in std_logic;
			wdata		:	 in unsigned(data_width-1 downto 0);
			waddr		:	 in natural range 0 to 2**ADDR_WIDTH - 1;
			
			match_data		:	 in unsigned(data_width-1 downto 0);
			match_addr		:	 out natural range 0 to 2**ADDR_WIDTH - 1;
			match_hit		:	 out std_logic
		);
	end component;
	
	-- constants
	constant clk_period : time := 10 ns;
	constant data_width : integer := 32;
	constant ADDR_WIDTH : integer := 5;
	
	--stimulus signals
	signal rst : std_logic;
	signal clk : std_logic;
	signal ce : std_logic;
	
	signal we : std_logic;
	signal wdata : unsigned(data_width - 1 downto 0);
	signal waddr : natural range 0 to 2**ADDR_WIDTH - 1;
	
	signal match_data : unsigned(data_width - 1 downto 0);
		
	--observed signals
	signal match_hit : std_logic;
	signal match_addr : natural range 0 to 2**ADDR_WIDTH - 1;
	
	--
	signal testing : unsigned(3 downto 0) := "1100";
	signal end_sim: boolean := false;
	
	--procedures
	
	
	
	
begin
		uut : cam_memory
		port map 
		(
			rst => rst,
			clk => clk,
			ce => ce,
			we => we,
			wdata => wdata,
			waddr => waddr,
			match_data => match_data,
			match_hit => match_hit,
			match_addr => match_addr
		);
		

	stimulus : process 
	begin         
		wait for 10 ns;
		testing <= testing + 2;
		-- code executes for every event on sensitivity list
		wait for 10 ns;
		rst <= '1';
		ce <= '0';
		we <= '0';
		wdata <= (others => '0');
		waddr <= 0;
		match_data <= (others => '0');
		wait for 10 ns;
		rst <= '0';
--		match_data <= x"00FF00CC";
--		match_data <= x"00100010";
		wait for 10 ns;
		ce <= '1';
		waddr <= 0;
		wdata <= x"00FE00CB";
		we <= '1';
		wait for 10 ns;
		we <= '0';
		match_data <= x"00000000";
		wait for 20 ns;
		match_data <= to_unsigned(31, data_width);
		wait for 20 ns;
		match_data <= to_unsigned(20, data_width);
		wait for 20 ns;
		match_data <= x"00FF00CC";
		wait for 20 ns;
		match_data <= x"00FE00CB";
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
