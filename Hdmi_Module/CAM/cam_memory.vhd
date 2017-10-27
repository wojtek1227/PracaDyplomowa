-------------------------------------------------------------------------------
--
-- Title       : 
-- Design      : 
-- Author      : 
--
-------------------------------------------------------------------------------
--
-- Description : 
--
-------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity cam_memory is 

	generic
	(
		address_width : integer := 5;
		data_width : integer := 32;
		object_size : integer := 20
	);
	port
	(
		rst : in std_logic;
		clk : in std_logic;
		ce : in std_logic;
		
		write_enable: in std_logic;
		write_data : in std_logic_vector(data_width - 1 downto 0);
		write_addr : in std_logic_vector(address_width - 1 downto 0);
		
		match_data : in std_logic_vector(data_width - 1 downto 0);
		match_hit : out std_logic;
		match_addr : out std_logic_vector(address_width - 1 downto 0)
		
	);
end cam_memory;

architecture behavioral of cam_memory is

	type memory is array (2**address_width - 1 downto 0) of unsigned(data_width - 1 downto 0);
	signal content_memory : memory;
	
	begin
	
	process(rst, clk)
	
		variable w_addr : natural range 2**address_width - 1 downto 0;
		variable hit_addr : integer := 0;
		
	begin
		if rst = '1' then
			content_memory <= (others => (others => '0'));
			match_addr <= (others => '0');
			match_hit <= '0';
		elsif rising_edge(clk) then
			if ce = '1' then
				if write_enable = '1' then
					w_addr := to_integer(unsigned(write_addr));
					content_memory(w_addr) <= unsigned(write_data);
				end if;
				
				for i in content_memory'range loop
					if content_memory(i)(31 downto 16) <= unsigned(match_data(31 downto 16))
						and content_memory(i)(15 downto 0) <= unsigned(match_data(15 downto 0)) 
						and (content_memory(i)(31 downto 16) + to_unsigned(object_size,16)) >= unsigned(match_data(31 downto 0))
						and (content_memory(i)(15 downto 0) + to_unsigned(object_size,16)) >= unsigned(match_data(15 downto 0)) then
						hit_addr := i;
						match_hit <= '1';
						match_addr <= std_logic_vector(to_unsigned(hit_addr,address_width));
					end if;
				end loop;
					
			end if;
		end if;
	end process;
	
end behavioral;
	