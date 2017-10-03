

vcom -reportprogress 300 -work work C:/Users/caput/OneDrive/PracaDyplomowa/TMDS_Encoder/tmds_encoder.vhd
vcom -reportprogress 300 -work work C:/Users/caput/OneDrive/PracaDyplomowa/TMDS_Encoder/simulation/modelsim/tb_tmds_encoder.vhd


vsim work.tb_tmds_encoder


view signals
view wave
add wave -noupdate /tb_tmds_encoder/uut/rst
add wave -noupdate /tb_tmds_encoder/uut/clk
add wave -noupdate /tb_tmds_encoder/uut/ce
add wave -noupdate /tb_tmds_encoder/uut/de
add wave -noupdate /tb_tmds_encoder/uut/c1c0
add wave -noupdate /tb_tmds_encoder/uut/data_in
add wave -noupdate /tb_tmds_encoder/uut/data_out
add wave -noupdate /tb_tmds_encoder/uut/load_to_serial
add wave -noupdate /tb_tmds_encoder/uut/d_int

run -all