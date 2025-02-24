################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.c 

C_DEPS += \
./CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.d 

OBJS += \
./CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.o 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP/Source/ComplexMathFunctions/%.o: ../CMSIS/DSP/Source/ComplexMathFunctions/%.c CMSIS/DSP/Source/ComplexMathFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSERIAL_PORT_TYPE_UART=1 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\board" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\utilities" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\drivers" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\device" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\component\serial_manager" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\component\lists" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\CMSIS" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\component\uart" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\CMSIS\DSP\Include" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\CMSIS_driver" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\phy" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\mdio" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\PDS_LAB2\source" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-CMSIS-2f-DSP-2f-Source-2f-ComplexMathFunctions

clean-CMSIS-2f-DSP-2f-Source-2f-ComplexMathFunctions:
	-$(RM) ./CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.d ./CMSIS/DSP/Source/ComplexMathFunctions/ComplexMathFunctions.o

.PHONY: clean-CMSIS-2f-DSP-2f-Source-2f-ComplexMathFunctions

