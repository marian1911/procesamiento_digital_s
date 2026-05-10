################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.c \
../CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.c 

OBJS += \
./CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.o \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.o 

C_DEPS += \
./CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.d \
./CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS-DSP/Source/QuaternionMathFunctions/%.o CMSIS-DSP/Source/QuaternionMathFunctions/%.su CMSIS-DSP/Source/QuaternionMathFunctions/%.cyclo: ../CMSIS-DSP/Source/QuaternionMathFunctions/%.c CMSIS-DSP/Source/QuaternionMathFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include/dsp" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/PrivateInclude" -O0 -ffunction-sections -fdata-sections -Wall -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CMSIS-2d-DSP-2f-Source-2f-QuaternionMathFunctions

clean-CMSIS-2d-DSP-2f-Source-2f-QuaternionMathFunctions:
	-$(RM) ./CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.d ./CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.o ./CMSIS-DSP/Source/QuaternionMathFunctions/QuaternionMathFunctions.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion2rotation_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_conjugate_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_inverse_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_norm_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_normalize_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_quaternion_product_single_f32.su ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.cyclo ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.d ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.o ./CMSIS-DSP/Source/QuaternionMathFunctions/arm_rotation2quaternion_f32.su

.PHONY: clean-CMSIS-2d-DSP-2f-Source-2f-QuaternionMathFunctions

