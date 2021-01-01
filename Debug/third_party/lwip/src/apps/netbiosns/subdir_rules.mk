################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
third_party/lwip/src/apps/netbiosns/%.o: ../third_party/lwip/src/apps/netbiosns/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/home/islam/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc-9.2.1" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C1294NCPDT -I"/home/islam/code/workspace_v10/demo" -I"/home/islam/code/workspace_v10/demo/inc" -I"/home/islam/code/workspace_v10/demo/driverlib" -I"/home/islam/code/workspace_v10/demo/third_party/FreeRTOSv202012.00/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"/home/islam/code/workspace_v10/demo/src" -I"/home/islam/code/workspace_v10/demo/third_party/FreeRTOSv202012.00/FreeRTOS/Source/include" -I"/home/islam/code/workspace_v10/demo/third_party/lwip/src/include" -I"/home/islam/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/include" -Og -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -std=c99 -Dgcc $(GEN_OPTS__FLAG) -o"$@" "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


