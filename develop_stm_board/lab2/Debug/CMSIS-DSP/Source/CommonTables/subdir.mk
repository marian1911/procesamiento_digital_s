################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS-DSP/Source/CommonTables/CommonTables.c \
../CMSIS-DSP/Source/CommonTables/CommonTablesF16.c \
../CMSIS-DSP/Source/CommonTables/arm_common_tables.c \
../CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.c \
../CMSIS-DSP/Source/CommonTables/arm_const_structs.c \
../CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.c \
../CMSIS-DSP/Source/CommonTables/arm_mve_tables.c \
../CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.c \
../CMSIS-DSP/Source/CommonTables/arm_neon_tables.c \
../CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.c 

OBJS += \
./CMSIS-DSP/Source/CommonTables/CommonTables.o \
./CMSIS-DSP/Source/CommonTables/CommonTablesF16.o \
./CMSIS-DSP/Source/CommonTables/arm_common_tables.o \
./CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.o \
./CMSIS-DSP/Source/CommonTables/arm_const_structs.o \
./CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.o \
./CMSIS-DSP/Source/CommonTables/arm_mve_tables.o \
./CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.o \
./CMSIS-DSP/Source/CommonTables/arm_neon_tables.o \
./CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.o 

C_DEPS += \
./CMSIS-DSP/Source/CommonTables/CommonTables.d \
./CMSIS-DSP/Source/CommonTables/CommonTablesF16.d \
./CMSIS-DSP/Source/CommonTables/arm_common_tables.d \
./CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.d \
./CMSIS-DSP/Source/CommonTables/arm_const_structs.d \
./CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.d \
./CMSIS-DSP/Source/CommonTables/arm_mve_tables.d \
./CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.d \
./CMSIS-DSP/Source/CommonTables/arm_neon_tables.d \
./CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS-DSP/Source/CommonTables/%.o CMSIS-DSP/Source/CommonTables/%.su CMSIS-DSP/Source/CommonTables/%.cyclo: ../CMSIS-DSP/Source/CommonTables/%.c CMSIS-DSP/Source/CommonTables/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/Include/dsp" -I"C:/FACULTAD/Materias--4to-2do/Procesamiento Digital de Señales/procesamiento_digital_s/develop_stm_board/lab2/CMSIS-DSP/PrivateInclude" -O0 -ffunction-sections -fdata-sections -Wall -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-CMSIS-2d-DSP-2f-Source-2f-CommonTables

clean-CMSIS-2d-DSP-2f-Source-2f-CommonTables:
	-$(RM) ./CMSIS-DSP/Source/CommonTables/CommonTables.cyclo ./CMSIS-DSP/Source/CommonTables/CommonTables.d ./CMSIS-DSP/Source/CommonTables/CommonTables.o ./CMSIS-DSP/Source/CommonTables/CommonTables.su ./CMSIS-DSP/Source/CommonTables/CommonTablesF16.cyclo ./CMSIS-DSP/Source/CommonTables/CommonTablesF16.d ./CMSIS-DSP/Source/CommonTables/CommonTablesF16.o ./CMSIS-DSP/Source/CommonTables/CommonTablesF16.su ./CMSIS-DSP/Source/CommonTables/arm_common_tables.cyclo ./CMSIS-DSP/Source/CommonTables/arm_common_tables.d ./CMSIS-DSP/Source/CommonTables/arm_common_tables.o ./CMSIS-DSP/Source/CommonTables/arm_common_tables.su ./CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.cyclo ./CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.d ./CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.o ./CMSIS-DSP/Source/CommonTables/arm_common_tables_f16.su ./CMSIS-DSP/Source/CommonTables/arm_const_structs.cyclo ./CMSIS-DSP/Source/CommonTables/arm_const_structs.d ./CMSIS-DSP/Source/CommonTables/arm_const_structs.o ./CMSIS-DSP/Source/CommonTables/arm_const_structs.su ./CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.cyclo ./CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.d ./CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.o ./CMSIS-DSP/Source/CommonTables/arm_const_structs_f16.su ./CMSIS-DSP/Source/CommonTables/arm_mve_tables.cyclo ./CMSIS-DSP/Source/CommonTables/arm_mve_tables.d ./CMSIS-DSP/Source/CommonTables/arm_mve_tables.o ./CMSIS-DSP/Source/CommonTables/arm_mve_tables.su ./CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.cyclo ./CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.d ./CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.o ./CMSIS-DSP/Source/CommonTables/arm_mve_tables_f16.su ./CMSIS-DSP/Source/CommonTables/arm_neon_tables.cyclo ./CMSIS-DSP/Source/CommonTables/arm_neon_tables.d ./CMSIS-DSP/Source/CommonTables/arm_neon_tables.o ./CMSIS-DSP/Source/CommonTables/arm_neon_tables.su ./CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.cyclo ./CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.d ./CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.o ./CMSIS-DSP/Source/CommonTables/arm_neon_tables_f16.su

.PHONY: clean-CMSIS-2d-DSP-2f-Source-2f-CommonTables

