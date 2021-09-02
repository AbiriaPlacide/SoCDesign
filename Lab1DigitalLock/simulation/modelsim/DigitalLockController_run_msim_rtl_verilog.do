transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+/home/abiria/SoCDesign_Class/Lab1DigitalLock {/home/abiria/SoCDesign_Class/Lab1DigitalLock/DigitalLockController.v}

