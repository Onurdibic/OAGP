################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PaketLibrary/Src/Paket.cpp 

OBJS += \
./PaketLibrary/Src/Paket.o 

CPP_DEPS += \
./PaketLibrary/Src/Paket.d 


# Each subdirectory must supply rules for building sources it contributes
PaketLibrary/Src/%.o PaketLibrary/Src/%.su PaketLibrary/Src/%.cyclo: ../PaketLibrary/Src/%.cpp PaketLibrary/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/ImuLibrary" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/ImuLibrary/Inc" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/GpsLibrary" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/GpsLibrary/Inc" -I"C:/Users/T_rab/OneDrive/Desktop/STM32/OAGP/OAGP_ANKU/PaketLibrary/Inc" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-PaketLibrary-2f-Src

clean-PaketLibrary-2f-Src:
	-$(RM) ./PaketLibrary/Src/Paket.cyclo ./PaketLibrary/Src/Paket.d ./PaketLibrary/Src/Paket.o ./PaketLibrary/Src/Paket.su

.PHONY: clean-PaketLibrary-2f-Src

