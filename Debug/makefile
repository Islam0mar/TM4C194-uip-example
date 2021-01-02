################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /home/islam/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./driverlib/adc.o" \
"./driverlib/aes.o" \
"./driverlib/can.o" \
"./driverlib/comp.o" \
"./driverlib/cpu.o" \
"./driverlib/crc.o" \
"./driverlib/des.o" \
"./driverlib/eeprom.o" \
"./driverlib/emac.o" \
"./driverlib/epi.o" \
"./driverlib/flash.o" \
"./driverlib/fpu.o" \
"./driverlib/gpio.o" \
"./driverlib/hibernate.o" \
"./driverlib/i2c.o" \
"./driverlib/interrupt.o" \
"./driverlib/lcd.o" \
"./driverlib/mpu.o" \
"./driverlib/onewire.o" \
"./driverlib/pwm.o" \
"./driverlib/qei.o" \
"./driverlib/shamd5.o" \
"./driverlib/ssi.o" \
"./driverlib/sw_crc.o" \
"./driverlib/sysctl.o" \
"./driverlib/sysexc.o" \
"./driverlib/systick.o" \
"./driverlib/timer.o" \
"./driverlib/uart.o" \
"./driverlib/udma.o" \
"./driverlib/usb.o" \
"./driverlib/watchdog.o" \
"./src/bsp.o" \
"./src/logger.o" \
"./src/main.o" \
"./src/ring-buffer.o" \
"./src/tm4c1294ncpdt_startup_ccs_gcc.o" \
"./third_party/uip/lib/memb.o" \
"./third_party/uip/uip/psock.o" \
"./third_party/uip/uip/timer.o" \
"./third_party/uip/uip/uip-fw.o" \
"./third_party/uip/uip/uip-neighbor.o" \
"./third_party/uip/uip/uip.o" \
"./third_party/uip/uip/uip_arp.o" \
"./third_party/uip/uip/uiplib.o" \
-Wl,-T"../tm4c1294ncpdt.lds" \
$(GEN_CMDS__FLAG) \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include driverlib/subdir_vars.mk
-include src/subdir_vars.mk
-include third_party/uip/lib/subdir_vars.mk
-include third_party/uip/uip/subdir_vars.mk
-include subdir_rules.mk
-include driverlib/subdir_rules.mk
-include src/subdir_rules.mk
-include third_party/uip/lib/subdir_rules.mk
-include third_party/uip/uip/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
demo.elf \

EXE_OUTPUTS__QUOTED += \
"demo.elf" \

BIN_OUTPUTS += \
demo.hex \

BIN_OUTPUTS__QUOTED += \
"demo.hex" \


# All Target
all: $(OBJS) $(LDS_SRCS) $(LIB_SRCS) $(A_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "demo.elf" secondary-outputs

# Tool invocations
demo.elf: $(OBJS) $(LDS_SRCS) $(LIB_SRCS) $(A_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: GNU Linker'
	"/home/islam/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc-9.2.1" -mfpu=fpv4-sp-d16 -Dgcc -Og -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -mcpu=cortex-m4 -Wl,-Map,"demo.map" -mfloat-abi=hard -o"demo.elf" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: "$@"'
	@echo ' '

demo.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: GNU Objcopy Utility'
	"/home/islam/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-objcopy"  $(EXE_OUTPUTS__QUOTED) "demo.hex"
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "driverlib/adc.o" "driverlib/aes.o" "driverlib/can.o" "driverlib/comp.o" "driverlib/cpu.o" "driverlib/crc.o" "driverlib/des.o" "driverlib/eeprom.o" "driverlib/emac.o" "driverlib/epi.o" "driverlib/flash.o" "driverlib/fpu.o" "driverlib/gpio.o" "driverlib/hibernate.o" "driverlib/i2c.o" "driverlib/interrupt.o" "driverlib/lcd.o" "driverlib/mpu.o" "driverlib/onewire.o" "driverlib/pwm.o" "driverlib/qei.o" "driverlib/shamd5.o" "driverlib/ssi.o" "driverlib/sw_crc.o" "driverlib/sysctl.o" "driverlib/sysexc.o" "driverlib/systick.o" "driverlib/timer.o" "driverlib/uart.o" "driverlib/udma.o" "driverlib/usb.o" "driverlib/watchdog.o" "src/bsp.o" "src/logger.o" "src/main.o" "src/ring-buffer.o" "src/tm4c1294ncpdt_startup_ccs_gcc.o" "third_party/uip/lib/memb.o" "third_party/uip/uip/psock.o" "third_party/uip/uip/timer.o" "third_party/uip/uip/uip-fw.o" "third_party/uip/uip/uip-neighbor.o" "third_party/uip/uip/uip.o" "third_party/uip/uip/uip_arp.o" "third_party/uip/uip/uiplib.o" 
	-$(RM) "driverlib/adc.d" "driverlib/aes.d" "driverlib/can.d" "driverlib/comp.d" "driverlib/cpu.d" "driverlib/crc.d" "driverlib/des.d" "driverlib/eeprom.d" "driverlib/emac.d" "driverlib/epi.d" "driverlib/flash.d" "driverlib/fpu.d" "driverlib/gpio.d" "driverlib/hibernate.d" "driverlib/i2c.d" "driverlib/interrupt.d" "driverlib/lcd.d" "driverlib/mpu.d" "driverlib/onewire.d" "driverlib/pwm.d" "driverlib/qei.d" "driverlib/shamd5.d" "driverlib/ssi.d" "driverlib/sw_crc.d" "driverlib/sysctl.d" "driverlib/sysexc.d" "driverlib/systick.d" "driverlib/timer.d" "driverlib/uart.d" "driverlib/udma.d" "driverlib/usb.d" "driverlib/watchdog.d" "src/bsp.d" "src/logger.d" "src/main.d" "src/ring-buffer.d" "src/tm4c1294ncpdt_startup_ccs_gcc.d" "third_party/uip/lib/memb.d" "third_party/uip/uip/psock.d" "third_party/uip/uip/timer.d" "third_party/uip/uip/uip-fw.d" "third_party/uip/uip/uip-neighbor.d" "third_party/uip/uip/uip.d" "third_party/uip/uip/uip_arp.d" "third_party/uip/uip/uiplib.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-@echo 'Display size info.'
	-arm-none-eabi-size -A -d -t /home/islam/code/workspace_v10/demo/Debug/demo.elf
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

