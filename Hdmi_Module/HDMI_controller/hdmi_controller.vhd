
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


entity hdmi_controller is
	generic
	(
		constant const_h_total 	: integer := 800; 		--all pixels per line, active and blank
		constant const_h_active : integer := 640;		--active pixels per line
		constant const_h_front 	: integer := 16;		--
		constant const_h_sync 	: integer := 96;			--
		constant const_h_back 	: integer := 48;
		constant const_h_blank 	: integer := 160;		--

		constant const_v_total 	: integer := 525;		--n
		constant const_v_active : integer := 480;
		constant const_v_front 	: integer := 10;
		constant const_v_sync 	: integer := 2;
		constant const_v_back 	: integer := 33;
		constant const_v_blank 	: integer := 45;

		constant active_hsync 	: std_logic := '0';
		constant inactive_hsync : std_logic := '1';
		constant active_vsync 	: std_logic := '0';
		constant inactive_vsync : std_logic := '1';
		
		constant vram_addr_width : integer := 19
		
	);
	port
	(
		rst : in std_logic;
		clk : in std_logic;
		ce : in std_logic;
		
		out_vde : out std_logic;									--video data enable, required by T.M.D.S encoder
		out_hsync : out std_logic;									--horizontal synchronization
		out_vsync : out std_logic;									--vertical synchronization
		out_vram_address : out std_logic_vector(vram_addr_width - 1 downto 0) --Pixel position format: (y,x) x and y are 16bits long
	);
end hdmi_controller;

architecture behavioral of hdmi_controller is
	
	signal vde : std_logic;
	signal hsync : std_logic;
	signal vsync : std_logic;
	signal vram_addr : unsigned(vram_addr_width - 1 downto 0) := (others => '0');

begin
	
controller : process(rst, clk)
	
	variable counterx : unsigned (15 downto 0) := (others => '0');
	variable countery : unsigned (15 downto 0) := (others => '0');
	

	begin
		if rst = '1' then
			vde <= '0';
			hsync <= inactive_hsync;
			vsync <= inactive_vsync;
			counterx := (others => '0');
			countery := (others => '0');
			vram_addr <= (others => '0');
			--rst
		elsif rising_edge(clk) then
			if ce = '1' then
			
				if counterx = const_h_total - 1 then
					if countery = const_v_total - 1 then
						countery := (others => '0');
					else
						countery := countery + 1;
					end if;
					counterx := (others => '0');
				else
					counterx := counterx + 1;
				end if;
			
				if 0 <= counterx and counterx < const_h_sync then
					hsync <= active_hsync;
				else
					hsync <= inactive_hsync;
				end if;
			
				if (const_h_sync + const_h_back)  <= counterx and counterx < (const_h_total - const_h_front) and (const_v_blank - 1) < countery then
					vde <= '1';

				else
					vde <= '0';
				end if;
				
				if (const_v_front) <= countery and countery < (const_v_front + const_v_sync) then
					vsync <= active_vsync;
				else
					vsync <= inactive_vsync;
				end if;
				
				if vde = '1' then
					if vram_addr = (640 * 480 - 1) then
						vram_addr <= (others => '0');
					else
						vram_addr <= vram_addr + 1;
					end if;
				end if;
				
			end if;
		end if;
		
	end process;
	
	
	
delay : process(clk)
	
	begin
		if rising_edge(clk) then
			out_vram_address <= std_logic_vector(vram_addr);
			out_vde <= vde;
			out_hsync <= hsync;
			out_vsync <= vsync;
		end if;
	end process;
		
end behavioral;