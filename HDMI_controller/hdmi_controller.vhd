library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity hdmi_controller is
	generic
	(
		h_active : integer := 640;
		h_total : integer := 800;
		h_front : integer := 16;
		h_sync : integer := 96;
		h_blank : integer := 160;
		
		v_active : integer := 480;
		v_total : integer := 525;
		v_front : integer := 10;
		v_sync : integer := 2;
		v_blank : integer := 45;
		
		serializer_clk_delay : integer := 2
	);
	port
	(
		rst : in std_logic;
		clk : in std_logic;
		ce : in std_logic;
		
		vde : out std_logic;
		hsync : out std_logic;
		vsync : out std_logic;
		tmds_ce : out std_logic;
		out_serializer_ce : out std_logic;
		pixel_position : out std_logic_vector(31 downto 0)
	);
end hdmi_controller;

architecture behavioral of hdmi_controller is

	signal counterx : integer range 0 to h_total - 1;
	signal countery : integer range 0 to v_total - 1;
	signal serial_ce : std_ulogic_vector(serializer_clk_delay - 1 downto 0);

begin

h_counter : process(rst, clk)
	
	begin
		if rst = '1' then
			counterx <= 0;
		elsif rising_edge(clk) then
			if ce = '1' then
				if counterx = h_total - 1 then
					counterx <= 0;
				else
					counterx <= counterx + 1;
				end if;
			end if;
		end if;
	end process;

v_counter : process(rst, clk)

	begin 
		if rst = '1' then
			countery <= 0;
		elsif rising_edge(clk) then
			if ce = '1' then
				if counterx = h_total - 1 then
					if countery = v_total - 1 then
						countery <= 0;
					else
						countery <= countery + 1;
					end if;
				end if;
			end if;
		end if;
	end process;

serializer_enable : process(rst, clk)

	begin
		if rst = '1' then
			serial_ce(serializer_clk_delay - 1 downto 1) <= (others => '0');
			serial_ce(0) <= '1';
		elsif rising_edge(clk) then
			if ce = '1' then
				serial_ce <= serial_ce(serializer_clk_delay - 1 downto 1) & '1';
			end if;
		end if;
	end process;
	
out_serializer_ce <= serial_ce(serializer_clk_delay - 1);
	
controller : process(rst, clk)

	begin
		if rst = '1' then
			--rst
		elsif rising_edge(clk) then
			if ce = '1' then
				
			end if;
		end if;
	end process;
	
end behavioral;