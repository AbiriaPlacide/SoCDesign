onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /tb_EightxOneMux/Read
add wave -noupdate /tb_EightxOneMux/Din0
add wave -noupdate /tb_EightxOneMux/Din1
add wave -noupdate /tb_EightxOneMux/Din2
add wave -noupdate /tb_EightxOneMux/Din3
add wave -noupdate /tb_EightxOneMux/Din4
add wave -noupdate /tb_EightxOneMux/Din5
add wave -noupdate /tb_EightxOneMux/Din6
add wave -noupdate /tb_EightxOneMux/Din7
add wave -noupdate /tb_EightxOneMux/Output
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {23 ns} 0}
quietly wave cursor active 1
configure wave -namecolwidth 150
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
WaveRestoreZoom {6 ns} {133 ns}
