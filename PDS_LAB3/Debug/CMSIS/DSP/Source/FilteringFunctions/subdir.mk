################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.c 

C_DEPS += \
./CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.d 

OBJS += \
./CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP/Source/FilteringFunctions/%.o: ../CMSIS/DSP/Source/FilteringFunctions/%.c CMSIS/DSP/Source/FilteringFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\board" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\source" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\utilities" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\drivers" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\device" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\component\serial_manager" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\component\lists" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\CMSIS" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\component\uart" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\CMSIS\DSP\Include" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\CMSIS_driver" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\phy" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB3\mdio" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS-2f-DSP-2f-Source-2f-FilteringFunctions

clean-CMSIS-2f-DSP-2f-Source-2f-FilteringFunctions:
	-$(RM) ./CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.d ./CMSIS/DSP/Source/FilteringFunctions/FilteringFunctions.o

.PHONY: clean-CMSIS-2f-DSP-2f-Source-2f-FilteringFunctions

