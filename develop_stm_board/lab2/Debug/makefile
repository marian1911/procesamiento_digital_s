################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include Drivers/CMSIS-DSP/Source/WindowFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/TransformFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/SupportFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/StatisticsFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/SVMFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/QuaternionMathFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/MatrixFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/InterpolationFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/FilteringFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/FastMathFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/DistanceFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/ControllerFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/ComplexMathFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/CommonTables/subdir.mk
-include Drivers/CMSIS-DSP/Source/BayesFunctions/subdir.mk
-include Drivers/CMSIS-DSP/Source/BasicMathFunctions/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := lab1
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
lab1.elf \

MAP_FILES += \
lab1.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
lab1.list \


# All Target
all: main-build

# Main-build Target
main-build: lab1.elf secondary-outputs

# Tool invocations
lab1.elf lab1.map: $(OBJS) $(USER_OBJS) C:\FACULTAD\Materias--4to-2do\Procesamiento\ Digital\ de\ Señales\procesamiento_digital_s\develop_stm_board\lab2\STM32F446RETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "lab1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\FACULTAD\Materias--4to-2do\Procesamiento Digital de Señales\procesamiento_digital_s\develop_stm_board\lab2\STM32F446RETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="lab1.map" -Wl,--gc-sections -static -L"C:\FACULTAD\Materias--4to-2do\Procesamiento Digital de Señales\procesamiento_digital_s\develop_stm_board\lab2\Drivers" -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

lab1.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "lab1.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout lab1.elf lab1.list lab1.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
