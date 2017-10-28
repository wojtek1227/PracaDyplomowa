

vcom -reportprogress 300 -work work C:/Users/caput/OneDrive/PracaDyplomowa/PISO/serializer.vhd
vcom -reportprogress 300 -work work C:/Users/caput/OneDrive/PracaDyplomowa/PISO/simulation/modelsim/tb_serializer.vhd

vsim work.tb_serializer


view signals
view wave
add wave -noupdate /tb_serializer/uut/rst
add wave -noupdate /tb_serializer/uut/clk
add wave -noupdate /tb_serializer/uut/ce
add wave -noupdate /tb_serializer/uut/data_in
add wave -noupdate /tb_serializer/uut/data_out_p
add wave -noupdate /tb_serializer/uut/data_out_n

run -all