################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.c \
../CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.c 

OBJS += \
./CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.o \
./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.o 

C_DEPS += \
./CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.d \
./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/DSP/Source/ControllerFunctions/%.o CMSIS/DSP/Source/ControllerFunctions/%.su CMSIS/DSP/Source/ControllerFunctions/%.cyclo: ../CMSIS/DSP/Source/ControllerFunctions/%.c CMSIS/DSP/Source/ControllerFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS/DSP/Include" -O0 -ffunction-sections -fdata-sections -Wall -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CMSIS-2f-DSP-2f-Source-2f-ControllerFunctions

clean-CMSIS-2f-DSP-2f-Source-2f-ControllerFunctions:
	-$(RM) ./CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.cyclo ./CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.d ./CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.o ./CMSIS/DSP/Source/ControllerFunctions/ControllerFunctions.su ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.d ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.o ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_f32.su ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.d ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.o ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q15.su ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.d ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.o ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_init_q31.su ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.d ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.o ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_f32.su ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.d ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.o ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q15.su ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.d ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.o ./CMSIS/DSP/Source/ControllerFunctions/arm_pid_reset_q31.su ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.d ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.o ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_f32.su ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.cyclo ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.d ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.o ./CMSIS/DSP/Source/ControllerFunctions/arm_sin_cos_q31.su

.PHONY: clean-CMSIS-2f-DSP-2f-Source-2f-ControllerFunctions

