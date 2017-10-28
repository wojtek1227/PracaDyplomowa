

vcom -reportprogress 300 -work work C:/Users/caput/OneDrive/PracaDyplomowa/Hdmi_Module/CAM/cam_memory.vhd
vcom -reportprogress 300 -work work C:/Users/caput/OneDrive/PracaDyplomowa/Hdmi_Module/CAM/simulation/modelsim/tb_cam_memory.vhd

vsim work.tb_cam_memory


view signals
view wave
add wave -noupdate /tb_cam_memory/uut/rst
add wave -noupdate /tb_cam_memory/uut/clk
add wave -noupdate /tb_cam_memory/uut/ce

add wave -noupdate /tb_cam_memory/uut/we
add wave -noupdate -radix hex /tb_cam_memory/uut/wdata
add wave -noupdate -radix hex /tb_cam_memory/uut/waddr

add wave -noupdate -radix hex /tb_cam_memory/uut/match_data
add wave -noupdate /tb_cam_memory/uut/match_hit
add wave -noupdate -radix hex /tb_cam_memory/uut/match_addr

add wave -noupdate -radix hex /tb_cam_memory/uut/ram

run -all


