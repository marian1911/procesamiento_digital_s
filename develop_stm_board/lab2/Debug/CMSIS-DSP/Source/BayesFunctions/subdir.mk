################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS-DSP/Source/BayesFunctions/BayesFunctions.c \
../CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.c \
../CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.c \
../CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.c 

OBJS += \
./CMSIS-DSP/Source/BayesFunctions/BayesFunctions.o \
./CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.o \
./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.o \
./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.o 

C_DEPS += \
./CMSIS-DSP/Source/BayesFunctions/BayesFunctions.d \
./CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.d \
./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.d \
./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS-DSP/Source/BayesFunctions/%.o CMSIS-DSP/Source/BayesFunctions/%.su CMSIS-DSP/Source/BayesFunctions/%.cyclo: ../CMSIS-DSP/Source/BayesFunctions/%.c CMSIS-DSP/Source/BayesFunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include/dsp" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/PrivateInclude" -O0 -ffunction-sections -fdata-sections -Wall -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CMSIS-2d-DSP-2f-Source-2f-BayesFunctions

clean-CMSIS-2d-DSP-2f-Source-2f-BayesFunctions:
	-$(RM) ./CMSIS-DSP/Source/BayesFunctions/BayesFunctions.cyclo ./CMSIS-DSP/Source/BayesFunctions/BayesFunctions.d ./CMSIS-DSP/Source/BayesFunctions/BayesFunctions.o ./CMSIS-DSP/Source/BayesFunctions/BayesFunctions.su ./CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.cyclo ./CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.d ./CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.o ./CMSIS-DSP/Source/BayesFunctions/BayesFunctionsF16.su ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.cyclo ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.d ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.o ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f16.su ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.cyclo ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.d ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.o ./CMSIS-DSP/Source/BayesFunctions/arm_gaussian_naive_bayes_predict_f32.su

.PHONY: clean-CMSIS-2d-DSP-2f-Source-2f-BayesFunctions

