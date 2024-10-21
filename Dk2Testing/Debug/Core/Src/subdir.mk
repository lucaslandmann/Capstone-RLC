################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/bsp.c \
../Core/Src/error.c \
../Core/Src/log.c \
../Core/Src/main.c \
../Core/Src/mixer.c \
../Core/Src/pcm6260.c \
../Core/Src/perf.c \
../Core/Src/platform.c \
../Core/Src/ring.c \
../Core/Src/shell.c \
../Core/Src/stm32u5xx_hal_msp.c \
../Core/Src/stm32u5xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32u5xx.c \
../Core/Src/timer.c \
../Core/Src/version.c 

OBJS += \
./Core/Src/bsp.o \
./Core/Src/error.o \
./Core/Src/log.o \
./Core/Src/main.o \
./Core/Src/mixer.o \
./Core/Src/pcm6260.o \
./Core/Src/perf.o \
./Core/Src/platform.o \
./Core/Src/ring.o \
./Core/Src/shell.o \
./Core/Src/stm32u5xx_hal_msp.o \
./Core/Src/stm32u5xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32u5xx.o \
./Core/Src/timer.o \
./Core/Src/version.o 

C_DEPS += \
./Core/Src/bsp.d \
./Core/Src/error.d \
./Core/Src/log.d \
./Core/Src/main.d \
./Core/Src/mixer.d \
./Core/Src/pcm6260.d \
./Core/Src/perf.d \
./Core/Src/platform.d \
./Core/Src/ring.d \
./Core/Src/shell.d \
./Core/Src/stm32u5xx_hal_msp.d \
./Core/Src/stm32u5xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32u5xx.d \
./Core/Src/timer.d \
./Core/Src/version.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DLOG_ENABLE -DUSE_HAL_DRIVER -DSTM32U5G9xx -c -I../Core/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/bsp.cyclo ./Core/Src/bsp.d ./Core/Src/bsp.o ./Core/Src/bsp.su ./Core/Src/error.cyclo ./Core/Src/error.d ./Core/Src/error.o ./Core/Src/error.su ./Core/Src/log.cyclo ./Core/Src/log.d ./Core/Src/log.o ./Core/Src/log.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mixer.cyclo ./Core/Src/mixer.d ./Core/Src/mixer.o ./Core/Src/mixer.su ./Core/Src/pcm6260.cyclo ./Core/Src/pcm6260.d ./Core/Src/pcm6260.o ./Core/Src/pcm6260.su ./Core/Src/perf.cyclo ./Core/Src/perf.d ./Core/Src/perf.o ./Core/Src/perf.su ./Core/Src/platform.cyclo ./Core/Src/platform.d ./Core/Src/platform.o ./Core/Src/platform.su ./Core/Src/ring.cyclo ./Core/Src/ring.d ./Core/Src/ring.o ./Core/Src/ring.su ./Core/Src/shell.cyclo ./Core/Src/shell.d ./Core/Src/shell.o ./Core/Src/shell.su ./Core/Src/stm32u5xx_hal_msp.cyclo ./Core/Src/stm32u5xx_hal_msp.d ./Core/Src/stm32u5xx_hal_msp.o ./Core/Src/stm32u5xx_hal_msp.su ./Core/Src/stm32u5xx_it.cyclo ./Core/Src/stm32u5xx_it.d ./Core/Src/stm32u5xx_it.o ./Core/Src/stm32u5xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32u5xx.cyclo ./Core/Src/system_stm32u5xx.d ./Core/Src/system_stm32u5xx.o ./Core/Src/system_stm32u5xx.su ./Core/Src/timer.cyclo ./Core/Src/timer.d ./Core/Src/timer.o ./Core/Src/timer.su ./Core/Src/version.cyclo ./Core/Src/version.d ./Core/Src/version.o ./Core/Src/version.su

.PHONY: clean-Core-2f-Src

