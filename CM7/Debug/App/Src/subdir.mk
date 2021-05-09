################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU ARM Embedded(10 2020-q4-major)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../App/Src/stream_test.cpp 

OBJS += \
./App/Src/stream_test.o 

CPP_DEPS += \
./App/Src/stream_test.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/stream_test.o: ../App/Src/stream_test.cpp App/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++17 -g3 -DDEBUG -DCORE_CM7 -DUSE_HAL_DRIVER -DSTM32H745xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Abstract -I../App/Inc -Os -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -std=gnu++20 -fstack-usage -MMD -MP -MF"App/Src/stream_test.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

