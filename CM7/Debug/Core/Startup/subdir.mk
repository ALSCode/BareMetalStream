################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU ARM Embedded(10 2020-q4-major)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32h745zitx.s 

S_DEPS += \
./Core/Startup/startup_stm32h745zitx.d 

OBJS += \
./Core/Startup/startup_stm32h745zitx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32h745zitx.o: ../Core/Startup/startup_stm32h745zitx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32h745zitx.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

