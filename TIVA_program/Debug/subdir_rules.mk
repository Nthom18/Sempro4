################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/Programmer_P/CCS8/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DPART_TM4C123GH6PM -I"C:/Users/frida/Documents/4_semester/Projekt/TempSemapro4/Tiva_program" -I"C:/Programmer_P/CCS8/ccsv8/tools/compiler/gcc-arm-none-eabi-7-2017-q4-major-win32/arm-none-eabi/include" -I"C:/Users/frida/Documents/4_semester/Projekt/Sempro4/TIVA_program/includes" -I"C:/Users/frida/Documents/4_semester/Projekt/Sempro4/TIVA_program/frt10/inc" -I"C:/Users/frida/Documents/4_semester/Projekt/Sempro4/TIVA_program/frt10/port/TivaM4" -Og -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall -specs="nosys.specs" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


