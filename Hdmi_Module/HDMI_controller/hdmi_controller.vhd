
-------------------------------------------------------------------------------
--
-- Title       : hdmi_controller
-- Design      : HDMI_Module
-- Author      : Wojciech Caputa
--
-------------------------------------------------------------------------------
--
-- Description :  HDMI control unit
--						
-------------------------------------------------------------------------------

library ieee;
use ieee.numeric_std.all;
use ieee.std_logic_1164.all;
--use ieee.std_logic_unsigned.all;


entity hdmi_controller is
	generic
	(
		const_h_total : integer := 800; 		--all pixels per line, active and blank
		const_h_active : integer := 640;		--active pixels per line
		const_h_front : integer := 16;		--
		const_h_sync : integer := 96;			--
		const_h_blank : integer := 160;		--
		
		const_v_total : integer := 525;		--n
		const_v_active : integer := 480;
		const_v_front : integer := 10;
		const_v_sync : integer := 2;
		const_v_blank : integer := 45;
		
		active_hsync : std_logic := '0';
		blank_hsync : std_logic := '1';
		active_vsync : std_logic := '0';
		blank_vsync : std_logic := '1';
		
		serializer_clk_delay : integer := 2
	);
	port
	(
		rst : in std_logic;
		clk : in std_logic;
		ce : in std_logic;
		
		out_vde : out std_logic;									--video data enable, required by T.M.D.S encoder
		out_hsync : out std_logic;									--horizontal synchronization
		out_vsync : out std_logic;									--vertical synchronization
		tmds_ce : out std_logic;									--T.M.D.S encoder clock enable
		out_serializer_ce : out std_logic;						--Serializer clock enable
		pixel_position : out std_logic_vector(31 downto 0) --Pixel position format: (y,x) x and y are 16bits long
	);
end hdmi_controller;

architecture behavioral of hdmi_controller is

	signal counterx : unsigned (15 downto 0);
	signal countery : unsigned (15 downto 0);
	
	signal vde : std_logic;
	signal hsync : std_logic;
	signal vsync : std_logic;
	signal serializer_ce : std_ulogic_vector(serializer_clk_delay - 1 downto 0);
	signal pixel_counter : std_logic_vector(31 downto 0);

begin

horizontal_counter : process(rst, clk)
	
	begin
		if rst = '1' then
			counterx <= (others => '0');
		elsif rising_edge(clk) then
			if ce = '1' then
				if counterx = const_h_total - 1 then
					counterx <= (others => '0');
				else
					counterx <= counterx + 1;
				end if;
			end if;
		end if;
	end process;

vertical_counter : process(rst, clk)

	begin 
		if rst = '1' then
			countery <= (others => '0');
		elsif rising_edge(clk) then
			if ce = '1' then
				if counterx = const_h_total - 1 then
					if countery = const_v_total - 1 then
						countery <= (others => '0');
					else
						countery <= countery + 1;
					end if;
				end if;
			end if;
		end if;
	end process;
	
	pixel_position <= std_logic_vector(countery) & std_logic_vector(counterx);
	
controller : process(rst, clk)

	begin
		if rst = '1' then
			vde <= '0';
			pixel_counter <= (others => '0');
			--rst
		elsif rising_edge(clk) then
			if ce = '1' then
			
				if (const_h_blank - 1)  <= counterx and counterx < (const_h_total - 1) and (const_v_blank - 1) < countery then
					vde <= '1';
				else
					vde <= '0';
				end if;
				
				if (const_h_front - 1) <= counterx and counterx < (const_h_front + const_h_sync - 1) then
					hsync <= '1';
				else
					hsync <= '0';
				end if;
				
				if (const_v_front - 1) <= countery and countery < (const_v_front + const_v_sync - 1) then
					vsync <= '1';
				else
					vsync <= '0';
				end if;
			end if;
		end if;
	end process;
	
	out_vde <= vde;
	out_hsync <= hsync;
	out_vsync <= vsync;
	
	--TODO 
	serializer_enable : process(rst, clk)

	begin
		if rst = '1' then
			serializer_ce(serializer_clk_delay - 1 downto 1) <= (others => '0');
			serializer_ce(0) <= '1';
		elsif rising_edge(clk) then
			if ce = '1' then
				serializer_ce <= serializer_ce(serializer_clk_delay - 1 downto 1) & '1';
			end if;
		end if;
	end process;
	
out_serializer_ce <= serializer_ce(serializer_clk_delay - 1);
	
end behavioral;