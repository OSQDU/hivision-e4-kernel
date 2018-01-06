#
# Makefile for the linux kernel.
#
#

# Common objects
obj-y 			:= time.o clock.o serial.o \
			   sram.o irq.o iomux.o pmu.o pm.o sram.o
# gpio.o
# Chip specific
obj-$(CONFIG_ARCH_FH81)		+= fh81.o devices.o
obj-$(CONFIG_ARCH_WUDANG)       += fh81.o devices.o
obj-$(CONFIG_ARCH_LINBAO)       += linbao.o devices.o

# Board specific
obj-$(CONFIG_MACH_FH81)  	+= board-fh81.o
obj-$(CONFIG_MACH_WUDANG)  	+= board-fh81.o
obj-$(CONFIG_MACH_LINBAO_FPGA) 	+= board-linbaofpga.o
obj-$(CONFIG_MACH_LINBAO) 	+= board-linbao.o pinctrl.o

# Power Management
obj-$(CONFIG_CPU_FREQ)		+= cpufreq.o
obj-$(CONFIG_CPU_IDLE)		+= cpuidle.o
obj-$(CONFIG_SUSPEND)		+= pm.o sleep.o
