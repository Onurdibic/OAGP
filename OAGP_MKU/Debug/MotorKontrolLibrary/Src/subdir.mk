################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../MotorKontrolLibrary/Src/Motor.cpp 

OBJS += \
./MotorKontrolLibrary/Src/Motor.o 

CPP_DEPS += \
./MotorKontrolLibrary/Src/Motor.d 


# Each subdirectory must supply rules for building sources it contributes
MotorKontrolLibrary/Src/%.o MotorKontrolLibrary/Src/%.su MotorKontrolLibrary/Src/%.cyclo: ../MotorKontrolLibrary/Src/%.cpp MotorKontrolLibrary/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m3 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_MKU/MotorKontrolLibrary" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_MKU/MotorKontrolLibrary/Inc" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_MKU/PaketLibrary/Inc" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_MKU/PaketLibrary" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MotorKontrolLibrary-2f-Src

clean-MotorKontrolLibrary-2f-Src:
	-$(RM) ./MotorKontrolLibrary/Src/Motor.cyclo ./MotorKontrolLibrary/Src/Motor.d ./MotorKontrolLibrary/Src/Motor.o ./MotorKontrolLibrary/Src/Motor.su

.PHONY: clean-MotorKontrolLibrary-2f-Src

