library ieee;                                               
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all; 

use ieee.std_logic_textio.all;
use std.textio.all;

entity tb_hdmi_controller is
end tb_hdmi_controller;

architecture tb_architecture of tb_hdmi_controller is

	component hdmi_controller
		port 
		(
			rst : in std_logic;
			clk : in std_logic;
			ce : in std_logic;
			
			out_vde : out std_logic;
			out_hsync : out std_logic;
			out_vsync : out std_logic;
			tmds_ce : out std_logic;
			vram_s1_address : out std_logic_vector(50 downto 0)
		);
	end component;
	
	-- constants
	constant clk_period : time := 10 ns;
	constant frames : integer := 5;
	
	--stimulus signals
	signal rst : std_logic;
	signal clk : std_logic;
	signal ce : std_logic;
	
	--observed signals
	signal out_vde : std_logic;
	signal out_hsync : std_logic;
	signal out_vsync : std_logic;
	signal tmds_ce : std_logic;
	signal vram_s1_address : std_logic_vector(50 downto 0);
	
	--
	
	signal vram_s1_readdata : std_logic_vector(7 downto 0);
	--
	signal end_sim: boolean := false;
	
	--procedures
	
	
	
	
begin
		uut : hdmi_controller
		port map 
		(
			rst => rst,
			clk => clk,
			ce => ce,
			out_vde => out_vde,
			out_hsync => out_hsync,
			out_vsync => out_vsync,
			tmds_ce => tmds_ce,
			vram_s1_address => vram_s1_address	
		);
			

	stimulus : process 

		procedure test_reset is
		begin
			wait for 10 ns;
			rst <= '1';
			wait for 10 ns;
			rst <= '0';
		end test_reset;
		variable l :line;
	begin                                                         
		-- code executes for every event on sensitivity list
		test_reset;
		ce <= '1';
		wait for 800*525*frames*clk_period;
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
