################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c \
../utilities/fsl_str.c 

C_DEPS += \
./utilities/fsl_debug_console.d \
./utilities/fsl_str.d 

OBJS += \
./utilities/fsl_debug_console.o \
./utilities/fsl_str.o 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -DSDK_OS_FREE_RTOS -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\board" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\utilities" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\drivers" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\device" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\component\serial_manager" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\component\lists" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\CMSIS" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\component\uart" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\freertos\freertos_kernel\include" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\source" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\drivers\freertos" -I"C:\Users\maria\Desktop\UNC Drive\Procesamiento digital de señales\WorkSpace\Laboratorio_1_PDS\freertos\freertos_kernel\portable\GCC\ARM_CM4F" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-utilities

clean-utilities:
	-$(RM) ./utilities/fsl_debug_console.d ./utilities/fsl_debug_console.o ./utilities/fsl_str.d ./utilities/fsl_str.o

.PHONY: clean-utilities

