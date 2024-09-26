################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c \
../source/semihost_hardfault.c 

C_DEPS += \
./source/main.d \
./source/semihost_hardfault.d 

OBJS += \
./source/main.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\board" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\source" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\utilities" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\drivers" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\device" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\component\serial_manager" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\component\lists" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\CMSIS" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\component\uart" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\CMSIS\DSP\Include" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\CMSIS_driver" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\phy" -I"C:\Users\enzog\Documents\MCUXpressoIDE_11.10.0_3148\workspace\Lab_1_PDS\mdio" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/main.d ./source/main.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

