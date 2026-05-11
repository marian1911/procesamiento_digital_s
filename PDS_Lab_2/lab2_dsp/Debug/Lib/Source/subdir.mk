################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Lib/Source/arm_fill_q15.c \
../Lib/Source/arm_fir_init_q15.c \
../Lib/Source/arm_fir_q15.c 

OBJS += \
./Lib/Source/arm_fill_q15.o \
./Lib/Source/arm_fir_init_q15.o \
./Lib/Source/arm_fir_q15.o 

C_DEPS += \
./Lib/Source/arm_fill_q15.d \
./Lib/Source/arm_fir_init_q15.d \
./Lib/Source/arm_fir_q15.d 


# Each subdirectory must supply rules for building sources it contributes
Lib/Source/%.o Lib/Source/%.su Lib/Source/%.cyclo: ../Lib/Source/%.c Lib/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -D__FPU_PRESENT=0 -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/maria/Desktop/Proyectos/STM32 Projet/lab2_dsp/Lib/Include" -I"C:/Users/maria/Desktop/Proyectos/STM32 Projet/lab2_dsp/Drivers" -O0 -ffunction-sections -fdata-sections -Wall -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Lib-2f-Source

clean-Lib-2f-Source:
	-$(RM) ./Lib/Source/arm_fill_q15.cyclo ./Lib/Source/arm_fill_q15.d ./Lib/Source/arm_fill_q15.o ./Lib/Source/arm_fill_q15.su ./Lib/Source/arm_fir_init_q15.cyclo ./Lib/Source/arm_fir_init_q15.d ./Lib/Source/arm_fir_init_q15.o ./Lib/Source/arm_fir_init_q15.su ./Lib/Source/arm_fir_q15.cyclo ./Lib/Source/arm_fir_q15.d ./Lib/Source/arm_fir_q15.o ./Lib/Source/arm_fir_q15.su

.PHONY: clean-Lib-2f-Source

