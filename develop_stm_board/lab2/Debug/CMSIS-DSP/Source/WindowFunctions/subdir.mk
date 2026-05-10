################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS-DSP/Source/WindowFunctions/WindowFunctions.c \
../CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.c \
../CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.c \
../CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.c 

OBJS += \
./CMSIS-DSP/Source/WindowFunctions/WindowFunctions.o \
./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.o \
./CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.o \
./CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.o 

C_DEPS += \
./CMSIS-DSP/Source/WindowFunctions/WindowFunctions.d \
./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.d \
./CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.d \
./CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS-DSP/Source/WindowFunctions/%.o CMSIS-DSP/Source/WindowFunctions/%.su CMSIS-DSP/Source/WindowFunctions/%.cyclo: ../CMSIS-DSP/Source/WindowFunctions/%.c CMSIS-DSP/Source/WindowFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include/dsp" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/PrivateInclude" -O0 -ffunction-sections -fdata-sections -Wall -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CMSIS-2d-DSP-2f-Source-2f-WindowFunctions

clean-CMSIS-2d-DSP-2f-Source-2f-WindowFunctions:
	-$(RM) ./CMSIS-DSP/Source/WindowFunctions/WindowFunctions.cyclo ./CMSIS-DSP/Source/WindowFunctions/WindowFunctions.d ./CMSIS-DSP/Source/WindowFunctions/WindowFunctions.o ./CMSIS-DSP/Source/WindowFunctions/WindowFunctions.su ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_bartlett_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_blackman_harris_92db_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hamming_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hanning_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft116d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft144d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft169d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft196d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft223d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft248d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft90d_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_hft95_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3_f64.su
	-$(RM) ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3a_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall3b_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4a_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4b_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_nuttall4c_f64.su ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.d ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.o ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f32.su ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.cyclo ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.d ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.o ./CMSIS-DSP/Source/WindowFunctions/arm_welch_f64.su

.PHONY: clean-CMSIS-2d-DSP-2f-Source-2f-WindowFunctions

