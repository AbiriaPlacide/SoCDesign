onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_NbitRegister/Data_in
add wave -noupdate /tb_NbitRegister/Data_out
add wave -noupdate /tb_NbitRegister/clock
add wave -noupdate /tb_NbitRegister/clear
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {95 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 177
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ns} {307 ns}
